"""
Module containing the room models and constraints.
"""

from enum import Enum
from typing import NotRequired, TypedDict

from pydantic import PositiveInt

from .base_item import BaseItem
from .object import RoomObject, RoomObjectDict


class GlobalConstraintEnum(str, Enum):
    """Enum for global constraints."""

    ALL_CELLS_CONNECTED = "all_cells_connected"


GlobalConstraint = tuple[bool, GlobalConstraintEnum]


class SubRoomDict(TypedDict):
    """Room dictionary for configuration."""

    terrain: str
    layers: PositiveInt
    constraints: NotRequired[list[GlobalConstraint]]
    objects: NotRequired[list[RoomObjectDict]]


RoomDict = dict[str, SubRoomDict]


class Room(BaseItem):
    """Room model for the form system."""

    terrain: str
    layers: int
    constraints: list[GlobalConstraint]
    objects: list[RoomObject]

    def to_config_dict(self) -> RoomDict:
        """Convert the room to a configuration dictionary."""
        return {
            self.name: {
                "terrain": self.terrain,
                "layers": self.layers,
                "constraints": self.constraints,
                "objects": [obj.to_config_dict() for obj in self.objects],
            }
        }

    @classmethod
    def from_config_dict(cls, data: RoomDict) -> "Room":
        """Create a room from a configuration dictionary."""
        sub_data = list(data.values())[0]
        return cls(
            name=list(data.keys())[0],
            terrain=sub_data["terrain"],
            layers=sub_data["layers"],
            constraints=sub_data.get("constraints", []),
            objects=[
                RoomObject.from_config_dict(obj)
                for obj in sub_data.get("objects", [])
            ],
        )
