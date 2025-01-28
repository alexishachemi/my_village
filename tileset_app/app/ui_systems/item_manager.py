from typing import Callable, Type
from PySide6.QtWidgets import (
    QApplication, QMainWindow, QVBoxLayout, QHBoxLayout, QPushButton,
    QListWidget, QWidget, QLineEdit, QLabel, QComboBox, QFormLayout
)

class BaseItem:
    def __init__(self, name="New Item"):
        self.name = name

class BaseForm(QWidget):
    def __init__(self):
        super().__init__()
        self.form_layout = QFormLayout(self)

        # Form fields
        self.name_input = QLineEdit()

        # Add fields to the form
        self.form_layout.addRow(QLabel("Name:"), self.name_input)

        # Connect fields to update the item
        self.name_input.textChanged.connect(self.update_item_name)

        self.current_item = None
        self.item_name_changed_callback: Callable[[str], None] | None = None  # Callback to notify ItemManager

        self.layout = lambda: self.form_layout

    def update_form(self, item):
        """Populate the form fields with item details."""
        self.current_item = item
        self.name_input.setText(item.name)

    def clear_form(self):
        """Clear the form fields."""
        self.name_input.clear()
        self.current_item = None

    def update_item_name(self, name):
        """Update the item's name and notify ItemManager."""
        if self.current_item:
            self.current_item.name = name
            if self.item_name_changed_callback:
                self.item_name_changed_callback(name)

class ItemManager(QWidget):
    def __init__(self, form_widget: Type[BaseForm], cls: Type[BaseItem]):
        super().__init__()
        self.items: list[Type[BaseItem]] = []
        self.current_item: Type[BaseItem] | None = None
        self.cls: Type[BaseItem] = cls

        # Main Layout for the manager
        self.manager_layout = QHBoxLayout(self)

        # List Widget
        self.list_widget = QListWidget()
        self.list_widget.itemSelectionChanged.connect(self.update_form)
        self.manager_layout.addWidget(self.list_widget)

        # Side Layout
        side_layout = QVBoxLayout()

        # Form Widget (provided externally)
        self.form_widget = form_widget
        self.form_widget.setEnabled(False)
        self.form_widget.item_name_changed_callback = self.update_list_item_name
        side_layout.addWidget(self.form_widget)

        # Delete Button
        self.delete_button = QPushButton("Delete")
        self.delete_button.clicked.connect(self.delete_item)
        self.delete_button.setEnabled(False)
        side_layout.addWidget(self.delete_button)

        # Add Button
        self.add_button = QPushButton("+ Add Item")
        self.add_button.clicked.connect(self.add_item)
        side_layout.addWidget(self.add_button)

        self.manager_layout.addLayout(side_layout)

        self.layout = lambda: self.manager_layout

    def add_item(self):
        """Add a new item and set it as the current selection."""
        new_item = self.cls()
        self.items.append(new_item)
        self.list_widget.addItem(new_item.name)
        self.list_widget.setCurrentRow(len(self.items) - 1)
        self.update_form()

    def delete_item(self):
        """Delete the currently selected item."""
        current_row = self.list_widget.currentRow()
        if current_row >= 0:
            self.items.pop(current_row)
            self.list_widget.takeItem(current_row)
            self.update_form()

    def update_form(self):
        """Update the form with the selected item's details."""
        current_row = self.list_widget.currentRow()
        if current_row >= 0:
            self.current_item = self.items[current_row]
            self.form_widget.setEnabled(True)
            self.delete_button.setEnabled(True)
            self.form_widget.update_form(self.current_item)
        else:
            self.current_item = None
            self.form_widget.setEnabled(False)
            self.delete_button.setEnabled(False)
            self.form_widget.clear_form()

    def update_list_item_name(self, name):
        """Update the name of the current item in the list."""
        current_row = self.list_widget.currentRow()
        if current_row >= 0:
            self.list_widget.item(current_row).setText(name)

    @property
    def list(self):
        """Access the list widget."""
        return self.list_widget

    @property
    def add_button_widget(self):
        """Access the add button."""
        return self.add_button

    @property
    def delete_button_widget(self):
        """Access the delete button."""
        return self.delete_button