# Fix rpath for libraries and plugins on Non-Windows OS, and do post deploying tasks

# Usage: python postdeploy.py <dir>

from __future__ import annotations
from typing import Iterable

import sys
import re
import pathlib

from utils import *


def mac_postdeploy(dir):
    # macOS postdeploy
    import dylib_utils

    def add_rpath_iter(executables_dir: str | pathlib.Path, rpaths: Iterable[str]):
        if not isinstance(executables_dir, pathlib.Path):
            executables_dir = pathlib.Path(executables_dir)
        if not executables_dir.exists():
            return
        for executable in executables_dir.iterdir():
            existing_rpaths = dylib_utils.get_binary_rpath(executable)
            for rpath in rpaths:
                if rpath in existing_rpaths:
                    continue

                dylib_utils.add_rpath(executable, rpath)

    install_prefix = pathlib.Path(dir)

    rpaths = {
        "@executable_path/../Frameworks",
        "@executable_path/../Frameworks/Qt",
        "@executable_path/../Frameworks/ChorusKit",
    }

    shared_prefix = install_prefix / "shared"
    executables_dir_shared = shared_prefix / "MacOS"

    add_rpath_iter(executables_dir_shared, rpaths)

    for app_bundle_prefix in install_prefix.iterdir():
        if (not app_bundle_prefix.is_dir()) or app_bundle_prefix.suffix != '.app':
            continue

        contents_prefix = app_bundle_prefix / "Contents"
        # Fix the rpaths for executables
        executables_dir = contents_prefix / "MacOS"
        frameworks_dir = contents_prefix / "Frameworks"
        plugins_dir = contents_prefix / "Plugins"
        qt_dir = frameworks_dir / "Qt"
        add_rpath_iter(executables_dir, rpaths)

        # recursively collect any other external dependencies and fix rpath
        to_collect = []
        to_analyze = []
        for framework in dylib_utils.iter_macho_recursive(frameworks_dir):
            to_analyze.append(str(framework))

        to_collect.extend(dylib_utils.analyze_dependencies(to_analyze))
        print(to_collect)

        for lib in to_collect:
            realpath = pathlib.Path(lib).resolve()
            prefix_real, name_real, _ = dylib_utils.get_lib_components(realpath)
            prefix_notional, name_notional, _ = dylib_utils.get_lib_components(lib)
            src = pathlib.Path(prefix_real) / name_real
            if re.match(r"^(Qt.*\.framework|libQt.*\.dylib)$", str(name_notional)):
                if not qt_dir.exists():
                    qt_dir.mkdir()
                dest = qt_dir / name_notional
            else:
                dest = frameworks_dir / name_notional
            if dest.exists():
                continue
            if src.is_dir():
                shutil.copytree(src, dest, symlinks=True)
            else:
                shutil.copy2(src, dest, follow_symlinks=True)

        for framework in dylib_utils.iter_macho_recursive(
                frameworks_dir, executables_dir, plugins_dir):
            should_codesign = False
            deps = dylib_utils.get_dependencies(framework)
            for dep in deps:
                if dylib_utils.get_path_type(dep) == 'other':
                    new_dep = '@rpath/' + dylib_utils.get_lib_components(dep)[2]
                    print("changing", framework, ":", dep, "->", new_dep)
                    dylib_utils.change_dependency(framework, dep, new_dep)
                    should_codesign = True

            install_id = dylib_utils.get_id(framework)
            if install_id != '' and dylib_utils.get_path_type(install_id) == 'other':
                new_install_id = dylib_utils.get_lib_components(install_id)[2]
                if not framework.is_relative_to(qt_dir / "plugins"):
                    install_id = "@rpath/" + new_install_id
                    dylib_utils.change_id(framework, install_id)
                    should_codesign = True

            if should_codesign:
                if dylib_utils.is_signed(framework):
                    dylib_utils.codesign_binary(framework)


def linux_postdeploy(dir):
    # Linux postdeploy
    import shutil
    import pathlib
    import subprocess

    def set_rpath(executable_path, rpath):
        # Set the RPATH of the executable
        subprocess.run(["patchelf", "--set-rpath", rpath, str(executable_path)], check=True)

    install_prefix = pathlib.Path(dir)

    # The directory where shared libraries will be copied
    lib_dir = install_prefix / "libs"

    # The directory where plugins will be copied
    plugins_dir = install_prefix / "plugins"

    # List of Qt plugins to deploy
    qt_plugins = ["platforms/libqxcb.so", "imageformats", "iconengines"]

    # Ensure directories exist
    lib_dir.mkdir(parents=True, exist_ok=True)
    plugins_dir.mkdir(parents=True, exist_ok=True)

    # Copy Qt libraries and plugins to the target directories
    qt_libs_path = subprocess.check_output(
        ["qmake", "-query", "QT_INSTALL_LIBS"]).strip().decode('utf-8')
    qt_plugins_path = subprocess.check_output(
        ["qmake", "-query", "QT_INSTALL_PLUGINS"]).strip().decode('utf-8')

    for plugin in qt_plugins:
        src = pathlib.Path(qt_plugins_path) / plugin
        if src.is_dir():
            shutil.copytree(src, plugins_dir / plugin, symlinks=True, dirs_exist_ok=True)
        else:
            shutil.copy2(src, plugins_dir, follow_symlinks=True)

    # Set the RPATH of the application executable
    # to look for libraries in the 'libs' directory
    executable_path = install_prefix / "your_application_executable"  # Change to your application executable name
    rpath = "$ORIGIN/libs"
    set_rpath(executable_path, rpath)


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
    elif os_name == "osx":
        print("MacOS")
        mac_postdeploy(dir)
    else:
        print("Linux")
        linux_postdeploy(dir)


if __name__ == "__main__":
    main()
