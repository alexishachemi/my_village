# views/main_window.py
from PySide6.QtWidgets import QMainWindow, QTabWidget
from views.loading_tab import LoadingTab
from views.terrain_props_tab import TerrainPropsTab
from views.relations_tab import RelationsTab
from views.biome_tab import BiomeTab
from data_manager import DataManager


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
