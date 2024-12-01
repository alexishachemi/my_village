# models/tileset.py
from __future__ import annotations
from dataclasses import dataclass, field
from pathlib import Path
from typing import List, Tuple
from PySide6.QtGui import QPixmap

@dataclass
class Tile:
    tileset: Tileset
    coord: Tuple[int, int]  # (row, col) indices in the tileset grid

    def get_pixmap(self) -> QPixmap:
        tile_size = self.tileset.tile_size
        x = self.coord[1] * tile_size
        y = self.coord[0] * tile_size
        rect = (x, y, tile_size, tile_size)
        return self.tileset.image.copy(*rect)

@dataclass
class Tileset:
    name: str
    path: Path
    tile_size: int  # Pixel size of a tile
    image: QPixmap = None
    tiles: List[Tile] = field(default_factory=list)

    def load_tiles(self) -> None:
        self.image = QPixmap(str(self.path))
        if self.image.isNull():
            print(f"Failed to load image from {self.path}")
            return

        width = self.image.width()
        height = self.image.height()
        cols = width // self.tile_size
        rows = height // self.tile_size

        for row in range(rows):
            for col in range(cols):
                tile = Tile(tileset=self, coord=(row, col))
                self.tiles.append(tile)
