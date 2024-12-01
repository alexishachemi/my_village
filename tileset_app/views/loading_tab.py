
# views/loading_tab.py
from PySide6.QtWidgets import (
    QWidget, QVBoxLayout, QHBoxLayout, QPushButton, QListWidget,
    QLabel, QLineEdit, QFileDialog
)
from PySide6.QtCore import Qt
from models.tileset import Tileset
# views/loading_tab.py
class LoadingTab(QWidget):
    def __init__(self, data_manager):
        super().__init__()
        self.data_manager = data_manager
        self.init_ui()
        self.tilesets = []  # List to store Tileset instances

    def add_tileset(self):
        options = QFileDialog.Options()
        file_path, _ = QFileDialog.getOpenFileName(
            self, "Select Tileset Image", "", "Images (*.png *.jpg *.bmp)", options=options
        )
        if file_path:
            tileset_name = Path(file_path).stem
            tileset = Tileset(name=tileset_name, path=Path(file_path), tile_size=0)
            self.data_manager.add_tileset(tileset)
            self.tileset_list.addItem(tileset_name)

    def display_tileset_form(self, item):
        # Find the corresponding Tileset instance
        tileset_name = item.text()
        tileset = next((ts for ts in self.data_manager.tilesets if ts.name == tileset_name), None)
        if tileset:
            # Display the tile size input form
            self.tile_size_input.setText(str(tileset.tile_size))
            self.current_tileset = tileset

    def save_tile_size(self):
        if self.current_tileset:
            tile_size = int(self.tile_size_input.text())
            self.current_tileset.tile_size = tile_size
            self.current_tileset.load_tiles()

    def init_ui(self):
        self.main_layout = QVBoxLayout()

        self.new_button = QPushButton("New")
        self.load_button = QPushButton("Load")
        self.main_layout.addWidget(self.new_button)
        self.main_layout.addWidget(self.load_button)

        self.new_button.clicked.connect(self.create_new_project)
        self.load_button.clicked.connect(self.load_project)

        self.setLayout(self.main_layout)

    def create_new_project(self):
        # Switch to tileset loading UI
        self.tileset_layout = QHBoxLayout()

        # Left column: Tileset list and add button
        self.left_layout = QVBoxLayout()
        self.tileset_list = QListWidget()
        self.tileset_list.itemClicked.connect(self.display_tileset_form)
        self.left_layout.addWidget(self.tileset_list)

        self.add_tileset_button = QPushButton("+")
        self.add_tileset_button.clicked.connect(self.add_tileset)
        self.left_layout.addWidget(self.add_tileset_button)

        # Right column: Tileset form
        self.tileset_form_layout = QVBoxLayout()
        self.tileset_form_widget = QWidget()
        self.tileset_form_widget.setLayout(self.tileset_form_layout)

        self.tile_size_label = QLabel("Tile Size (pixels):")
        self.tile_size_input = QLineEdit()
        self.tileset_form_layout.addWidget(self.tile_size_label)
        self.tileset_form_layout.addWidget(self.tile_size_input)

        self.tileset_layout.addLayout(self.left_layout)
        self.tileset_layout.addWidget(self.tileset_form_widget)

        # Clear the main layout and set new layout
        for i in reversed(range(self.main_layout.count())):
            self.main_layout.itemAt(i).widget().setParent(None)
        self.main_layout.addLayout(self.tileset_layout)

    def load_project(self):
        # Placeholder for future implementation
        pass
