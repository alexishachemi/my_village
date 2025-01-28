from PySide6.QtWidgets import QWidget, QHBoxLayout
from typing import Type

from app.ui_systems.library_selector import LibrarySelector
from app.ui_systems.item_manager import ItemManager

class LibrarySelItemManager(QWidget):
    def __init__(self, library_selector: LibrarySelector, item_manager: ItemManager):
        super().__init__()
        self.library_selector = library_selector
        self.item_manager = item_manager

        layout = QHBoxLayout(self)
        layout.addWidget(self.library_selector.library)
        layout.addWidget(self.library_selector.selected)
        layout.addWidget(self.item_manager)

        self.setLayout(layout)

    def populate_library(self, items):
        self.library_selector.populate_library(items)

    def reset(self):
        self.library_selector.reset_selected()

    @property
    def selected_items(self):
        return self.library_selector.selected_items
