# Deploy Qt libraries and plugins on Non-Windows OS

# Usage: python unixdeployqt.py [--qmake path] [--libdir dir] [--plugindir dir] [--verbose] <binaries>

import argparse
import os
import re
import subprocess
import shutil

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
    parser.add_argument("--debug", help="Deploy debug libraries.", action="store_true")
    parser.add_argument(
        "--verbose", help="Show deploy process.", action="store_true")
    parser.add_argument('files', nargs='*')
    args = parser.parse_args()

    print("Selected plugins destination:", args.plugindir)
    print("Selected libraries destination:", args.libdir)
    print("Working directory:", os.getcwd())

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
        # Obtain Qt library dependencies of the required libraries
        qtdeps = set()
        for libname in args.files:
            otool_exc = ["otool", "-L", libname]
            regex = re.compile(r'^\s*(?P<qt>.+Qt\w+\.framework).+\s\(.+$')

            # Call otool.
            output = subprocess.check_output(otool_exc).decode('utf-8')

            # Parse the output.
            dependencies = []
            for line in output.splitlines():
                match = regex.match(line)
                if match:
                    dependencies.append(match.group('qt'))

            qtdeps = qtdeps.union(set(dependencies))
        if args.debug:
            print("Asked for debug version Qt libraries.")
        print(f"Qt dependencies: {qtdeps}")

        for qt_framework in qtdeps:
            print(f"  Copying {qt_framework}...")
            copydir(qt_framework, args.libdir)

        # Query Qt Plugins path
        qtplugins_path = subprocess.check_output([args.qmake, "-query", "QT_INSTALL_PLUGINS"]).strip().decode('utf-8')
        print(f"Qt Plugins path: {qtplugins_path}")

        plugin_list = ['bearer', 'iconengines', 'imageformats', 'platforminputcontexts', 'styles', 'virtualkeyboard']
        plugin_dylib_list = ['platforms/libqcocoa.dylib']

        for plugin_src in plugin_list:
            print(f"  Copying {plugin_src}...")
            copydir(f"{qtplugins_path}/{plugin_src}", f"{args.plugindir}")

        for plugin_dylib in plugin_dylib_list:
            print(f"  Copying {plugin_dylib}...")
            base_dir = os.path.dirname(f"{args.plugindir}/{plugin_dylib}")
            copyfile(f"{qtplugins_path}/{plugin_dylib}", base_dir)

    else:
        print("Linux")


if __name__ == "__main__":
    main()
