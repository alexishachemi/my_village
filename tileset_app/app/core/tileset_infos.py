from typing import TypedDict

from PySide6.QtGui import QPixmap


class TilesetInfo(TypedDict):
    path: str
    pixmap: QPixmap


TilesetInfos = dict[str, TilesetInfo]
