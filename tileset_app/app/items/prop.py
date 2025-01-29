"""
Module containing Prop models.
"""

from typing import NotRequired, TypedDict

from pydantic import BaseModel

from .asset import Asset, AssetDictValues, NamedAssetDict
from .base_item import BaseItem

Offset = tuple[int, int]


class ChildPropDict(TypedDict):
    """Child prop dictionary for configuration."""

    offset: Offset
    assets: AssetDictValues


class ChildProp(BaseModel):
    """Child prop for multi-tile props."""

    offset: Offset
    asset: Asset

    def to_config_dict(self, unnamed_assets: bool = True) -> ChildPropDict:
        """Convert the asset to a configuration dictionary."""
        return {
            "offset": self.offset,
            **self.asset.to_config_dict(unnamed_assets),
        }

    @classmethod
    def from_config_dict(
        cls, assets: NamedAssetDict, data: ChildPropDict
    ) -> "ChildProp":
        """Create an asset from a configuration dictionary."""
        return cls(
            offset=data["offset"],
            asset=Asset.from_config_dict(assets, {"assets": data["assets"]}),
        )


class SubPropDict(TypedDict):
    """Sub-prop dictionary for configuration."""

    assets: AssetDictValues
    children: NotRequired[list[ChildPropDict]]


PropDict = dict[str, SubPropDict]


class Prop(BaseItem):
    """Prop for the form system."""

    asset: Asset
    children: list[ChildProp] | None = None

    def to_config_dict(self, unnamed_assets: bool = True) -> PropDict:
        """Convert the asset to a configuration dictionary."""
        sub_dict: SubPropDict = {**self.asset.to_config_dict(unnamed_assets)}
        if self.children:
            sub_dict["children"] = [
                child.to_config_dict(unnamed_assets) for child in self.children
            ]
        return {self.name: sub_dict}

    @classmethod
    def from_config_dict(cls, assets: NamedAssetDict, data: PropDict) -> "Prop":
        """Create an asset from a configuration dictionary."""
        name, sub_data = list(data.items())[0]
        children = None
        if "children" in sub_data:
            children = [
                ChildProp.from_config_dict(assets, child)
                for child in sub_data["children"]
            ]
        return cls(
            name=name,
            asset=Asset.from_config_dict(
                assets, {"assets": sub_data["assets"]}
            ),
            children=children,
        )
