# data_manager.py
from app.models.tileset import Tileset, Tile
from app.models.terrain_prop import TerrainProp
from app.models.relation import Relation
from app.models.biome import Biome
from typing import List

class DataManager:
    def __init__(self):
        self.tilesets: List[Tileset] = []
        self.tiles: List[Tile] = []
        self.terrain_props: List[TerrainProp] = []
        self.relations: List[Relation] = []
        self.biomes: List[Biome] = []

    def add_tileset(self, tileset: Tileset):
        self.tilesets.append(tileset)
        self.tiles.extend(tileset.tiles)
