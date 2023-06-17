# Utilities to manipulate Mach-O binaries
# (executables and shared libraries) on macOS

from __future__ import annotations

import re
import os
import pathlib
import subprocess
import struct

from collections.abc import Iterable


FILE_EXEC = '/usr/bin/file'
OTOOL_EXEC = '/usr/bin/otool'
INSTALL_NAME_TOOL_EXEC = '/usr/bin/install_name_tool'
CODESIGN_EXEC = '/usr/bin/codesign'
LIPO_EXEC = '/usr/bin/lipo'


def get_path_type(path: "str | pathlib.Path"):
    path = str(path)
    if path.startswith('@rpath'):
        return 'rpath'
    elif path.startswith('@executable_path'):
        return 'executable_path'
    elif path.startswith('@loader_path'):
        return 'loader_path'
    else:
        system_prefix = [
            '/usr/lib/',
            '/Library/',
            '/System/',
        ]
        if any(path.startswith(prefix) for prefix in system_prefix):
            return 'system'
    return 'other'


def get_otool_l_output(path: "str | pathlib.Path",
                       arch: str = None) -> str:
    path = str(path)
    args = [OTOOL_EXEC]
    if arch is not None:
        args.extend(["-arch", arch])
    args.extend(["-l", path])
    output = subprocess.check_output(args).decode()
    return output


def get_binary_rpath(executable_path: "str | pathlib.Path",
                     arch: str = None) -> list[str]:
    executable_path = str(executable_path)
    output = get_otool_l_output(executable_path, arch)
    pattern = re.compile(
        r"\s*Load command \d+\n\s*cmd LC_RPATH\n\s*cmdsize \d+\n\s*path (.*) \(offset \d+\)")
    rpath_list = pattern.findall(output)
    return rpath_list


def get_dependencies(path: "str | pathlib.Path",
                     arch: str = None) -> list[str]:
    path = str(path)
    output = get_otool_l_output(path, arch)
    pattern = re.compile(
        r"\s*Load command \d+\n\s*cmd LC_LOAD_DYLIB\n\s*cmdsize \d+\n\s*name (.*) \(offset \d+\)")
    dependency_list = pattern.findall(output)
    return dependency_list


def get_id(path: "str | pathlib.Path",
           arch: str = None) -> str:
    path = str(path)
    output = get_otool_l_output(path, arch)
    pattern = re.compile(
        r"\s*Load command \d+\n\s*cmd LC_ID_DYLIB\n\s*cmdsize \d+\n\s*name (.*) \(offset \d+\)")
    result = pattern.findall(output)
    if result:
        return result[0]
    return ''


def get_lib_components(path: "str | pathlib.Path") -> tuple[str, str, str]:
    path = str(path)
    name = ""
    name_with_suffix = ""
    pattern = re.compile(
        r"^(?P<prefix>.*)/(?P<name>.*?\.framework)/(?P<suffix>.*)$")
    match = pattern.match(path)
    if match is not None:
        name = match.group("name")
        prefix = match.group("prefix")
        suffix = match.group("suffix")
        name_with_suffix = f"{name}/{suffix}"
    else:
        p = pathlib.Path(path)
        name = p.name
        prefix = p.parent
        name_with_suffix = name
    return prefix, name, name_with_suffix


def collect_dependencies_recursive(path: "str | pathlib.Path", *,
                                   ignore_path_types: Iterable[str] = None,
                                   verbose: bool = False) -> list[str]:
    def print_verbose(*args, **kwargs):
        if verbose:
            print(*args, **kwargs)

    if ignore_path_types is None:
        ignore_path_types = ['rpath', 'executable_path', 'loader_path', 'system']

    collect_list = set()
    path = str(path)
    print_verbose(f"=== Dependency tree for {path} ===")
    layer = 0

    # Recursion helper function
    def _helper(_path: str) -> str:
        nonlocal layer
        deps = [s for s in get_dependencies(_path)
                if (get_path_type(s) not in ignore_path_types)]
        print_verbose(f"[{layer}]".rjust(4), "|   " * layer + _path)
        for dep in deps:
            layer += 1
            item = _helper(dep)
            if item is not None:
                collect_list.add(item)
        layer -= 1
        return _path

    # Run recursion
    _helper(path)
    return list(collect_list)


def install_name_tool(macho_binary_path: "str | pathlib.Path",
                      commands: dict):
    macho_binary_path = str(macho_binary_path)
    cmd_add_rpath = commands.get("add_rpath", [])
    cmd_delete_rpath = commands.get("delete_rpath", [])
    cmd_change_rpath = commands.get("rpath", [])
    cmd_change = commands.get("change", [])
    cmd_id = commands.get("id", None)

    _args = []
    for new_rpath in cmd_add_rpath:
        _args.extend(["-add_rpath", str(new_rpath)])
    for old_rpath in cmd_delete_rpath:
        _args.extend(["-delete_rpath", str(old_rpath)])
    for old_rpath, new_rpath in cmd_change_rpath:
        _args.extend(["-rpath", str(old_rpath), str(new_rpath)])
    for old_path, new_path in cmd_change:
        _args.extend(["-change", str(old_path), str(new_path)])
    if cmd_id is not None:
        _args.extend(["-id", str(cmd_id)])
    if len(_args) == 0:
        return
    exec_args = [INSTALL_NAME_TOOL_EXEC]
    exec_args.extend(_args)
    exec_args.append(macho_binary_path)
    subprocess.run(exec_args)


