"""
Main Module.
"""

import sys

from PySide6.QtWidgets import QApplication

from app.views import MainWindow


def main():
    """Main function."""
    app = QApplication(sys.argv)

    # Initialize with empty configuration
    w = MainWindow()
    w.show()
    sys.exit(app.exec())


if __name__ == "__main__":
    main()
