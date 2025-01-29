from PySide6.QtWidgets import QMainWindow, QTabWidget

from app.core import TilesetInfos

from .assets import TilesetTab
from .naming import NamedTilesTab


class MainWindow(QMainWindow):
    def __init__(self, tile_size: int, tileset_infos: TilesetInfos):
        super().__init__()
        self.setWindowTitle("Tileset Editor")

        self.tabs = QTabWidget()
        self.setCentralWidget(self.tabs)

        self.terrains_tab = TilesetTab(tileset_infos, tile_size, has_pen=False)
        self.props_tab = TilesetTab(tileset_infos, tile_size, has_pen=True)
        self.terrain_naming_tab = NamedTilesTab(tileset_infos, "Terrain")
        self.props_naming_tab = NamedTilesTab(tileset_infos, "Prop")

        self.terrains_tab.selected_changed.connect(
            self.terrain_naming_tab.onSelected
        )
        self.props_tab.selected_changed.connect(
            self.props_naming_tab.onSelected
        )

        self.tabs.addTab(self.terrains_tab, "Assets for Terrains")
        self.tabs.addTab(self.props_tab, "Assets for Props")
        self.tabs.addTab(self.terrain_naming_tab, "Terrains")
        self.tabs.addTab(self.props_naming_tab, "Props")

    def closeEvent(self, event):
        """
        On close, we gather final results and print them:
          - Named Terrains
          - Discarded Terrains
          - Named Props
          - Discarded Props
        """
        named_terrains = self.terrain_naming_tab.getNamedItems()
        disc_terrains = self.terrain_naming_tab.getDiscardedItems()
        named_props = self.props_naming_tab.getNamedItems()
        disc_props = self.props_naming_tab.getDiscardedItems()

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
