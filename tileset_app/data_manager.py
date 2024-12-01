# data_manager.py
from models.tileset import Tileset
from models.terrain_prop import TerrainProp
from models.relation import Relation
from models.biome import Biome
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
