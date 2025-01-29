"""
Module containing the world configuration model.
"""

from typing import NotRequired, TypedDict

from pydantic import BaseModel, PositiveInt

from app.items import (
    Asset,
    AssetDict,
    NamedAssetDict,
    Prop,
    PropDict,
    Room,
    RoomDict,
)


class RenderConfigDict(TypedDict):
    """Render configuration dictionary for configuration."""

    screen_size: tuple[PositiveInt, PositiveInt]
    tile_size_px: PositiveInt


class WorldConfigDict(TypedDict):
    """World configuration dictionary for configuration."""

    size: PositiveInt
    chunk_size: PositiveInt


class TilesetConfigDict(TypedDict):
    """Main configuration dictionary for configuration."""

    render: RenderConfigDict
    world: WorldConfigDict
    textures: dict[str, str]
    terrains: dict[str, AssetDict]
    props: PropDict
    rooms: RoomDict
    assets: NotRequired[NamedAssetDict]


class TilesetConfig(BaseModel):
    """Main configuration model."""

    render: RenderConfigDict
    world: WorldConfigDict
    textures: dict[str, str]
    terrains: dict[str, Asset]
    props: list[Prop]
    rooms: list[Room]

    def to_config_dict(self) -> TilesetConfigDict:
        """Convert the configuration to a dictionary."""
        props_dict: PropDict = {}
        for prop in self.props:
            props_dict = {**props_dict, **prop.to_config_dict()}
        rooms_dict: RoomDict = {}
        for room in self.rooms:
            rooms_dict = {**rooms_dict, **room.to_config_dict()}
        return {
            "render": self.render,
            "world": self.world,
            "textures": self.textures,
            "terrains": {
                key: value.to_config_dict()
                for key, value in self.terrains.items()
            },
            "props": props_dict,
            "rooms": rooms_dict,
        }

    @classmethod
    def from_config_dict(cls, data: TilesetConfigDict) -> "TilesetConfig":
        """Create a configuration from a dictionary."""
        asset_data = data.get("assets", {})
        return cls(
            render=data["render"],
            world=data["world"],
            textures=data["textures"],
            terrains={
                key: Asset.from_config_dict(asset_data, value)
                for key, value in data["terrains"].items()
            },
            props=[
                Prop.from_config_dict(asset_data, {key: data["props"][key]})
                for key in data["props"]
            ],
            rooms=[
                Room.from_config_dict({key: data["rooms"][key]})
                for key in data["rooms"]
            ],
        )
