"""
Module containg the tileset tab, which is used to
display and edit tilesets for assets.
"""

from PySide6.QtCore import Signal
from PySide6.QtGui import QAction, QIcon
from PySide6.QtWidgets import (
    QHBoxLayout,
    QListWidget,
    QStackedWidget,
    QToolBar,
    QVBoxLayout,
    QWidget,
)

from app.core import TilesetInfos
from app.items import UnnamedMonoAsset

from .tileset_view import TilesetView


class TilesetTab(QWidget):
    """
    Each tab (Terrains or Assets) has:
      - A toolbar with add/remove/(pen),
      - A list of tileset files on the left,
      - A QStackedWidget of TilesetView on the right.
    """

    selected_changed = Signal(list)

    def __init__(
        self, tileset_infos: TilesetInfos, tile_size: int, has_pen: bool
    ):
        super().__init__()
        self.tileset_infos = tileset_infos
        self.tile_size = tile_size
        self.has_pen = has_pen

        # Layout
        main_layout = QVBoxLayout(self)
        self.toolbar = QToolBar()
        main_layout.addWidget(self.toolbar)

        # Horizontal area: list on the left, stacked widget on the right
        content_layout = QHBoxLayout()
        main_layout.addLayout(content_layout)

        self.listWidget = QListWidget()
        content_layout.addWidget(self.listWidget)

        self.stack = QStackedWidget()
        content_layout.addWidget(self.stack)

        # Create a TilesetView per path
        self.views: list[TilesetView] = []
        for key, info in self.tileset_infos.items():
            view = TilesetView((key, info["path"]), tile_size, has_pen)
            self.stack.addWidget(view)
            self.views.append(view)
            # Also add to the list widget
            self.listWidget.addItem(key)
            # connect signals
            view.selected_changed.connect(self.emitSelectedChanged)

        # Make the first item selected by default
        if self.tileset_infos:
            self.listWidget.setCurrentRow(0)
            self.stack.setCurrentIndex(0)

        # Hook list changes to stacked widget
        self.listWidget.currentRowChanged.connect(self.stack.setCurrentIndex)

        # Toolbar actions
        self.actAdd = QAction(QIcon("assets/approuve.png"), "Add")
        self.actRemove = QAction(QIcon("assets/croix-rouge.png"), "Remove")
        self.toolbar.addAction(self.actAdd)
        self.toolbar.addAction(self.actRemove)

        self.actAdd.triggered.connect(self.onAddTriggered)
        self.actRemove.triggered.connect(self.onRemoveTriggered)

        # Optionally pen
        if has_pen:
            self.actPen = QAction(QIcon("assets/cogwheel.png"), "Pen")
            self.toolbar.addAction(self.actPen)
            self.actPen.triggered.connect(self.onPenTriggered)

    def onAddTriggered(self):
        """Set the mode to add."""
        view = self.getCurrentView()
        if view:
            view.setAddMode()

    def onRemoveTriggered(self):
        """Set the mode to remove."""
        view = self.getCurrentView()
        if view:
            view.setRemoveMode()

    def onPenTriggered(self):
        """Set the mode to pen."""
        view = self.getCurrentView()
        if view:
            view.setPenMode()

    def getCurrentView(self) -> TilesetView | None:
        """Return the current TilesetView."""
        idx = self.stack.currentIndex()
        if idx < 0:
            return None
        return self.views[idx]

    def getAllSelected(self) -> list[UnnamedMonoAsset]:
        """
        Return all rectangles from all TilesetViews in this tab,
        aggregated across all file paths.
        """
        coords = []
        for view in self.views:
            coords.extend(view.getAllSelected())
        return coords

    def emitSelectedChanged(self):
        """Emit the selected changed signal."""
        self.selected_changed.emit(self.getAllSelected())