def add_rpath(executable_path: "str | pathlib.Path",
              new: str):
    install_name_tool(executable_path, commands={"add_rpath": [new]})


def delete_rpath(executable_path: "str | pathlib.Path",
                 old: str):
    install_name_tool(executable_path, commands={"delete_rpath": [old]})


def change_rpath(executable_path: "str | pathlib.Path",
                 old: str, new: str):
    install_name_tool(executable_path, commands={"rpath": [(old, new)]})


def change_dependency(executable_path: "str | pathlib.Path",
                      old: str, new: str):
    install_name_tool(executable_path, commands={"change": [(old, new)]})


def change_id(shared_lib_path: "str | pathlib.Path",
              name: str):
    install_name_tool(shared_lib_path, commands={"id": name})


def codesign_binary(path: "str | pathlib.Path",
                    identity: str = '-'):
    path = str(path)
    print(
        f"Codesigning for {path} " + \
        ("(adhoc signing)" if identity == '-' else f"using identity {identity}") + \
        "..."
    )
    subprocess.run([CODESIGN_EXEC, '--force', '--sign', identity, path])


def is_macho(path: "str | pathlib.Path"):
    """
    Determine if a file is Mach-O binary.
    """
    path = str(path)

    # First, let's check the first 4 bytes of the file
    # to quickly filter out non Mach-O binaries.

    # These are magic numbers to identify Mach-O binaries.
    macho_magic = {
        0xfeedface,  # MH_MAGIC
        0xcefaedfe,  # MH_CIGAM
        0xfeedfacf,  # MH_MAGIC_64
        0xcffaedfe,  # MH_CIGAM_64
        0xcafebabe,  # FAT_MAGIC
        0xbebafeca,  # FAT_CIGAM
    }

    magic_len = 4
    header_len = 32

    if os.stat(path).st_size < header_len:
        return False

    with open(path, "rb") as fp:
        header = fp.read(magic_len)

    magic = struct.unpack("<I", header)[0]
    if magic not in macho_magic:
        return False

    # Since these magic numbers may also be used by other file types
    # (e.g. CAFEBABE is also used by Java .class),
    # we should check it again using `file` command to be sure.
    output_b = subprocess.check_output([FILE_EXEC, "-b", path])
    return output_b.startswith(b'Mach-O')


def strip_universal_binary(input_file: "str | pathlib.Path",
                           target_arch: str,
                           skip_error: bool = False):
    input_file = str(input_file)
    cmd_output = subprocess.check_output([LIPO_EXEC, "-archs", input_file])
    if cmd_output.startswith(b"fatal error:"):
        # skip these files, they are not executables or shared libraries
        return

    all_archs = cmd_output.split()

    if target_arch not in all_archs:
        if not skip_error:
            raise ValueError(f"Target architecture {target_arch} not in input file {input_file}")
        else:
            return

    if len(all_archs) == 1:
        # Thin binary, nothing to do.
        return

    # For fat binaries, extract target architecture
    output_file = f"{input_file}_{target_arch}"
    subprocess.run([LIPO_EXEC, input_file, "-thin", target_arch, "-output", output_file])
    os.remove(input_file)
    os.rename(output_file, input_file)


def strip_universal_binary_recursive(input_dir: "str | pathlib.Path",
                                     target_arch: str,
                                     skip_error: bool = False):
    inp = pathlib.Path(input_dir)
    for file in inp.rglob('*'):
        if file.is_dir() or file.is_symlink() or file.is_fifo() or file.is_block_device() or file.is_char_device():
            # skip directories, symbolic links, named pipes, block devices, character devices
            continue
        if not is_macho(file):
            continue

        strip_universal_binary(file, target_arch, skip_error)


def analyze_dependencies(paths: Iterable[str],
                         *,
                         ignore_path_types: Iterable[str] = None) -> list[str]:
    if ignore_path_types is None:
        ignore_path_types = ['rpath', 'executable_path', 'loader_path', 'system']

    collect_list = set()

    # Recursion helper function
    def _helper(_path: str) -> str:
        if _path in collect_list:
            print("Skip:", _path)
            return None

        print("Get deps:", _path)
        deps = [s for s in get_dependencies(_path)
                if (get_path_type(s) not in ignore_path_types) and s not in collect_list]
        print("Get deps:", _path)
        for dep in deps:
            print(dep)
            if dep in collect_list:
                print("Skip:", dep)
                continue

            print("Do:", dep)
            item = _helper(dep)
            if item is not None:
                collect_list.add(item)
                print(collect_list)
        return _path

    for path in paths:
        path = str(path)

        # Run recursion
        _helper(path)

    return list(collect_list)


def iter_macho_recursive(*parents: "str | pathlib.Path"):
    for parent in parents:
        parent = pathlib.Path(parent)
        for framework in parent.rglob("*"):
            if framework.is_dir() or framework.is_symlink() or framework.is_fifo() or \
                    framework.is_char_device() or framework.is_block_device():
                continue

            if not is_macho(framework):
                continue

            yield framework
