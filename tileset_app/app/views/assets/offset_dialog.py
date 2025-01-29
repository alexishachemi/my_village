"""
Module containg a dialog for setting offsets for assets.
"""

from PySide6.QtWidgets import QDialog, QDialogButtonBox, QFormLayout, QSpinBox


class OffsetDialog(QDialog):
    """Dialog for setting offsets for assets."""

    def __init__(
        self,
        rect_start: tuple[int, int],
        tile_size: int,
        rect_max: tuple[int, int],
        parent=None,
    ):
        super().__init__(parent)
        self.rect_max = rect_max
        self.setWindowTitle("Set Offsets")

        self.left_spin = self.create_spinbox(
            rect_start[0], 0, self.rect_max[0] - 1
        )
        self.top_spin = self.create_spinbox(
            rect_start[1], 0, self.rect_max[1] - 1
        )
        self.width_spin = self.create_spinbox(tile_size, 1, self.rect_max[0])
        self.height_spin = self.create_spinbox(tile_size, 1, self.rect_max[1])

        layout = QFormLayout(self)
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

    def create_spinbox(
        self, value: int, min_val: int, max_val: int
    ) -> QSpinBox:
        """Create a spinbox with a range and value."""
        spinbox = QSpinBox()
        spinbox.setRange(min_val, max_val)
        spinbox.setValue(value)
        spinbox.valueChanged.connect(self.validate_offsets)
        return spinbox

    def validate_offsets(self) -> bool:
        """Validate the offsets."""
        valid = (
            self.left_spin.value() + self.width_spin.value()
        ) <= self.rect_max[0] and (
            self.top_spin.value() + self.height_spin.value()
        ) <= self.rect_max[
            1
        ]
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
