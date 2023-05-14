# Deploy Qt libraries and plugins on Non-Windows OS

# Usage: python unixdeployqt.py [--qmake path] [--libdir dir] [--plugindir dir] [--verbose] <binaries>

import argparse
import os
import subprocess

from utils import *


def main():

    # Parse args
    parser = argparse.ArgumentParser(
        description="Deploy qt libraries for this project.")
    parser.add_argument("--qmake", metavar="<path>",
                        help="Qt qmake path.", type=str, required=True)
    parser.add_argument("--libdir", metavar="<path>",
                        help="Copy libraries to path.", type=str, default=".")
    parser.add_argument("--plugindir", metavar="<path>",
                        help="Copy plugins to path.", type=str, default=".")
    parser.add_argument(
        "--verbose", help="Show deploy process.", action="store_true")
    parser.add_argument('files', nargs='*')
    args = parser.parse_args()

    # Get platform
    os_name, arch_name = get_platform()

    # Do deploy work for specific platform
    if os_name == "windows":
        windeployqt = os.path.dirname(args.qmake) + "/windeployqt.exe"
        if not os.path.isfile(windeployqt):
            print("windeployqt.exe not found!!!")
            sys.exit(-1)

        # Simply running windeployqt

        cmds: list[str] = [
            windeployqt,
            "--libdir",
            args.libdir,
            "--plugindir",
            args.plugindir,
            "--no-translations",
            "--no-system-d3d-compiler",
            "--no-compiler-runtime",
            "--no-opengl-sw",
            "--force",
        ]

        if not args.verbose:
            cmds += ["--verbose", "0"]

        cmds += args.files

        code = subprocess.run(cmds).returncode
        if code != 0:
            print("Deploy failed")
            sys.exit(code)

    elif os_name == "osx":
        print("Mac")
    else:
        print("Linux")


if __name__ == "__main__":
    main()
