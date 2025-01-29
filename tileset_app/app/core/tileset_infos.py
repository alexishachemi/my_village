"""
Module containing the tileset infos representation.
"""

from typing import TypedDict

from PySide6.QtGui import QPixmap


class TilesetInfo(TypedDict):
    """Tileset information."""

    path: str
    pixmap: QPixmap


TilesetInfos = dict[str, TilesetInfo]
