"""
Module containg the tileset tab, which is used to
display and edit tilesets for assets.
"""

from PySide6.QtGui import QAction, QIcon
from PySide6.QtWidgets import (
    QHBoxLayout,
    QListWidget,
    QStackedWidget,
    QToolBar,
    QVBoxLayout,
    QWidget,
)

from app.items import UnnamedMonoAsset

from .tileset_view import TilesetView

class TilesetTab(QWidget):
    """
    Each tab (Terrains or Assets) has:
      - A toolbar with add/remove/(pen),
      - A list of tileset files on the left,
      - A QStackedWidget of TilesetView on the right.
    """

    def __init__(
        self, textures: list[tuple[str, str]], tile_size: int, has_pen: bool
    ):
        super().__init__()
        self.textures = textures
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
        for p in textures:
            view = TilesetView(p, tile_size, has_pen)
            self.stack.addWidget(view)
            self.views.append(view)
            # Also add to the list widget
            self.listWidget.addItem(p[0])

        # Make the first item selected by default
        if textures:
            self.listWidget.setCurrentRow(0)
            self.stack.setCurrentIndex(0)

        # Hook list changes to stacked widget
        self.listWidget.currentRowChanged.connect(self.stack.setCurrentIndex)

        # Toolbar actions
        self.actAdd = QAction(QIcon("approuve.png"), "Add")
        self.actRemove = QAction(QIcon("croix-rouge.png"), "Remove")
        self.toolbar.addAction(self.actAdd)
        self.toolbar.addAction(self.actRemove)

        self.actAdd.triggered.connect(self.onAddTriggered)
        self.actRemove.triggered.connect(self.onRemoveTriggered)

        # Optionally pen
        if has_pen:
            self.actPen = QAction(QIcon("cogwheel.png"), "Pen")
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
