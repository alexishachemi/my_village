# views/main_window.py
from PySide6.QtWidgets import QMainWindow, QTabWidget
from app.views.loading_tab import LoadingTab
from app.views.terrain_props_tab import TerrainPropsTab
from app.views.relations_tab import RelationsTab
from app.views.biome_tab import BiomeTab
from app.data_manager import DataManager


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Tileset Manager")
        self.setGeometry(100, 100, 1200, 800)

        self.data_manager = DataManager()
        # Pass data_manager to tabs
        self.loading_tab = LoadingTab(self.data_manager)
        self.terrain_props_tab = TerrainPropsTab(self.data_manager)
        self.relations_tab = RelationsTab(self.data_manager)
        self.biome_tab = BiomeTab(self.data_manager)

        self.tab_widget = QTabWidget(self)
        self.setCentralWidget(self.tab_widget)

        self.loading_tab = LoadingTab(self.data_manager)
        self.terrain_props_tab = TerrainPropsTab(self.data_manager)
        self.relations_tab = RelationsTab(self.data_manager)
        self.biome_tab = BiomeTab(self.data_manager)

        self.tab_widget.addTab(self.loading_tab, "Loading / Creation")
        self.tab_widget.addTab(self.terrain_props_tab, "Terrain & Props")
        self.tab_widget.addTab(self.relations_tab, "Relations")
        self.tab_widget.addTab(self.biome_tab, "Biome")

        # ensure populate prop is called when tab Terrain prop is selected
        self.tab_widget.currentChanged.connect(self.on_tab_changed)

    def on_tab_changed(self, index):
        print(f"Tab changed to {index}")
        if index == 1:
            self.terrain_props_tab.populate_tiles()
        if index == 2:
            pass
            # self.relations_tab.populate_terrain_props()
        if index == 3:
            pass
            # self.biome_tab.populate_biomes()

