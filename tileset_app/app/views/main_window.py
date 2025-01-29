"""
Module containing the main window view.
"""

from pathlib import Path

from PySide6.QtCore import Qt
from PySide6.QtGui import QPixmap
from PySide6.QtWidgets import QMainWindow, QTabWidget

from app.core import TilesetConfigDict, TilesetInfos

from .assets import TilesetTab
from .config import ConfigTab
from .naming import NamedTilesTab


class MainWindowTabs:
    """Holder for the tabs of the MainWindow."""

    def __init__(self, tileset_infos: TilesetInfos, tile_size: int):

        self.tabs = QTabWidget()

        # Initialize tabs with empty data
        self.config_tab = ConfigTab()
        self.terrains_tab = TilesetTab(tileset_infos, tile_size)
        self.props_tab = TilesetTab(tileset_infos, tile_size)
        self.terrain_naming_tab = NamedTilesTab(tileset_infos, "Terrain")
        self.props_naming_tab = NamedTilesTab(tileset_infos, "Prop")

        # Connect signals
        self.config_tab.config_saved.connect(self.handle_config_update)
        self.terrains_tab.selected_changed.connect(
            self.terrain_naming_tab.on_selected
        )
        self.props_tab.selected_changed.connect(
            self.props_naming_tab.on_selected
        )

        # Add tabs
        self.tabs.addTab(self.config_tab, "Configuration")
        self.tabs.addTab(self.terrains_tab, "Assets for Terrains")
        self.tabs.addTab(self.props_tab, "Assets for Props")
        self.tabs.addTab(self.terrain_naming_tab, "Terrains")
        self.tabs.addTab(self.props_naming_tab, "Props")

    def handle_config_update(self, config: TilesetConfigDict):
        """Handle configuration updates from the config tab"""
        # Update tile size from render config
        tile_size = config["render"]["tile_size_px"]

        # Update tileset information
        new_tileset_infos: TilesetInfos = {}
        for name, path in config["textures"].items():
            path_obj = Path(path)
            pixmap = (
                QPixmap(str(path_obj))
                if path_obj.exists()
                else self.create_fallback_pixmap()
            )
            new_tileset_infos[name] = {"path": str(path_obj), "pixmap": pixmap}

        # # Reload all dependent tabs
        self.terrains_tab.reload_tilesets(new_tileset_infos, tile_size)
        self.props_tab.reload_tilesets(new_tileset_infos, tile_size)
        self.terrain_naming_tab.reload_tilesets(new_tileset_infos)
        self.props_naming_tab.reload_tilesets(new_tileset_infos)

    def create_fallback_pixmap(self):
        """Create a fallback pixmap when the file is not found."""
        pixmap = QPixmap(64, 64)
        pixmap.fill(Qt.GlobalColor.red)
        return pixmap


class MainWindow(QMainWindow):
    """Main Window for the Tileset Editor."""

    def __init__(self):
        super().__init__()
        self.setWindowTitle("Tileset Editor")
        self.setWindowState(Qt.WindowState.WindowMaximized)
        self.tile_size = 32  # Default value
        self.tileset_infos: TilesetInfos = {}

        self.main_tabs = MainWindowTabs(self.tileset_infos, self.tile_size)
        self.setCentralWidget(self.main_tabs.tabs)

    def create_fallback_pixmap(self):
        """Create a fallback pixmap when the file is not found."""
        pixmap = QPixmap(64, 64)
        pixmap.fill(Qt.GlobalColor.red)
        return pixmap

    def closeEvent(self, event):
        """
        On close, we gather final results and print them:
          - Named Terrains
          - Discarded Terrains
          - Named Props
          - Discarded Props
        """
        named_terrains = self.main_tabs.terrain_naming_tab.get_named_items()
        disc_terrains = self.main_tabs.terrain_naming_tab.get_discarded_items()
        named_props = self.main_tabs.props_naming_tab.get_named_items()
        disc_props = self.main_tabs.props_naming_tab.get_discarded_items()

        print("===== Final Named Terrains =====")
        for name_str, tile_coord in named_terrains:
            print(f"{name_str} = {tile_coord}")

        print("\n===== Discarded Terrains =====")
        for tile_coord in disc_terrains:
            print(tile_coord)

        print("\n===== Final Named Props =====")
        for name_str, tile_coord in named_props:
            print(f"{name_str} = {tile_coord}")

        print("\n===== Discarded Props =====")
        for tile_coord in disc_props:
            print(tile_coord)

        super().closeEvent(event)
