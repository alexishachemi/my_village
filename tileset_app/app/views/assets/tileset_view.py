"""
Module containing a view for a single tileset.
Allows advanced selection and settings.
"""

from PySide6.QtCore import QRect, Qt, Signal
from PySide6.QtGui import QColor, QPainter, QPen, QPixmap
from PySide6.QtWidgets import QDialog, QSizePolicy, QWidget

from app.items import UnnamedMonoAsset

from .offset_dialog import OffsetDialog


class TilesetView(QWidget):
    """
    Handles selection, painting, and pen-mode offset for a single tileset,
    with scaling + aspect ratio preserved.
    """

    selected_changed = Signal(list)

    def __init__(self, texture: tuple[str, str], tile_size: int, has_pen: bool):
        super().__init__()
        self.texture = texture
        self.tile_size = tile_size
        self.pixmap = QPixmap(texture[1])
        self.has_pen = has_pen

        self.selected: dict[tuple[int, int], QRect] = {}

        self.mode = "add"

        self.is_selecting = False
        self.start_tile = (0, 0)
        self.end_tile = (0, 0)
        self.setMouseTracking(True)

        self.setSizePolicy(
            QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Expanding
        )

    def sizeHint(self):
        return self.pixmap.size()

    # ------------------------------------------------------------------------
    # Helper methods for scaling
    # ------------------------------------------------------------------------
    def _get_scale_params(self):
        """
        Returns (scale_factor, xOffset, yOffset, scaled_pixmap)
        based on current widget size and Qt.KeepAspectRatio scaling.
        """
        available = self.size()
        scaled_pixmap = self.pixmap.scaled(
            available,
            Qt.AspectRatioMode.KeepAspectRatio,
            Qt.TransformationMode.FastTransformation,
        )
        scale_factor = scaled_pixmap.width() / self.pixmap.width()
        # center offsets
        x_off = (available.width() - scaled_pixmap.width()) // 2
        y_off = (available.height() - scaled_pixmap.height()) // 2
        return scale_factor, x_off, y_off, scaled_pixmap

    def _mouse_pos_to_image_coords(self, mouse_pos) -> tuple[float, float]:
        """
        Converts a point in widget coordinates (possibly outside
        or partially scaled) back to unscaled image coordinates.
        """
        scale_factor, x_off, y_off, _ = self._get_scale_params()
        # Shift by offset, then divide by scale
        img_x = (mouse_pos.x() - x_off) / scale_factor
        img_y = (mouse_pos.y() - y_off) / scale_factor
        return img_x, img_y

    def _rect_to_scaled_rect(self, rect: QRect) -> QRect:
        """
        Scales an unscaled QRect to the widget coordinates
        for drawing.
        """
        scale_factor, x_off, y_off, _ = self._get_scale_params()
        sx = int(rect.x() * scale_factor + x_off)
        sy = int(rect.y() * scale_factor + y_off)
        sw = int(rect.width() * scale_factor)
        sh = int(rect.height() * scale_factor)
        return QRect(sx, sy, sw, sh)

    # ------------------------------------------------------------------------
    # Mode switching
    # ------------------------------------------------------------------------
    def set_add_mode(self):
        """Set the mode to add."""
        self.mode = "add"

    def set_remove_mode(self):
        """Set the mode to remove."""
        self.mode = "remove"

    def set_pen_mode(self):
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
            img_x, img_y = self._mouse_pos_to_image_coords(event.position())
            col = int(img_x) // self.tile_size
            row = int(img_y) // self.tile_size
            if row < 0 or col < 0:
                return
            self.is_selecting = True
            self.start_tile = (row, col)
            self.end_tile = (row, col)
            self.update()

    def mouseMoveEvent(self, event):
        """Update the selection rectangle while dragging."""
        img_x, img_y = self._mouse_pos_to_image_coords(event.position())
        col = int(img_x) // self.tile_size
        row = int(img_y) // self.tile_size
        if self.is_selecting and self.mode in ("add", "remove"):
            if row >= 0 and col >= 0:
                self.end_tile = (row, col)
            self.update()
            return
        if row >= 0 and col >= 0:
            self.start_tile = (row, col)
            self.end_tile = (row, col)
        self.update()

    def mouseReleaseEvent(self, event):  # pylint: disable=unused-argument
        """Finish the selection and add/remove tiles."""
        if not self.is_selecting:
            return
        self.is_selecting = False

        row1, col1 = self.start_tile
        row2, col2 = self.end_tile
        r_top, r_bottom = min(row1, row2), max(row1, row2)
        c_left, c_right = min(col1, col2), max(col1, col2)

        self.start_tile = self.end_tile

        # Bound to actual tile count
        max_row = self.pixmap.height() // self.tile_size - 1
        max_col = self.pixmap.width() // self.tile_size - 1
        r_top = max(r_top, 0)
        r_bottom = min(r_bottom, max_row)
        c_left = max(c_left, 0)
        c_right = min(c_right, max_col)

        if self.mode in ("add", "remove"):
            for row in range(r_top, r_bottom + 1):
                for col in range(c_left, c_right + 1):
                    key = (row, col)
                    if self.mode == "add":
                        if key not in self.selected:
                            x = col * self.tile_size
                            y = row * self.tile_size
                            self.selected[key] = QRect(
                                x, y, self.tile_size, self.tile_size
                            )
                    else:  # remove
                        if key in self.selected:
                            del self.selected[key]
            self.update()

        elif self.mode == "pen":
            # Single tile + offset dialog
            # Convert endTile to a rectangle
            row, col = self.end_tile
            x = col * self.tile_size
            y = row * self.tile_size
            max_x, max_y = self.pixmap.width(), self.pixmap.height()

            # show your custom OffsetDialog (assuming it returns a QRect):
            dialog = OffsetDialog(x, y, self.tile_size, max_x, max_y, self)
            if dialog.exec() == QDialog.DialogCode.Accepted:
                self.selected[(row, col)] = QRect(*dialog.get_offsets())
            self.update()

        self.selected_changed.emit(self.get_all_selected())

    # ------------------------------------------------------------------------
    # Painting
    # ------------------------------------------------------------------------
    def paintEvent(self, event):  # pylint: disable=unused-argument
        """Paint the pixmap and selected rectangles."""
        painter = QPainter(self)
        _, x_off, y_off, scaled_pixmap = self._get_scale_params()
        # Draw the scaled pixmap centered
        painter.drawPixmap(x_off, y_off, scaled_pixmap)

        # Draw the selected rectangles in green, scaled
        pen = QPen(QColor("green"))
        pen.setWidth(2)
        painter.setPen(pen)
        for rect in self.selected.values():
            painter.drawRect(self._rect_to_scaled_rect(rect))

        # If dragging for add/remove, show a blue rect in scaled coords
        row1, col1 = self.start_tile
        row2, col2 = self.end_tile
        r_top, r_bottom = min(row1, row2), max(row1, row2)
        c_left, c_right = min(col1, col2), max(col1, col2)

        # Bound tile coords
        max_row = self.pixmap.height() // self.tile_size - 1
        max_col = self.pixmap.width() // self.tile_size - 1
        r_top = max(r_top, 0)
        r_bottom = min(r_bottom, max_row)
        c_left = max(c_left, 0)
        c_right = min(c_right, max_col)

        # cancel if out
        if r_top > max_row or r_bottom < 0 or c_left > max_col or c_right < 0:
            return

        # Create the unscaled selection rect
        x_left = c_left * self.tile_size
        y_top = r_top * self.tile_size
        width = (c_right - c_left + 1) * self.tile_size
        height = (r_bottom - r_top + 1) * self.tile_size

        in_selection = self.is_selecting and self.mode in ("add", "remove")
        pen = QPen(QColor("blue" if in_selection else "yellow"))
        pen.setWidth(2)
        pen.setStyle(Qt.PenStyle.DashLine)
        painter.setPen(pen)

        # Scale and draw it
        unscaled_rect = QRect(x_left, y_top, width, height)
        painter.drawRect(self._rect_to_scaled_rect(unscaled_rect))

    def get_all_selected(self) -> list[UnnamedMonoAsset]:
        """Get all selected rectangles in texture coordinates."""
        return [
            (self.texture[0], r.x(), r.y(), r.width(), r.height())
            for r in self.selected.values()
        ]
