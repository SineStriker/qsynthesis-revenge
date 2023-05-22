from __future__ import annotations

import os
import shutil
import platform
import sys


def print_begin(s: str):
    sz: int = (60 - len(s))//2
    print(f"{ '-' * sz } {s} { '-' * sz }")


def println_twice():
    for _ in range(2):
        print()


def rmdir(dir: str):
    if os.path.isdir(dir):
        shutil.rmtree(dir)


def copydir(dir: str, dest: str):
    name = os.path.basename(dir)
    path = f"{dest}/{name}"
    if os.path.isfile(path):
        os.remove(path)
    elif os.path.isdir(path):
        shutil.rmtree(path)
    shutil.copytree(dir, path)


def copyfile(src: str, dest_dir: str):
    name = os.path.basename(src)
    path = f"{dest_dir}/{name}"
    if os.path.isfile(path):
        os.remove(path)
    elif os.path.isdir(path):
        shutil.rmtree(path)
    if os.path.isfile(dest_dir):
        os.remove(dest_dir)
    if not os.path.exists(dest_dir):
        os.makedirs(dest_dir)
    shutil.copyfile(src, os.path.join(dest_dir, name))


def get_platform() -> tuple[str, str]:
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

    return os_name, arch_name
