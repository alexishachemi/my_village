"""
Main Module.
"""

import sys
from pathlib import Path

from PySide6.QtCore import Qt
from PySide6.QtGui import QPixmap
from PySide6.QtWidgets import QApplication

from app.core import TilesetInfos
from app.views import MainWindow


def main():
    """Main function."""
    app = QApplication(sys.argv)
    if len(sys.argv) < 3:
        print("Usage: python main.py <tile_size> <tileset1> [<tileset2> ...]")
        sys.exit(1)

    tile_size = int(sys.argv[1])
    paths = sys.argv[2:]
    if not paths:
        print("Error: No tileset files provided.")
        sys.exit(1)

    tileset_infos: TilesetInfos = {}
    for p in paths:
        path_obj = Path(p)
        short_name = path_obj.stem
        if path_obj.exists():
            pixmap = QPixmap(str(path_obj))
        else:
            # fallback
            pixmap = QPixmap(64, 64)
            pixmap.fill(Qt.GlobalColor.red)
        tileset_infos[short_name] = {"path": str(path_obj), "pixmap": pixmap}

    w = MainWindow(tile_size, tileset_infos)
    w.show()
    sys.exit(app.exec())


if __name__ == "__main__":
    main()
