import json
import os


class Config:
    def __init__(self) -> None:
        self.Qt5ConfigDir = ""


config_inst = Config()

config_dict = {
    "title": "This is a configuration file the content of which varies with each user",
    "values": {
        "Qt5ConfigDir": {
            "intro": "The absolute path where Qt5Config.cmake exists",
            "value": "<edit here>"
        }
    }
}

config_path = "setup-vcpkg.json"


def load() -> bool:
    # Check config exists
    if not os.path.isfile(config_path):
        print(f"Configuration file {config_path} doesn't exists, generate new one.")
        generate()
        return False

    # Read config
    with open(config_path, encoding='utf-8') as f:
        root = json.load(f)
        config_inst.Qt5ConfigDir = root.get(
            "values").get("Qt5ConfigDir").get("value")

    # Check Qt5Config.cmake exists
    if not os.path.isfile(f"{config_inst.Qt5ConfigDir}/Qt5Config.cmake"):
        print(f"Qt5Config.cmake not found in \"{config_inst.Qt5ConfigDir}\".")
        return False

    # Write file
    paths_txt_path = "scripts/vcpkg/triplets/paths/path_qt.txt"
    with open(paths_txt_path, "w") as f:
        f.write(config_inst.Qt5ConfigDir)
    
    return True

def generate():
    with open(config_path, "w") as f:
        f.write(json.dumps(config_dict, indent=4, ensure_ascii=False))