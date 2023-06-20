# Deploy Qt libraries and plugins on Non-Windows OS

# Usage: python deployqt.py [--qmake path] [--libdir dir] [--plugindir dir] [--verbose] <binaries>

from __future__ import annotations

import argparse
import os
import re
import glob
import shutil
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
        "--debug", help="Deploy debug libraries.", action="store_true")
    parser.add_argument(
        "--verbose", help="Show deploy process.", action="store_true")
    parser.add_argument('files', nargs='*')
    args = parser.parse_args()

    def print_verbose(*va_args):
        if args.verbose:
            print(va_args)

    print_verbose("Selected plugins destination:", args.plugindir)
    print_verbose("Selected libraries destination:", args.libdir)
    print_verbose("Working directory:", os.getcwd())

    # Get platform
    os_name, arch_name = get_platform()

    # Do deploy work for specific platform
    if os_name == "windows":
        qmake_dir = os.path.dirname(args.qmake)
        windeployqt = qmake_dir + "/windeployqt.exe"
        if not os.path.isfile(windeployqt):
            print("windeployqt.exe not found!!!")
            sys.exit(-1)

        # Add qmake directory to PATH
        path_var = os.environ["PATH"]
        os.environ["PATH"] = qmake_dir + \
            os.pathsep + os.environ.get("PATH", "")

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

        # Restore PATH
        os.environ["PATH"] = path_var

        if code != 0:
            print("Deploy failed")
            sys.exit(code)

    elif os_name == "osx":
        files_to_analyze = []
        files_to_analyze.extend(args.files)

        DEBUG_SUFFIX = "_debug"
        if args.debug:
            print_verbose("Asked for debug version Qt libraries.")
            ignore = None
        else:
            # Do not copy debug version Qt libraries for Release build
            ignore = shutil.ignore_patterns(
                f"*{DEBUG_SUFFIX}", f"*{DEBUG_SUFFIX}.dylib")

        # Query Qt Plugins path
        qtplugins_path = subprocess.check_output(
            [args.qmake, "-query", "QT_INSTALL_PLUGINS"]).strip().decode('utf-8')
        print_verbose(f"Qt Plugins path: {qtplugins_path}")

        plugin_list = ['bearer', 'iconengines', 'imageformats',
                       'platforminputcontexts', 'styles', 'virtualkeyboard']

        # No debug dylibs here. It will be added in the following steps.
        plugin_dylib_list = ['platforms/libqcocoa.dylib']

        if args.debug:
            print_verbose("Asked for debug version Qt libraries.")

            # Add debug dylibs
            plugin_dylib_list_debug = [
                re.sub("^(.*)\\.dylib$", f"\\1{DEBUG_SUFFIX}.dylib", s)
                for s in plugin_dylib_list
            ]
            plugin_dylib_list.extend(plugin_dylib_list_debug)

        for plugin_src in plugin_list:
            print_verbose(f"  Copying {plugin_src}...")
            copydir(f"{qtplugins_path}/{plugin_src}",
                    f"{args.plugindir}", symlinks=True, ignore=ignore)

        for plugin_dylib in plugin_dylib_list:
            print_verbose(f"  Copying {plugin_dylib}...")
            base_dir = os.path.dirname(f"{args.plugindir}/{plugin_dylib}")
            copyfile(f"{qtplugins_path}/{plugin_dylib}",
                     base_dir, follow_symlinks=False)

        plugin_dylib_paths = glob.glob(f"{args.plugindir}/**/*.dylib", recursive=True)
        files_to_analyze.extend(plugin_dylib_paths)

        # Obtain Qt library dependencies of the required libraries
        qtlibs_path = subprocess.check_output(
            [args.qmake, "-query", "QT_INSTALL_LIBS"]).strip().decode('utf-8')
        print_verbose(f"Qt Libraries path: {qtlibs_path}")

        qtdeps = set()
        for libname in files_to_analyze:
            otool_exc = ["otool", "-L", libname]
            regex = re.compile(
                r'^\s*(?P<qt_prefix>.+)(?P<qt>Qt\w+\.framework)(?P<qt_suffix>.+)\s\(.+$')

            # Call otool.
            output = subprocess.check_output(otool_exc).decode('utf-8')

            # Parse the output.
            dependencies = []
            for line in output.splitlines():
                match = regex.match(line)
                if match:
                    qt_framework_name = match.group('qt')
                    qt_framework_fullpath = os.path.join(
                        qtlibs_path, qt_framework_name)
                    dependencies.append(qt_framework_fullpath)

            qtdeps = qtdeps.union(set(dependencies))

        print_verbose(f"Qt dependencies: {qtdeps}")

        for qt_framework in qtdeps:
            print_verbose(f"  Copying {qt_framework}...")
            copydir(qt_framework, args.libdir, symlinks=True, ignore=ignore)

    elif os_name == "linux":
        files_to_analyze = []
        files_to_analyze.extend(args.files)

        DEBUG_SUFFIX = "_debug"
        if args.debug:
            print_verbose("Asked for debug version Qt libraries.")
            ignore = None
        else:
            # Do not copy debug version Qt libraries for Release build
            ignore = shutil.ignore_patterns(f"*{DEBUG_SUFFIX}", f"*{DEBUG_SUFFIX}.so")

        # Query Qt Plugins path
        qtplugins_path = (
            subprocess.check_output([args.qmake, "-query", "QT_INSTALL_PLUGINS"])
            .strip()
            .decode("utf-8")
        )
        print_verbose(f"Qt Plugins path: {qtplugins_path}")

        plugin_list = [
            "bearer",
            "iconengines",
            "imageformats",
            "platforminputcontexts",
            "styles",
            "virtualkeyboard",
        ]

        # No debug dylibs here. It will be added in the following steps.
        plugin_so_list = ["platforms/libqxcb.so"]

        if args.debug:
            print_verbose("Asked for debug version Qt libraries.")

            # Add debug dylibs
            plugin_so_list_debug = [
                re.sub("^(.*)\\.so$", f"\\1{DEBUG_SUFFIX}.so", s)
                for s in plugin_so_list
            ]
            plugin_so_list.extend(plugin_so_list_debug)

        for plugin_src in plugin_list:
            print_verbose(f"  Copying {plugin_src}...")
            copydir(
                f"{qtplugins_path}/{plugin_src}",
                f"{args.plugindir}",
                symlinks=True,
                ignore=ignore,
            )

        for plugin_so in plugin_so_list:
            print_verbose(f"  Copying {plugin_so}...")
            base_dir = os.path.dirname(f"{args.plugindir}/{plugin_so}")
            copyfile(f"{qtplugins_path}/{plugin_so}", base_dir, follow_symlinks=False)

        plugin_so_paths = glob.glob(f"{args.plugindir}/**/*.so", recursive=True)
        files_to_analyze.extend(plugin_so_paths)

        # Obtain Qt library dependencies of the required libraries
        qtlibs_path = (
            subprocess.check_output([args.qmake, "-query", "QT_INSTALL_LIBS"])
            .strip()
            .decode("utf-8")
        )
        print_verbose(f"Qt Libraries path: {qtlibs_path}")

        qtdeps = set()
        for libname in files_to_analyze:
            ldd_exc = ["ldd", libname]

            # Call ldd.
            output = subprocess.check_output(ldd_exc).decode("utf-8")

            # Parse the output.
            dependencies = []
            for line in output.splitlines():
                if "=>" in line and "(" in line:
                    deps_path = line.split(" => ")[1].split(" (")[0]
                    if qtlibs_path in deps_path:
                        dependencies.append(deps_path)

            qtdeps = qtdeps.union(set(dependencies))

        print_verbose(f"Qt dependencies: {qtdeps}")

        for qt_library in qtdeps:
            print_verbose(f"  Copying {qt_library}...")
            copyfile(qt_library, args.libdir, follow_symlinks=False)


if __name__ == "__main__":
    main()
