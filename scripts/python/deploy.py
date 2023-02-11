#!/usr/bin/python3

import os
import sys
import platform

from .utils import *
from .vcpkg_init import *


build_dir = "setup-vcpkg-temp-build"


def deploy(out_dir : str):
    if not os.path.isdir(out_dir):
        print(f"{out_dir} is not a directory.")
        return

    # Check requirements
    print_begin("Check requirements")
    if platform.system().lower() == 'windows':
        ret = os.system("cl /? | findstr x64")
        if ret != 0:
            print("Microsoft Visual C++ Compiler not found")
            sys.exit(ret)
    
    ret = os.system("cmake --version")
    if ret != 0:
        print("CMake not found")
        sys.exit(ret)
    
    ret = os.system("ninja --version")
    if ret != 0:
        print("Ninja not found")
        sys.exit(ret)
    
    py_cmd = ""
    ret = os.system("python --version")
    if ret != 0:
        ret = os.system("python3 --version")
        if ret != 0:
            print("Python not found")
            sys.exit(ret)
        py_cmd = "python3"
    else:
        py_cmd = "python"
    println_twice()

    # Config
    print_begin("CMake Config")
    ret = os.system(
        f"cmake -S . -B {build_dir} -DCMAKE_PREFIX_PATH:STRING={config_inst.Qt5ConfigDir} -DDCMAKE_BUILD_TYPE:STRING=Release -DCONFIG_WIN32_DEBUG:BOOL=off -G Ninja --no-warn-unused-cli")
    if ret != 0:
        sys.exit(ret)
    println_twice()

    # Build
    print_begin("CMake Build")
    ret = os.system(f"cmake --build {build_dir} --target all")
    if ret != 0:
        sys.exit(ret)
    println_twice()

    # Deploy
    print_begin("Deploy")
    ret = os.system(f"cmake --build {build_dir} --target deploy")
    if ret != 0:
        sys.exit(ret)
    println_twice()

    # Copy
    print_begin("Copy")
    copied = False
    for file_name in os.listdir(build_dir):
        if file_name.startswith("choruskit-"):
            copied = True
            print(f"Copy \"{file_name}\" to \"{out_dir}\"")
            copydir(f"{build_dir}/{file_name}", out_dir)
    if not copied:
        print("Deploy directory not found.")
        sys.exit(-1)
    println_twice()

    # Remove cache
    print_begin("Remove Cache")
    print(f"Remove build directory \"{build_dir}\".")
    rmdir(build_dir)
