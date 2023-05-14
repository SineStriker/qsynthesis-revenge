# Fix rpath for libraries and plugins on Non-Windows OS, and do post deploying tasks

# Usage: python postdeploy.py <dir>

import sys

from utils import *


def main():
    
    # Parse args
    if len(sys.argv) < 2:
        print("Usage: python postdeploy.py <dir>")
        sys.exit(0)

    dir = sys.argv[1]

    # Get platform
    os_name, arch_name = get_platform()

    # Do deploy work for specific platform
    if os_name == "windows":
        # Nothing to do on Windows
        sys.exit(0)


if __name__ == "__main__":
    main()
