# import sys
# from PySide6.QtWidgets import QApplication, QWidget, QHBoxLayout
# from app.ui_systems import LibrarySelector
# from PySide6.QtWidgets import QPushButton, QVBoxLayout



# app = QApplication(sys.argv)
# window = QWidget()
# layout = QHBoxLayout(window)

# selector = LibrarySelector()

# items = [f"Item {i}" for i in range(1,4)]
# selector.populate_library(items)

# layout.addWidget(selector.library)
# layout.addWidget(selector.selected)

# reset_button = QPushButton("Reset Selected")
# reset_button.clicked.connect(selector.reset_selected)

# print_selected_items = QPushButton("Print Selected Items")
# print_selected_items.clicked.connect(lambda: print(selector.selected_items))

# button_layout = QVBoxLayout()
# button_layout.addWidget(reset_button)
# button_layout.addWidget(print_selected_items)
# layout.addLayout(button_layout)

# window.show()

# sys.exit(app.exec())
from PySide6.QtWidgets import (
    QApplication, QMainWindow, QVBoxLayout, QHBoxLayout, QPushButton,
    QListWidget, QWidget, QLineEdit, QLabel, QComboBox, QFormLayout
)
from PySide6.QtCore import Qt
from app.ui_systems import ItemManager, BaseItem, BaseForm

class Item(BaseItem):
    def __init__(self, name="New Item", category="Category 1"):
        super().__init__(name)
        self.category = category

class ItemForm(BaseForm):
    def __init__(self):
        super().__init__()
        self.category_input = QComboBox()
        self.category_input.addItems(["Category 1", "Category 2", "Category 3"])

        # Add fields to the form
        self.form_layout.addRow(QLabel("Category:"), self.category_input)

        # Connect fields to update the item
        self.category_input.currentTextChanged.connect(self.update_item_category)

    def update_form(self, item):
        """Populate the form fields with item details."""
        super().update_form(item)
        self.category_input.setCurrentText(item.category)

    def clear_form(self):
        """Clear the form fields."""
        super().clear_form()
        self.category_input.setCurrentIndex(0)

    def update_item_category(self, category):
        """Update the item's category."""
        if self.current_item:
            self.current_item.category = category

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("Reusable Form and List")

        # Main Layout
        central_layout = QVBoxLayout()

        # Create a form widget and item manager
        self.form_widget = ItemForm()
        self.item_manager = ItemManager(self.form_widget, Item)

        # Add the item manager to the main layout
        central_layout.addWidget(self.item_manager)

        # Set the central widget
        central_widget = QWidget()
        central_widget.setLayout(central_layout)
        self.setCentralWidget(central_widget)

if __name__ == "__main__":
    app = QApplication([])

    window = MainWindow()
    window.show()

    app.exec()
