# views/terrain_props_tab.py
from PySide6.QtWidgets import (
    QWidget, QHBoxLayout, QVBoxLayout, QPushButton, QListWidget, QLabel, QLineEdit, QComboBox
)
from PySide6.QtGui import QPixmap, QDrag
from PySide6.QtCore import Qt, QMimeData, QSize
from models.terrain_prop import TerrainProp
from models.tileset import Tile
from PySide6.QtWidgets import QListWidgetItem

class TileListWidget(QListWidget):
    def __init__(self):
        super().__init__()
        self.setViewMode(QListWidget.IconMode)
        self.setIconSize(QSize(64, 64))
        self.setSpacing(10)
        self.setAcceptDrops(False)
        self.setDragEnabled(True)

    def populate(self, tiles):
        self.clear()
        for tile in tiles:
            item = QListWidgetItem()
            item.setIcon(QIcon(tile.get_pixmap()))
            item.setData(Qt.UserRole, tile)
            self.addItem(item)

class TileDropArea(QListWidget):
    def __init__(self):
        super().__init__()
        self.setViewMode(QListWidget.IconMode)
        self.setIconSize(QSize(64, 64))
        self.setSpacing(10)
        self.setAcceptDrops(True)
        self.setDragEnabled(True)
        self.setDragDropMode(QListWidget.InternalMove)

    def dragEnterEvent(self, event):
        if event.mimeData().hasImage() or event.mimeData().hasFormat('application/x-qabstractitemmodeldatalist'):
            event.accept()
        else:
            event.ignore()

    def dropEvent(self, event):
        source = event.source()
        if source == self:
            super().dropEvent(event)
        else:
            # Accept the drop from the tile list
            item = source.itemAt(event.pos())
            if item:
                tile = item.data(Qt.UserRole)
                new_item = QListWidgetItem()
                new_item.setIcon(QIcon(tile.get_pixmap()))
                new_item.setData(Qt.UserRole, tile)
                self.addItem(new_item)
            event.accept()

class TerrainPropsTab(QWidget):
    def __init__(self, data_manager):
        super().__init__()
        self.data_manager = data_manager
        self.init_ui()

    def init_ui(self):
        self.main_layout = QHBoxLayout()

        # Left column: Tile previews
        self.tile_list = QListWidget()
        self.tile_list.setViewMode(QListWidget.IconMode)
        self.tile_list.setIconSize(QSize(64, 64))
        self.tile_list.setSpacing(10)
        self.tile_list.setAcceptDrops(False)
        self.tile_list.setDragEnabled(True)
        # TODO: Populate tile_list with tiles
        self.main_layout.addWidget(self.tile_list)

        # Right column: Terrain & Props list and form
        self.right_layout = QVBoxLayout()
        self.terrain_prop_list = QListWidget()
        self.terrain_prop_list.itemClicked.connect(self.display_terrain_prop_form)
        self.right_layout.addWidget(self.terrain_prop_list)

        self.add_button = QPushButton("+")
        self.add_button.clicked.connect(self.add_terrain_prop)
        self.right_layout.addWidget(self.add_button)

        # Form for terrain/prop details
        self.form_layout = QVBoxLayout()
        self.form_widget = QWidget()
        self.form_widget.setLayout(self.form_layout)

        self.preview_label = QLabel("Preview:")
        self.form_layout.addWidget(self.preview_label)

        self.name_label = QLabel("Name:")
        self.name_input = QLineEdit()
        self.form_layout.addWidget(self.name_label)
        self.form_layout.addWidget(self.name_input)

        self.type_label = QLabel("Type:")
        self.type_combo = QComboBox()
        self.type_combo.addItems(["Terrain", "Prop"])
        self.type_combo.currentTextChanged.connect(self.toggle_prop_type)
        self.form_layout.addWidget(self.type_label)
        self.form_layout.addWidget(self.type_combo)

        self.prop_type_label = QLabel("Prop Type:")
        self.prop_type_combo = QComboBox()
        self.prop_type_combo.addItems(["type1", "type2", "type3", "type4"])
        self.form_layout.addWidget(self.prop_type_label)
        self.form_layout.addWidget(self.prop_type_combo)

        self.tiles_area_label = QLabel("Tiles:")
        self.form_layout.addWidget(self.tiles_area_label)
        # Area where tiles can be dropped
        self.tiles_area = QListWidget()
        self.tiles_area.setViewMode(QListWidget.IconMode)
        self.tiles_area.setIconSize(QSize(64, 64))
        self.tiles_area.setSpacing(10)
        self.tiles_area.setAcceptDrops(True)
        self.tiles_area.setDragEnabled(True)
        self.tiles_area.setDragDropMode(QListWidget.InternalMove)
        self.form_layout.addWidget(self.tiles_area)

        self.ok_button = QPushButton("OK")
        self.ok_button.clicked.connect(self.save_terrain_prop)
        self.form_layout.addWidget(self.ok_button)

        self.right_layout.addWidget(self.form_widget)

        self.main_layout.addLayout(self.right_layout)
        self.setLayout(self.main_layout)

        self.tile_list = TileListWidget()
        self.tile_list.populate(self.data_manager.tiles)
        self.main_layout.addWidget(self.tile_list)

        self.tiles_area = TileDropArea()
        self.form_layout.addWidget(self.tiles_area)

    def toggle_prop_type(self, text):
        is_prop = text == "Prop"
        self.prop_type_label.setVisible(is_prop)
        self.prop_type_combo.setVisible(is_prop)

    def add_terrain_prop(self):
        # Clear form fields for new entry
        self.name_input.clear()
        self.type_combo.setCurrentIndex(0)
        self.prop_type_combo.setCurrentIndex(0)
        self.tiles_area.clear()
        self.form_widget.show()

    def display_terrain_prop_form(self, item):
        # Load selected terrain/prop details into the form
        pass

    def save_terrain_prop(self):
        # Save the terrain/prop data
        # TODO: Implement saving logic
        self.form_widget.hide()
        tp = TerrainProp(
            name=self.name_input.text(),
            is_terrain=self.type_combo.currentText() == "Terrain",
            prop_type=self.prop_type_combo.currentText() if self.type_combo.currentText() == "Prop" else None,
            tiles=[item.data(Qt.UserRole) for item in self.tiles_area.findItems("*", Qt.MatchWildcard)]
        )

        # Add to data_manager
        self.data_manager.terrain_props.append(tp)
        self.terrain_prop_list.addItem(tp.name)
        self.form_widget.hide()
