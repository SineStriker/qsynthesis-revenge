import os
import shutil

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