"""
Module containing a tab to name fastly tiles.
"""

from pathlib import Path
from typing import TypedDict

from PySide6.QtCore import Qt
from PySide6.QtGui import QImage, QPixmap
from PySide6.QtWidgets import (
    QHBoxLayout,
    QLabel,
    QLineEdit,
    QPushButton,
    QScrollArea,
    QVBoxLayout,
    QWidget,
)

from app.core import TilesetInfos
from app.items import UnnamedMonoAsset


class NamedTilesTabItem(TypedDict):
    """Item in the NamedTilesTab."""

    widget: QWidget
    line_edit: QLineEdit
    discarded: bool
    pix_label: QLabel


class NamedTilesTab(QWidget):
    """Tab for naming tiles."""

    def __init__(
        self, tileset_infos: TilesetInfos, label_for_button: str, parent=None
    ):
        super().__init__(parent)

        self.label_for_button = label_for_button
        self.tileset_infos = tileset_infos

        self._items: dict[UnnamedMonoAsset, NamedTilesTabItem] = {}

        # Set up layout
        self.main_layout = QVBoxLayout(self)
        scroll = QScrollArea(self)
        container = QWidget()
        self._list_layout = QVBoxLayout(container)

        container.setLayout(self._list_layout)
        scroll.setWidget(container)
        scroll.setWidgetResizable(True)

        self.main_layout.addWidget(scroll)
        self.setLayout(self.main_layout)

    def add_or_update_items(self, new_items: list[UnnamedMonoAsset]):
        """Add or update items in this tab."""
        for key in new_items:
            if key in self._items:
                continue  # do not overwrite existing names

            # This is a new entry => create row
            row_widget = QWidget()
            row_layout = QHBoxLayout(row_widget)
            row_widget.setLayout(row_layout)

            pix_label = QLabel()
            pix_label.setFixedSize(key[3], key[4])
            loaded_pix = QPixmap()
            path_candidate = Path(self.tileset_infos[key[0]]["path"])
            if path_candidate.exists():
                # Load the full image, then maybe crop the region (x,y,w,h)
                full_img = QImage(str(path_candidate))
                cropped = full_img.copy(*key[1:])
                loaded_pix = QPixmap.fromImage(cropped)
            else:
                # fallback grey
                loaded_pix = QPixmap(*key[3:5])
                loaded_pix.fill(Qt.GlobalColor.gray)
            # scale if needed
            loaded_pix = loaded_pix.scaled(
                pix_label.width(),
                pix_label.height(),
                Qt.AspectRatioMode.KeepAspectRatio,
                Qt.TransformationMode.SmoothTransformation,
            )
            pix_label.setPixmap(loaded_pix)

            # 2) Text input
            line_edit = QLineEdit()
            line_edit.setPlaceholderText("Enter a name...")

            # 3) "Used for Multi-(...)" button
            multi_button = QPushButton(
                f"Used for Multi-{self.label_for_button}"
            )
            multi_button.clicked.connect(
                lambda _checked, row_key=key: self._discard_item(row_key)
            )

            row_layout.addWidget(pix_label)
            row_layout.addWidget(line_edit, stretch=1)
            row_layout.addWidget(multi_button)

            self._list_layout.addWidget(row_widget)

            # store references
            self._items[key] = {
                "widget": row_widget,
                "pix_label": pix_label,
                "line_edit": line_edit,
                "discarded": False,
            }

    def _discard_item(self, key):
        """Mark item as discarded, hide row."""
        if key in self._items:
            self._items[key]["discarded"] = True
            # remove from layout visually
            self._items[key]["widget"].setParent(None)

    def get_named_items(self):
        """
        Return a list of (user_name, UnnamedMonoAsset)
        for items that are not discarded and have a non-empty name.
        """
        results = []
        for key, info in self._items.items():
            if info["discarded"]:
                continue
            text_in = info["line_edit"].text().strip()
            if text_in:
                # reconstruct a UnnamedMonoAsset from the key
                (texture_name, x, y, w, h) = key
                coord = (texture_name, x, y, w, h)
                results.append((text_in, coord))
        return results

    def get_discarded_items(self):
        """
        Return a list of UnnamedMonoAsset for items that have been discarded.
        """
        results = []
        for key, info in self._items.items():
            if info["discarded"]:
                (texture_name, x, y, w, h) = key
                coord = (texture_name, x, y, w, h)
                results.append(coord)
        return results

    def on_selected(self, selected_list):
        """
        Slot that receives a list of (textureName, x, y, w, h);
        Fed into the items.
        """
        coords = []
        for sel in selected_list:
            coords.append(sel)
        self.add_or_update_items(coords)
