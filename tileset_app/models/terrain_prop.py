# models/terrain_prop.py
from __future__ import annotations
from dataclasses import dataclass, field
from typing import List, Literal
from models.tileset import Tile

PropType = Literal['type1', 'type2', 'type3', 'type4']


@dataclass
class TerrainProp:
    name: str
    is_terrain: bool
    prop_type: PropType | None = None
    tiles: List[Tile] = field(default_factory=list)

    def add_tile(self, tile: Tile) -> None:
        self.tiles.append(tile)

    def remove_tile(self, tile: Tile) -> None:
        self.tiles.remove(tile)
