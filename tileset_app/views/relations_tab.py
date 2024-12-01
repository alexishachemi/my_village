# views/relations_tab.py
from PySide6.QtWidgets import (
    QWidget, QHBoxLayout, QVBoxLayout, QListWidget, QLabel, QComboBox, QPushButton
)
from PySide6.QtCore import Qt
from models.relation import Relation
from models.terrain_prop import TerrainProp

class RelationsTab(QWidget):
    def __init__(self, data_manager):
        super().__init__()
        self.data_manager = data_manager
        self.init_ui()

    def init_ui(self):
        self.main_layout = QHBoxLayout()

        # Left column: List of terrains & props
        self.terrain_prop_list = QListWidget()
        self.terrain_prop_list.itemClicked.connect(self.display_terrain_prop_info)
        self.main_layout.addWidget(self.terrain_prop_list)

        # Right column: Relation details
        self.right_layout = QVBoxLayout()

        self.id_card_label = QLabel("ID Card:")
        self.right_layout.addWidget(self.id_card_label)

        self.direction_label = QLabel("Select Direction:")
        self.direction_combo = QComboBox()
        self.direction_combo.addItems(["top", "right", "bottom", "left", "all"])
        self.direction_combo.currentTextChanged.connect(self.update_relations_area)
        self.right_layout.addWidget(self.direction_label)
        self.right_layout.addWidget(self.direction_combo)

        self.relations_area_label = QLabel("Relations:")
        self.right_layout.addWidget(self.relations_area_label)

        self.relations_area = QListWidget()
        self.relations_area.setAcceptDrops(True)
        self.relations_area.setDragEnabled(True)
        self.relations_area.setDragDropMode(QListWidget.InternalMove)
        self.right_layout.addWidget(self.relations_area)

        self.main_layout.addLayout(self.right_layout)
        self.setLayout(self.main_layout)
        # Add a button to save the relation
        self.save_button = QPushButton("Save Relation")
        self.save_button.clicked.connect(self.save_relation)
        self.right_layout.addWidget(self.save_button)

    def display_terrain_prop_info(self, item):
        # Display selected terrain/prop's id card
        pass

    def update_relations_area(self, direction):
        # Update relations area based on selected direction
        pass

    def save_relation(self):
        if self.current_terrain_prop and self.current_direction:
            related_tps = [item.data(Qt.UserRole) for item in self.relations_area.findItems("*", Qt.MatchWildcard)]
            relation = Relation(
                terrain_prop=self.current_terrain_prop,
                direction=self.current_direction,
                related_terrain_props=related_tps
            )
            self.data_manager.relations.append(relation)
            print(f"Saved relation for {self.current_terrain_prop.name} in {self.current_direction} direction.")