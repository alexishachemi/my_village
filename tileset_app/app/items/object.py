"""
Module containing the room object models and constraints.
"""

from abc import ABC, abstractmethod
from enum import Enum
from typing import Literal, NotRequired, TypedDict, cast

from pydantic import BaseModel, Field, PositiveFloat, PositiveInt

from .asset import DirectionEnum
from .base_item import BaseItem


class ConstraintEnum(str, Enum):
    """Enum for constraints."""

    ADJACENT_TO_WALL = "adjacent_to_wall"
    ADJACENT_TO_PROP = "adjacent_to_prop"
    IN_CORNER = "in_corner"
    ON_TOP_OF_PROP = "on_top_of_prop"
    ON_GROUND = "on_ground"
    HAS_ORIENT = "has_orient"


SimpleConstraintType = Literal[
    ConstraintEnum.ADJACENT_TO_WALL,
    ConstraintEnum.IN_CORNER,
    ConstraintEnum.ON_GROUND,
]

PropConstraintType = Literal[
    ConstraintEnum.ADJACENT_TO_PROP, ConstraintEnum.ON_TOP_OF_PROP
]

OrientConstraintType = Literal[ConstraintEnum.HAS_ORIENT]

# Room Configuration

ConstraintTuple = (
    tuple[bool, SimpleConstraintType]
    | tuple[tuple[bool, PropConstraintType], tuple[str, ...]]
    | tuple[tuple[bool, OrientConstraintType], tuple[DirectionEnum, ...]]
)


def to_real_config_tuple(data: ConstraintTuple) -> tuple:
    """Convert a constraint tuple to the true shape of the constraint tuple."""
    if isinstance(data[0], bool):
        # Simple constraint
        return data
    # Complex constraints
    return (*data[0], *data[1])


def from_real_config_tuple(data: tuple) -> ConstraintTuple:
    """Convert a constraint tuple to the true shape of the constraint tuple."""
    assert data[0] is bool
    if len(data) == 2:
        assert data[1] in {
            ConstraintEnum.ADJACENT_TO_WALL,
            ConstraintEnum.IN_CORNER,
            ConstraintEnum.ON_GROUND,
        }
        return data
    assert data[1] in {
        ConstraintEnum.ADJACENT_TO_PROP,
        ConstraintEnum.ON_TOP_OF_PROP,
        ConstraintEnum.HAS_ORIENT,
    }
    # ensure all following elements are strings
    assert all(isinstance(x, str) for x in data[2:]) or all(
        isinstance(DirectionEnum(x), DirectionEnum) for x in data[2:]
    )
    return (data[:2], tuple(data[2:]))


class BaseConstraint(BaseModel, ABC):
    """Base model for constraints."""

    activate: bool
    constraint: ConstraintEnum

    @abstractmethod
    def to_config_tup(self) -> ConstraintTuple:
        """Convert the constraint to a configuration tuple."""

    @classmethod
    @abstractmethod
    def from_config_tup(cls, data: ConstraintTuple) -> "BaseConstraint":
        """Create a constraint from a configuration tuple."""


class SimpleConstraint(BaseConstraint):
    """Simple constraint for most constraints"""

    constraint: SimpleConstraintType  # type: ignore

    def to_config_tup(self) -> ConstraintTuple:
        """Convert the constraint to a configuration tuple."""
        return (self.activate, self.constraint)

    @classmethod
    def from_config_tup(cls, data: ConstraintTuple) -> "SimpleConstraint":
        """Create a constraint from a configuration tuple."""
        assert isinstance(data[0], bool)
        assert data[1] in {
            ConstraintEnum.ADJACENT_TO_WALL,
            ConstraintEnum.IN_CORNER,
            ConstraintEnum.ON_GROUND,
        }
        return cls(
            activate=data[0], constraint=cast(SimpleConstraintType, data[1])
        )


