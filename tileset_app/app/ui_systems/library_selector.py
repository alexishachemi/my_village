from PySide6.QtWidgets import QListWidget, QListWidgetItem, QAbstractItemView
from PySide6.QtCore import Qt


class LibrarySelector:
    """Handle config for a dual list system, without any layout logic"""

    def __init__(self):
        self.library_list = QListWidget()
        self.selected_list = QListWidget()
        self.items = []

        for l in [self.library_list, self.selected_list]:
            l.setSelectionMode(QAbstractItemView.SelectionMode.SingleSelection)
            l.setDragEnabled(True)
            l.setAcceptDrops(True)
            l.setDropIndicatorShown(True)
            l.setDragDropMode(QAbstractItemView.DragDropMode.DragDrop)
            l.setDefaultDropAction(Qt.DropAction.MoveAction)

    @property
    def library(self):
        return self.library_list

    @property
    def selected(self):
        return self.selected_list

    @property
    def selected_items(self):
        return [self.selected_list.item(i).text() for i in range(self.selected_list.count())]

    def populate_library(self, items):
        self.items = items
        for i in items:
            item = QListWidgetItem(i)
            self.library_list.addItem(item)

    def reset_selected(self):
        self.selected_list.clear()
        self.library_list.clear()
        self.populate_library(self.items)

    