"""
Module containing the Assets models.
"""

import uuid
from abc import ABC, abstractmethod
from enum import Enum
from typing import TypedDict

from pydantic import BaseModel, PositiveInt

from .base_item import BaseItem

TextureCoords = tuple[PositiveInt, PositiveInt, PositiveInt, PositiveInt]

UnnamedMonoAsset = tuple[
    str, PositiveInt, PositiveInt, PositiveInt, PositiveInt
]


def coords_to_str(coords: TextureCoords) -> str:
    """Convert texture coordinates to a string."""
    return f"{coords[0]}, {coords[1]}, {coords[2]}, {coords[3]}"


class DirectionEnum(str, Enum):
    """Directional enum for assets."""

    UP = "up"
    DOWN = "down"
    LEFT = "left"
    RIGHT = "right"


class UnnamedDirectionalAsset(TypedDict, total=False):
    """Unnamed directional asset model for tileset splitting (dict version)."""

    up: UnnamedMonoAsset
    down: UnnamedMonoAsset
    left: UnnamedMonoAsset
    right: UnnamedMonoAsset


UnnamedAsset = UnnamedMonoAsset | UnnamedDirectionalAsset


class BaseAsset(BaseModel, ABC):
    """Base asset model for tileset splitting."""

    @property
    @abstractmethod
    def unnamed(self) -> UnnamedAsset:
        """Convert the asset to an unnamed asset."""

    @classmethod
    @abstractmethod
    def from_unnamed(cls, data: UnnamedAsset) -> "BaseAsset":
        """Create an asset from an unnamed asset."""


class MonoAsset(BaseAsset):
    """Asset model for tileset splitting."""

    texture: str
    coords: TextureCoords

    @property
    def unnamed(self) -> UnnamedMonoAsset:
        """Convert the asset to an unnamed tuple."""
        return (self.texture, *self.coords)

    @classmethod
    def from_unnamed(cls, data: UnnamedAsset) -> "MonoAsset":
        """Create an asset from an unnamed tuple."""
        assert isinstance(data, tuple)
        return MonoAsset(texture=data[0], coords=data[1:])


class DirectionnalAsset(BaseAsset):
    """Directional Asset model for tileset splitting, with directions."""

    up: MonoAsset | None
    down: MonoAsset | None
    left: MonoAsset | None
    right: MonoAsset | None

    @property
    def unnamed(self) -> UnnamedDirectionalAsset:
        """Convert the asset to an unnamed dictionary."""
        returned: UnnamedDirectionalAsset = {}
        if self.up:
            returned["up"] = self.up.unnamed
        if self.down:
            returned["down"] = self.down.unnamed
        if self.left:
            returned["left"] = self.left.unnamed
        if self.right:
            returned["right"] = self.right.unnamed
        return returned

    @classmethod
    def from_unnamed(cls, data: UnnamedAsset) -> "DirectionnalAsset":
        """Create an asset from an unnamed dictionary."""
        assert isinstance(data, dict)
        return DirectionnalAsset(
            up=MonoAsset.from_unnamed(data["up"]) if "up" in data else None,
            down=(
                MonoAsset.from_unnamed(data["down"]) if "down" in data else None
            ),
            left=(
                MonoAsset.from_unnamed(data["left"]) if "left" in data else None
            ),
            right=(
                MonoAsset.from_unnamed(data["right"])
                if "right" in data
                else None
            ),
        )


AssetType = MonoAsset | DirectionnalAsset

AssetDictValues = UnnamedAsset | str


class AssetDict(TypedDict):
    """Asset dictionary for configuration."""

    assets: AssetDictValues


NamedAssetDict = dict[str, UnnamedAsset]


class Asset(BaseItem):
    """Asset wrapper for the form system."""

    asset: AssetType

    def to_config_dict(self, unnamed: bool = True) -> AssetDict:
        """Convert the asset to a configuration dictionary."""
        return {"assets": self.asset.unnamed if unnamed else self.name}

    @classmethod
    def from_config_dict(
        cls, assets: NamedAssetDict, data: AssetDict
    ) -> "Asset":
        """Create an asset from a configuration dictionary."""
        _asset: AssetType
        _data: UnnamedAsset
        _name: str
        if isinstance(data["assets"], str):
            _data = assets[data["assets"]]
            _name = data["assets"]
        else:
            _data = data["assets"]
            _name = str(uuid.uuid4())

        if isinstance(_data, tuple):
            _asset = MonoAsset.from_unnamed(_data)
        else:
            _asset = DirectionnalAsset.from_unnamed(_data)

        return Asset(name=_name, asset=_asset)
