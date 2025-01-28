"""
Module containing the BaseItem model.
"""

from abc import ABC
from typing import TypeVar

from pydantic import BaseModel


class BaseItem(BaseModel, ABC):
    """Base model for a form-editable item."""

    name: str = "New Item"


ItemT = TypeVar("ItemT", bound=BaseItem)
