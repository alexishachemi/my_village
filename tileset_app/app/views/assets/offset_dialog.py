"""
Module containg a dialog for setting offsets for assets.
"""

from PySide6.QtWidgets import (
    QDialog,
    QDialogButtonBox,
    QFormLayout,
    QSpinBox,
)

class OffsetDialog(QDialog):
    """Dialog for setting offsets for assets."""
    def __init__(
        self,
        left: int,
        top: int,
        tile_size: int,
        max_x: int,
        max_y: int,
        parent=None,
    ):
        super().__init__(parent)
        self.tile_size = tile_size
        self.max_x = max_x
        self.max_y = max_y
        self.setWindowTitle("Set Offsets")

        layout = QFormLayout(self)

        self.left_spin = self.create_spinbox(left, 0, max_x - 1)
        self.top_spin = self.create_spinbox(top, 0, max_y - 1)
        self.width_spin = self.create_spinbox(tile_size, 1, max_x)
        self.height_spin = self.create_spinbox(tile_size, 1, max_y)

        layout.addRow("Left:", self.left_spin)
        layout.addRow("Top:", self.top_spin)
        layout.addRow("Width:", self.width_spin)
        layout.addRow("Height:", self.height_spin)

        self.button_box = QDialogButtonBox(
            QDialogButtonBox.StandardButton.Ok
            | QDialogButtonBox.StandardButton.Cancel
        )
        self.button_box.accepted.connect(self.validate_and_accept)
        self.button_box.rejected.connect(self.reject)
        layout.addRow(self.button_box)

        self.setLayout(layout)

    def create_spinbox(self, value: int, min: int, max: int) -> QSpinBox:
        """Create a spinbox with a range and value."""
        spinbox = QSpinBox()
        spinbox.setRange(min, max)
        spinbox.setValue(value)
        spinbox.valueChanged.connect(self.validate_offsets)
        return spinbox

    def validate_offsets(self) -> bool:
        """Validate the offsets."""
        valid = (
            self.left_spin.value() + self.width_spin.value()
        ) <= self.max_x and (
            self.top_spin.value() + self.height_spin.value()
        ) <= self.max_y
        self.button_box.button(QDialogButtonBox.StandardButton.Ok).setEnabled(
            valid
        )
        return valid

    def validate_and_accept(self):
        """Validate the offsets and accept the dialog."""
        if self.validate_offsets():
            self.accept()

    def get_offsets(self) -> tuple[int, int, int, int]:
        """Get the offsets from the dialog."""
        return (
            self.left_spin.value(),
            self.top_spin.value(),
            self.width_spin.value(),
            self.height_spin.value(),
        )
