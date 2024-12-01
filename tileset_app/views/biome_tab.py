# views/biome_tab.py
from PySide6.QtWidgets import (
    QWidget, QHBoxLayout, QVBoxLayout, QLabel, QComboBox, QListWidget, QSpinBox, QPushButton
)
from PySide6.QtCore import Qt
from models.biome import Biome
# views/biome_tab.py
from PySide6.QtWidgets import QInputDialog
from PySide6.QtWidgets import QLineEdit

class BiomeTab(QWidget):
    def __init__(self, data_manager):
        super().__init__()
        self.data_manager = data_manager
        self.init_ui()

    def init_ui(self):
        self.main_layout = QVBoxLayout()

        # Top panel: Biome selection
        self.biome_selection_layout = QHBoxLayout()
        self.biome_label = QLabel("Select Biome:")
        self.biome_combo = QComboBox()
        self.biome_combo.addItems([biome.name for biome in self.data_manager.biomes])
        self.biome_combo.currentIndexChanged.connect(self.load_biome)
        self.new_biome_button = QPushButton("New Biome")
        self.new_biome_button.clicked.connect(self.create_new_biome)

        self.biome_selection_layout.addWidget(self.biome_label)
        self.biome_selection_layout.addWidget(self.biome_combo)
        self.biome_selection_layout.addWidget(self.new_biome_button)
        self.main_layout.addLayout(self.biome_selection_layout)

        # Biome details
        self.biome_name_label = QLabel("Biome Name:")
        self.biome_name_input = QLineEdit()
        self.biome_name_input.textChanged.connect(self.update_biome_name)
        self.main_layout.addWidget(self.biome_name_label)
        self.main_layout.addWidget(self.biome_name_input)

        # Main area split into two columns
        self.content_layout = QHBoxLayout()

        # Left column: Terrains/Props with checkboxes
        self.terrain_prop_list = QListWidget()
        self.terrain_prop_list.setSelectionMode(QListWidget.MultiSelection)
        self.terrain_prop_list.itemSelectionChanged.connect(self.update_relations_list)
        self.content_layout.addWidget(self.terrain_prop_list)

        # Right column: Relations and weights
        self.relations_list = QListWidget()
        self.content_layout.addWidget(self.relations_list)

        self.main_layout.addLayout(self.content_layout)
        self.setLayout(self.main_layout)

        self.current_biome = None
        self.load_terrain_props()

    def create_new_biome(self):
        biome_name, ok = QInputDialog.getText(self, "New Biome", "Enter biome name:")
        if ok and biome_name:
            biome = Biome(name=biome_name)
            self.data_manager.biomes.append(biome)
            self.biome_combo.addItem(biome_name)
            self.biome_combo.setCurrentText(biome_name)

    def load_biome(self, index):
        if index >= 0:
            biome_name = self.biome_combo.itemText(index)
            self.current_biome = next((b for b in self.data_manager.biomes if b.name == biome_name), None)
            if self.current_biome:
                self.biome_name_input.setText(self.current_biome.name)
                self.update_terrain_prop_selection()

    def update_biome_name(self, name):
        if self.current_biome:
            self.current_biome.name = name
            index = self.biome_combo.currentIndex()
            self.biome_combo.setItemText(index, name)

    def load_terrain_props(self):
        self.terrain_prop_list.clear()
        for tp in self.data_manager.terrain_props:
            item = QListWidgetItem(tp.name)
            item.setData(Qt.UserRole, tp)
            item.setCheckState(Qt.Unchecked)
            self.terrain_prop_list.addItem(item)

    def update_terrain_prop_selection(self):
        if self.current_biome:
            for index in range(self.terrain_prop_list.count()):
                item = self.terrain_prop_list.item(index)
                tp = item.data(Qt.UserRole)
                if tp in self.current_biome.terrain_props:
                    item.setCheckState(Qt.Checked)
                else:
                    item.setCheckState(Qt.Unchecked)

    def update_relations_list(self):
        if self.current_biome:
            # Update the list of selected terrain_props in the biome
            self.current_biome.terrain_props = [
                item.data(Qt.UserRole) for item in self.terrain_prop_list.selectedItems()
            ]
            # Display relations involving the selected terrain_props
            self.relations_list.clear()
            for relation in self.data_manager.relations:
                if (relation.terrain_prop in self.current_biome.terrain_props or
                    any(tp in self.current_biome.terrain_props for tp in relation.related_terrain_props)):
                    item = QListWidgetItem(f"{relation.terrain_prop.name} - {relation.direction}")
                    item.setData(Qt.UserRole, relation)
                    # Add input for weight (e.g., a spin box)
                    # For simplicity, let's display the weight as part of the item text
                    weight = self.current_biome.relation_weights.get(relation, 1.0)
                    item.setText(item.text() + f" (Weight: {weight})")
                    self.relations_list.addItem(item)