class PropConstraint(BaseConstraint):
    """Model for the ADJACENT and ON TOP OF PROP constraints"""

    constraint: PropConstraintType  # type: ignore
    props: set[str] = Field(default_factory=set)

    def to_config_tup(self) -> ConstraintTuple:
        """Convert the constraint to a configuration tuple."""
        return ((self.activate, self.constraint), tuple(self.props))

    @classmethod
    def from_config_tup(cls, data: ConstraintTuple) -> "PropConstraint":
        """Create a constraint from a configuration tuple."""
        assert isinstance(data[0], tuple)
        assert isinstance(data[0][0], bool)
        assert data[0][1] in {
            ConstraintEnum.ADJACENT_TO_PROP,
            ConstraintEnum.ON_TOP_OF_PROP,
        }
        assert isinstance(data[1], tuple)
        assert all(isinstance(x, str) for x in data[1])
        return cls(
            activate=data[0][0],
            constraint=cast(PropConstraintType, data[0][1]),
            props=cast(set[str], data[1]),
        )


class OrientConstraint(BaseConstraint):
    """Model for the HAS_ORIENT constraint"""

    constraint: OrientConstraintType  # type: ignore
    directions: set[DirectionEnum] = Field(default_factory=set)

    def to_config_tup(self) -> ConstraintTuple:
        """Convert the constraint to a configuration tuple."""
        return ((self.activate, self.constraint), tuple(self.directions))

    @classmethod
    def from_config_tup(cls, data: ConstraintTuple) -> "OrientConstraint":
        """Create a constraint from a configuration tuple."""
        assert isinstance(data[0], tuple)
        assert isinstance(data[0][0], bool)
        assert data[0][1] == ConstraintEnum.HAS_ORIENT
        assert isinstance(data[1], tuple)
        assert all(isinstance(DirectionEnum(x), DirectionEnum) for x in data[1])
        return cls(
            activate=data[0][0],
            constraint=cast(OrientConstraintType, data[0][1]),
            directions=cast(set[DirectionEnum], data[1]),
        )


ConstraintType = SimpleConstraint | PropConstraint | OrientConstraint

AmountType = PositiveInt | tuple[PositiveInt, PositiveInt]


class RoomObjectDict(TypedDict):
    """Sub-room object dictionary for configuration."""

    prop: str | list[str]
    chance: PositiveFloat
    amount: AmountType
    constraints: NotRequired[list[tuple]]


class RoomObject(BaseItem):
    """Model for room objects."""

    prop: str | list[str]
    chance: PositiveFloat
    amount: AmountType
    constraints: list[ConstraintType] | None = None

    def to_config_dict(self) -> RoomObjectDict:
        """Convert the room object to a configuration dictionary."""
        returned: RoomObjectDict = {
            "prop": self.prop,
            "chance": self.chance,
            "amount": self.amount,
        }
        if self.constraints:
            returned["constraints"] = [
                to_real_config_tuple(constraint.to_config_tup())
                for constraint in self.constraints
            ]
        return returned

    @classmethod
    def from_config_dict(cls, data: RoomObjectDict) -> "RoomObject":
        """Create a room object from a configuration dictionary."""
        constraints = None
        if "constraints" in data:
            constraints = [
                (
                    SimpleConstraint.from_config_tup(
                        from_real_config_tuple(constraint)
                    )
                    if constraint[1]
                    in {
                        ConstraintEnum.ADJACENT_TO_WALL,
                        ConstraintEnum.IN_CORNER,
                        ConstraintEnum.ON_GROUND,
                    }
                    else (
                        PropConstraint.from_config_tup(
                            from_real_config_tuple(constraint)
                        )
                        if constraint[1]
                        in {
                            ConstraintEnum.ADJACENT_TO_PROP,
                            ConstraintEnum.ON_TOP_OF_PROP,
                        }
                        else OrientConstraint.from_config_tup(
                            from_real_config_tuple(constraint)
                        )
                    )
                )
                for constraint in data["constraints"]
            ]
        return cls(
            prop=data["prop"],
            chance=data["chance"],
            amount=data["amount"],
            constraints=constraints,
        )
