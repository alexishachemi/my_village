"""
Module containing a view for a single tileset.
Allows advanced selection and settings.
"""


from PySide6.QtCore import QRect, Qt
from PySide6.QtGui import QColor, QPainter, QPen, QPixmap
from PySide6.QtWidgets import (
    QDialog,
    QWidget,
    QSizePolicy,
)

from app.items import UnnamedMonoAsset

from .offset_dialog import OffsetDialog

class TilesetView(QWidget):
    """
    Handles selection, painting, and pen-mode offset for a single tileset,
    with scaling + aspect ratio preserved.
    """

    def __init__(self, texture: tuple[str, str], tile_size: int, has_pen: bool):
        super().__init__()
        self.texture = texture
        self.tile_size = tile_size
        self.pixmap = QPixmap(texture[1])
        self.has_pen = has_pen

        self.selected: dict[tuple[int, int], QRect] = {}

        self.mode = "add"

        self.isSelecting = False
        self.startTile = (0, 0)
        self.endTile = (0, 0)

        self.setSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Expanding)

    def sizeHint(self):
        return self.pixmap.size()

    # ------------------------------------------------------------------------
    # Helper methods for scaling
    # ------------------------------------------------------------------------
    def _getScaleParams(self):
        """
        Returns (scale_factor, xOffset, yOffset, scaled_pixmap)
        based on current widget size and Qt.KeepAspectRatio scaling.
        """
        available = self.size()
        scaled_pixmap = self.pixmap.scaled(
            available, Qt.AspectRatioMode.KeepAspectRatio, Qt.TransformationMode.FastTransformation
        )
        scale_factor = scaled_pixmap.width() / self.pixmap.width()
        # center offsets
        x_off = (available.width() - scaled_pixmap.width()) // 2
        y_off = (available.height() - scaled_pixmap.height()) // 2
        return scale_factor, x_off, y_off, scaled_pixmap

    def _mousePosToImageCoords(self, mouse_pos) -> tuple[float, float]:
        """
        Converts a point in widget coordinates (possibly outside
        or partially scaled) back to unscaled image coordinates.
        """
        scale_factor, x_off, y_off, _ = self._getScaleParams()
        # Shift by offset, then divide by scale
        img_x = (mouse_pos.x() - x_off) / scale_factor
        img_y = (mouse_pos.y() - y_off) / scale_factor
        return img_x, img_y

    def _rectToScaledRect(self, rect: QRect) -> QRect:
        """
        Scales an unscaled QRect to the widget coordinates
        for drawing. 
        """
        scale_factor, x_off, y_off, _ = self._getScaleParams()
        sx = int(rect.x() * scale_factor + x_off)
        sy = int(rect.y() * scale_factor + y_off)
        sw = int(rect.width() * scale_factor)
        sh = int(rect.height() * scale_factor)
        return QRect(sx, sy, sw, sh)

    # ------------------------------------------------------------------------
    # Mode switching
    # ------------------------------------------------------------------------
    def setAddMode(self):
        """Set the mode to add."""
        self.mode = "add"

    def setRemoveMode(self):
        """Set the mode to remove."""
        self.mode = "remove"

    def setPenMode(self):
        """Set the mode to pen if available."""
        if self.has_pen:
            self.mode = "pen"

    # ------------------------------------------------------------------------
    # Mouse events
    # ------------------------------------------------------------------------
    def mousePressEvent(self, event):
        """Start the selection rectangle."""
        if event.button() == Qt.MouseButton.LeftButton:
            # Convert mouse coords -> unscaled image coords
            img_x, img_y = self._mousePosToImageCoords(event.position())
            col = int(img_x) // self.tile_size
            row = int(img_y) // self.tile_size
            if row < 0 or col < 0:
                return
            self.isSelecting = True
            self.startTile = (row, col)
            self.endTile = (row, col)
            self.update()

    def mouseMoveEvent(self, event):
        """Update the selection rectangle while dragging."""
        if self.isSelecting and self.mode in ("add", "remove"):
            img_x, img_y = self._mousePosToImageCoords(event.position())
            col = int(img_x) // self.tile_size
            row = int(img_y) // self.tile_size
            if row >= 0 and col >= 0:
                self.endTile = (row, col)
            self.update()

    def mouseReleaseEvent(self, event):
        """Finish the selection and add/remove tiles."""
        if not self.isSelecting:
            return
        self.isSelecting = False

        row1, col1 = self.startTile
        row2, col2 = self.endTile
        rTop, rBottom = min(row1, row2), max(row1, row2)
        cLeft, cRight = min(col1, col2), max(col1, col2)

        # Bound to actual tile count
        max_row = self.pixmap.height() // self.tile_size - 1
        max_col = self.pixmap.width() // self.tile_size - 1
        rTop = max(rTop, 0)
        rBottom = min(rBottom, max_row)
        cLeft = max(cLeft, 0)
        cRight = min(cRight, max_col)

        if self.mode in ("add", "remove"):
            for row in range(rTop, rBottom + 1):
                for col in range(cLeft, cRight + 1):
                    key = (row, col)
                    if self.mode == "add":
                        if key not in self.selected:
                            x = col * self.tile_size
                            y = row * self.tile_size
                            self.selected[key] = QRect(x, y, self.tile_size, self.tile_size)
                    else:  # remove
                        if key in self.selected:
                            del self.selected[key]
            self.update()

        elif self.mode == "pen":
            # Single tile + offset dialog
            # Convert endTile to a rectangle
            row, col = self.endTile
            x = col * self.tile_size
            y = row * self.tile_size
            max_x, max_y = self.pixmap.width(), self.pixmap.height()

            # show your custom OffsetDialog (assuming it returns a QRect):
            dialog = OffsetDialog(x, y, self.tile_size, max_x, max_y, self)
            if dialog.exec() == QDialog.DialogCode.Accepted:
                self.selected[(row, col)] = QRect(*dialog.get_offsets())
            self.update()

    # ------------------------------------------------------------------------
    # Painting
    # ------------------------------------------------------------------------
    def paintEvent(self, event):
        """Paint the pixmap and selected rectangles."""
        painter = QPainter(self)
        _, x_off, y_off, scaled_pixmap = self._getScaleParams()
        # Draw the scaled pixmap centered
        painter.drawPixmap(x_off, y_off, scaled_pixmap)

        # Draw the selected rectangles in green, scaled
        pen = QPen(QColor("green"))
        pen.setWidth(2)
        painter.setPen(pen)
        for rect in self.selected.values():
            painter.drawRect(self._rectToScaledRect(rect))

        # If dragging for add/remove, show a blue rect in scaled coords
        if self.isSelecting and self.mode in ("add", "remove"):
            row1, col1 = self.startTile
            row2, col2 = self.endTile
            rTop, rBottom = min(row1, row2), max(row1, row2)
            cLeft, cRight = min(col1, col2), max(col1, col2)

            # Bound tile coords
            max_row = self.pixmap.height() // self.tile_size - 1
            max_col = self.pixmap.width() // self.tile_size - 1
            rTop = max(rTop, 0)
            rBottom = min(rBottom, max_row)
            cLeft = max(cLeft, 0)
            cRight = min(cRight, max_col)

            # cancel if out
            if rTop > max_row or rBottom < 0 or cLeft > max_col or cRight < 0:
                return

            pen = QPen(QColor("blue"))
            pen.setWidth(2)
            pen.setStyle(Qt.PenStyle.DashLine)
            painter.setPen(pen)

            # Create the unscaled selection rect
            xLeft = cLeft * self.tile_size
            yTop = rTop * self.tile_size
            width = (cRight - cLeft + 1) * self.tile_size
            height = (rBottom - rTop + 1) * self.tile_size

            # Scale and draw it
            unscaledRect = QRect(xLeft, yTop, width, height)
            painter.drawRect(self._rectToScaledRect(unscaledRect))

    def getAllSelected(self) -> list[UnnamedMonoAsset]:
        """Get all selected rectangles in texture coordinates."""
        return [
            (self.texture[0], r.x(), r.y(), r.width(), r.height())
            for r in self.selected.values()
        ]
