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

    def __init__(self, tileset_infos: TilesetInfos, tile_size: int):
        super().__init__()

        print(tileset_infos.keys())
        self.tileset_infos = tileset_infos
        self.tile_size = tile_size

        # Layout
        main_layout = QVBoxLayout(self)
        self.toolbar = QToolBar()
        main_layout.addWidget(self.toolbar)

        # Horizontal area: list on the left, stacked widget on the right
        content_layout = QHBoxLayout()
        main_layout.addLayout(content_layout)

        self.list_widget = QListWidget()
        content_layout.addWidget(self.list_widget)

        self.stack = QStackedWidget()
        content_layout.addWidget(self.stack)

        # Create a TilesetView per path
        self.views: list[TilesetView] = []
        for key, info in self.tileset_infos.items():
            view = TilesetView((key, info["path"]), tile_size)
            self.stack.addWidget(view)
            self.views.append(view)
            # Also add to the list widget
            self.list_widget.addItem(key)
            # connect signals
            view.selected_changed.connect(self.emit_selected_changed)

        # Make the first item selected by default
        if self.tileset_infos:
            self.list_widget.setCurrentRow(0)
            self.stack.setCurrentIndex(0)

        # Hook list changes to stacked widget
        self.list_widget.currentRowChanged.connect(self.stack.setCurrentIndex)

        # Toolbar actions
        self.act_add = QAction(QIcon("assets/approuve.png"), "Add")
        self.act_remove = QAction(QIcon("assets/croix-rouge.png"), "Remove")
        self.toolbar.addAction(self.act_add)
        self.toolbar.addAction(self.act_remove)

        self.act_add.triggered.connect(self.on_add_triggered)
        self.act_remove.triggered.connect(self.on_remove_triggered)

        self.act_pen = QAction(QIcon("assets/cogwheel.png"), "Pen")
        self.toolbar.addAction(self.act_pen)
        self.act_pen.triggered.connect(self.on_pen_triggered)

    def on_add_triggered(self):
        """Set the mode to add."""
        view = self.get_current_view()
        if view:
            view.set_add_mode()

    def on_remove_triggered(self):
        """Set the mode to remove."""
        view = self.get_current_view()
        if view:
            view.set_remove_mode()

    def on_pen_triggered(self):
        """Set the mode to pen."""
        view = self.get_current_view()
        if view:
            view.set_pen_mode()

    def get_current_view(self) -> TilesetView | None:
        """Return the current TilesetView."""
        idx = self.stack.currentIndex()
        if idx < 0:
            return None
        return self.views[idx]

    def get_all_selected(self) -> list[UnnamedMonoAsset]:
        """
        Return all rectangles from all TilesetViews in this tab,
        aggregated across all file paths.
        """
        coords = []
        for view in self.views:
            coords.extend(view.get_all_selected())
        return coords

    def emit_selected_changed(self):
        """Emit the selected changed signal."""
        self.selected_changed.emit(self.get_all_selected())

    def reload_tilesets(self, tileset_infos: TilesetInfos, tile_size: int):
        """Reload the tileset infos."""
        # Create a TilesetView per path
        for key, info in tileset_infos.items():
            if key in self.tileset_infos.keys():
                continue
            view = TilesetView((key, info["path"]), tile_size)
            self.stack.addWidget(view)
            self.views.append(view)
            # Also add to the list widget
            self.list_widget.addItem(key)
            # connect signals
            view.selected_changed.connect(self.emit_selected_changed)
        self.tileset_infos = tileset_infos
        self.tile_size = tile_size
