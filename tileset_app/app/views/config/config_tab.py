"""
Configuration tab for setting up application parameters.
"""

from pathlib import Path

from PySide6.QtCore import Signal
from PySide6.QtWidgets import (
    QFileDialog,
    QFormLayout,
    QGroupBox,
    QHBoxLayout,
    QListWidget,
    QPushButton,
    QSpinBox,
    QVBoxLayout,
    QWidget,
)

from app.core import RenderConfigDict, StarterTilesetConfigDict, WorldConfigDict


class ConfigTabSpinBoxes:
    """Holder for spin boxes in the configuration tab."""

    def __init__(self):
        self.screen_width = QSpinBox()
        self.screen_width.setRange(100, 4096)
        self.screen_width.setValue(400)

        self.screen_height = QSpinBox()
        self.screen_height.setRange(100, 4096)
        self.screen_height.setValue(300)

        self.tile_size_px = QSpinBox()
        self.tile_size_px.setRange(8, 256)
        self.tile_size_px.setValue(16)

        self.world_size = QSpinBox()
        self.world_size.setRange(20, 800)
        self.world_size.setValue(20)

        self.chunk_size = QSpinBox()
        self.chunk_size.setRange(0, 512)
        self.chunk_size.setValue(0)

    def get_render_dict(self) -> RenderConfigDict:
        """Collect values from spin boxes and emit render configuration"""
        return {
            "screen_size": (
                self.screen_width.value(),
                self.screen_height.value(),
            ),
            "tile_size_px": self.tile_size_px.value(),
        }

    def get_world_dict(self) -> WorldConfigDict:
        """Collect values from spin boxes and emit world configuration"""
        return {
            "size": self.world_size.value(),
            "chunk_size": self.chunk_size.value(),
        }


class ConfigTab(QWidget):
    """
    Configuration tab for setting up render, world, and texture parameters.
    """

    config_saved = Signal(StarterTilesetConfigDict)

    def __init__(self):
        super().__init__()
        self.textures: dict[str, str] = {}
        self.spin_boxes = ConfigTabSpinBoxes()
        self.init_ui()

    def init_ui(self):
        """Initialize the UI."""
        layout = QVBoxLayout()

        # Render Configuration
        render_group = QGroupBox("Render Configuration")
        render_layout = QFormLayout()

        render_layout.addRow("Screen Width:", self.spin_boxes.screen_width)
        render_layout.addRow("Screen Height:", self.spin_boxes.screen_height)
        render_layout.addRow("Tile Size (px):", self.spin_boxes.tile_size_px)
        render_group.setLayout(render_layout)

        # World Configuration
        world_group = QGroupBox("World Configuration")
        world_layout = QFormLayout()

        world_layout.addRow("World Size:", self.spin_boxes.world_size)
        world_layout.addRow("Chunk Size:", self.spin_boxes.chunk_size)
        world_group.setLayout(world_layout)

        # Textures Configuration
        textures_group = QGroupBox("Textures Configuration")
        textures_layout = QVBoxLayout()
        self.texture_list = QListWidget()

        btn_layout = QHBoxLayout()
        self.add_btn = QPushButton("Add Texture")
        self.add_btn.clicked.connect(self.add_texture)
        self.remove_btn = QPushButton("Remove Selected")
        self.remove_btn.clicked.connect(self.remove_texture)
        btn_layout.addWidget(self.add_btn)
        btn_layout.addWidget(self.remove_btn)

        textures_layout.addWidget(self.texture_list)
        textures_layout.addLayout(btn_layout)
        textures_group.setLayout(textures_layout)

        # Save Button
        self.save_btn = QPushButton("Save Configuration")
        self.save_btn.clicked.connect(self.validate_and_save)

        layout.addWidget(render_group)
        layout.addWidget(world_group)
        layout.addWidget(textures_group)
        layout.addWidget(self.save_btn)
        self.setLayout(layout)

    def add_texture(self):
        """Add a texture to the list."""
        path, _ = QFileDialog.getOpenFileName(
            self, "Select Texture File", "", "Image Files (*.png *.jpg *.bmp)"
        )
        if path:
            name = Path(path).stem
            self.textures[name] = path
            self.texture_list.addItem(f"{name}: {path}")

    def remove_texture(self):
        """Remove a texture from the list."""
        selected = self.texture_list.currentRow()
        if selected >= 0:
            item = self.texture_list.takeItem(selected)
            name = item.text().split(":")[0]
            del self.textures[name]

    def validate_and_save(self):
        """Collect values from spin boxes and emit configuration"""
        config: StarterTilesetConfigDict = {
            "render": self.spin_boxes.get_render_dict(),
            "world": self.spin_boxes.get_world_dict(),
            "textures": self.textures.copy(),
        }

        self.config_saved.emit(config)
