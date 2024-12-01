# models/biome.py
from __future__ import annotations
from dataclasses import dataclass, field
from typing import Dict, List
from models.terrain_prop import TerrainProp
from models.relation import Relation


@dataclass
class Biome:
    name: str
    terrain_props: List[TerrainProp] = field(default_factory=list)
    relation_weights: Dict[Relation, float] = field(default_factory=dict)

    def add_terrain_prop(self, terrain_prop: TerrainProp) -> None:
        if terrain_prop not in self.terrain_props:
            self.terrain_props.append(terrain_prop)

    def remove_terrain_prop(self, terrain_prop: TerrainProp) -> None:
        if terrain_prop in self.terrain_props:
            self.terrain_props.remove(terrain_prop)

    def set_relation_weight(self, relation: Relation, weight: float) -> None:
        self.relation_weights[relation] = weight
