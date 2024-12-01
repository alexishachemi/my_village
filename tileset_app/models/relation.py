# models/relation.py
from __future__ import annotations
from dataclasses import dataclass, field
from typing import List, Literal
from models.terrain_prop import TerrainProp

Direction = Literal['top', 'right', 'bottom', 'left', 'all']


@dataclass
class Relation:
    terrain_prop: TerrainProp
    direction: Direction
    related_terrain_props: List[TerrainProp] = field(default_factory=list)

    def add_related(self, terrain_prop: TerrainProp) -> None:
        if terrain_prop not in self.related_terrain_props:
            self.related_terrain_props.append(terrain_prop)

    def remove_related(self, terrain_prop: TerrainProp) -> None:
        if terrain_prop in self.related_terrain_props:
            self.related_terrain_props.remove(terrain_prop)
