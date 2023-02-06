#!/usr/bin/python3

from __future__ import annotations

import os
import sys
import shutil
import platform

import enum
import argparse
import urllib.request


def print_begin(s: str):
    sz: int = (60 - len(s))//2
    print(f"{ '-' * sz } {s} { '-' * sz }")


def println_twice():
    for _ in range(2):
        print()


def rmdir(dir: str):
    if os.path.isdir(dir):
        shutil.rmtree(dir)


class OSFlags(enum.Flag):
    WINDOWS = 1
    MAC = 2
    LINUX = 4
    EXCLUDE_WINDOWS = MAC | LINUX
    EXCLUDE_MAC = WINDOWS | LINUX
    EXCLUDE_LINUX = WINDOWS | MAC
    ALL = WINDOWS | MAC | LINUX

    def Validate(os_flags: OSFlags) -> bool:
        os_name = platform.system().lower()
        if os_name == "windows":
            return os_flags & OSFlags.WINDOWS
        elif os_name == "darwin":
            return os_flags & OSFlags.MAC
        elif os_name == "linux":
            return os_flags & OSFlags.LINUX
        return False


class library_task:

    vcpkg: str                  # vcpkg command
    triplet: str                # os and arch
    overlay_ports: str          # repository override
    overlay_triplets: str       # build configuration override

    """
    name                 : package name
    use_overlay_ports    : use your repository ports instead of default ports
    use_overlay_triplets : use your own build settings (Always on)
    os_flags             : specify operating systems the library supports
    """

    def __init__(self, name: str,
                 use_overlay_ports: bool = False,
                 use_overlay_triplets: bool = True,
                 os_flags: OSFlags = OSFlags.ALL):
        self.name = name
        self.use_overlay_ports = use_overlay_ports
        self.use_overlay_triplets = use_overlay_triplets
        self.os_flags = os_flags

    def install(self) -> int:
        if not OSFlags.Validate(self.os_flags):
            return 0
        cmds: list[str] = []
        cmds.append(f"{library_task.vcpkg}")
        cmds.append("install")
        cmds.append(f"{self.name}:{library_task.triplet}")
        if self.use_overlay_ports:
            cmds.append(f"--overlay-ports={library_task.overlay_ports}")
        if self.use_overlay_triplets:
            cmds.append(f"--overlay-triplets={library_task.overlay_triplets}")
        return os.system(str(" ").join(cmds))


# Vcpkg repo
vcpkg_repo = "https://github.com/microsoft/vcpkg.git"

# TODO: To be added.
vcpkg_tasks: list[library_task] = [
    library_task("zlib"),
    library_task("sdl2"),
    library_task("yaml-cpp"),
    library_task("quazip", True),
    library_task("framelesshelper", True, os_flags=OSFlags.EXCLUDE_MAC),
    library_task("ffmpeg-fake", True),
    library_task("qnrbf", True),
]


# Usage: python setup_vcpkg.py [--clean] [--distclean]

if __name__ == "__main__":

    # Parse args
    parser = argparse.ArgumentParser(
        description="Setup vcpkg for this project.")
    parser.add_argument("-c",
                        "--clean", help="Clean local caches after build.", action="store_true")
    parser.add_argument(
        "--distclean", help="Clean all caches after build.", action="store_true")
    parser.add_argument(
        "--skip", help="Skip build.", action="store_true")
    args = parser.parse_args()

    # Determine os and arch
    os_name = platform.system().lower()
    arch_name = platform.machine().lower()

    # vcpkg calls macOS as osx, change it on the fly
    if os_name == "darwin":
        os_name = "osx"

    if arch_name == "i386" or arch_name == "x86":
        arch_name = "x86"
    elif arch_name == "amd64" or arch_name == "x86_64":
        arch_name = "x64"
    elif arch_name == "arm64":
        pass
    else:
        print(f"Unsupported architecture {arch_name}!")
        sys.exit(-1)

    # Determine file extension name
    if os_name == 'windows':
        exe_suffix = ".exe"
        bat_suffix = ".bat"
        dot_slash = ""
        vcpkg_cache_dir = f"{os.environ['LOCALAPPDATA']}/vcpkg"
    else:
        exe_suffix = ""
        bat_suffix = ".sh"
        dot_slash = "./"
        vcpkg_cache_dir = f"{os.environ['HOME']}/.cache/vcpkg"

    vcpkg_dir = "vcpkg"
    vcpkg_exe = "vcpkg" + exe_suffix
    bootstrap_bat = "bootstrap-vcpkg" + bat_suffix

    vcpkg_cmd = dot_slash + vcpkg_exe
    bootstrap_cmd = dot_slash + bootstrap_bat

    # Configure tasks
    library_task.vcpkg = vcpkg_cmd
    library_task.triplet = f"{arch_name}-{os_name}"
    library_task.overlay_ports = "../scripts/vcpkg/ports"
    library_task.overlay_triplets = "../scripts/vcpkg/triplets"

    # -- Change to .py path and start main process
    app_path = os.path.dirname(os.path.realpath(__file__))
    os.chdir(app_path)

    # Check script path
    if not os.path.isdir("scripts/vcpkg"):
        print("Vcpkg scripts not found")
        sys.exit(-1)
    
    # Vcpkg path configuration file
    paths_txt_path = "scripts/vcpkg/triplets/paths/path_qt.txt"
    if not os.path.isfile(paths_txt_path):
        print(f"{paths_txt_path} not found")
        sys.exit(-1)

    # -- Detect system proxy
    print_begin("Setup context proxy")

    proxy = urllib.request.getproxies()

    if "http" in proxy:
        print(f"HTTP proxy detected: {proxy['http']}")
        os.environ["http_proxy"] = proxy['http']
        os.environ["https_proxy"] = proxy['http']
    else:
        print("System proxy not detected")

    println_twice()

    # -- Execute git clone
    print_begin("Pull vcpkg")

    # Remove dir if .git doesn't esist
    if os.path.isdir(vcpkg_dir) and not os.path.isdir(vcpkg_dir + "/.git"):
        print("Vcpkg repository is not valid, auto remove")
        rmdir(vcpkg_dir)

    if not os.path.isdir(vcpkg_dir):
        print(f"Clone {vcpkg_repo}")
        code = os.system(f"git clone {vcpkg_repo}")  # git clone vcpkg.git
        if code != 0:
            print("Clone failed")
            sys.exit(code)
    else:
        print(f"Vcpkg repository found")

    println_twice()

    # -- Execute bootstrap
    print_begin("Vcpkg Bootstrap")

    os.chdir(vcpkg_dir)  # cd vcpkg
    if not os.path.isfile(vcpkg_exe):
        code = os.system(f"{bootstrap_cmd}")  # ./bootstrap
        if code != 0:
            print("Bootstrap failed")
            sys.exit(code)
    else:
        print(f"Vcpkg executable found")

    println_twice()

    if not args.skip:
        # -- Build libraries
        for task in vcpkg_tasks:
            print_begin(f"Build {task.name}")

            code = task.install()               # ./vcpkg install ...
            if code != 0:
                print(f"Build {task.name} failed")
                sys.exit(code)

            println_twice()

    # -- Clean up
    print_begin("Clean up")

    if args.clean or args.distclean:
        print("Remove build trees")
        rmdir("buildtrees")

        print("Remove downloads")
        rmdir("downloads")

        print("Remove packages")
        rmdir("packages")

        if args.distclean:
            print("Remove caches")
            rmdir(vcpkg_cache_dir)
    else:
        print("Skip.")

    println_twice()

    # -- End
    print("Pre-build tasks finished")
