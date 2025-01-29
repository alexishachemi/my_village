"""
Module for the BaseForm class.
"""

from typing import Callable, Generic, TypeVar

from PySide6.QtWidgets import QFormLayout, QLabel, QLineEdit, QWidget

from app.items import ItemT


class BaseForm(QWidget, Generic[ItemT]):
    """Base form for editing an item."""

    def __init__(self) -> None:
        super().__init__()

        self.form_layout: QFormLayout = QFormLayout(self)
        self.name_input: QLineEdit = QLineEdit()
        self.current_item: ItemT | None = None
        self.item_name_changed_callback: Callable[[str], None] | None = None

        self.form_layout.addRow(QLabel("Name:"), self.name_input)
        self.name_input.textChanged.connect(self.update_item_name)
        self.layout = lambda: self.form_layout

    def update_form(self, item: ItemT) -> None:
        """Populate the form fields with item details."""
        self.current_item = item
        self.name_input.setText(item.name)

    def clear_form(self) -> None:
        """Clear the form fields."""
        self.name_input.clear()
        self.current_item = None

    def update_item_name(self, name: str) -> None:
        """Update the item's name and notify ItemManager."""
        if not self.current_item:
            return

        self.current_item.name = name

        if self.item_name_changed_callback:
            self.item_name_changed_callback(name)


FormT = TypeVar("FormT", bound=BaseForm)
