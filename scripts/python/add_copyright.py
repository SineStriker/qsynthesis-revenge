import os
import sys


def get_sources(base_path: str):
    sources = []

    if not os.path.isdir(base_path):
        return sources

    search_paths = [base_path]
    while len(search_paths) > 0:
        path = search_paths[0]
        del (search_paths[0])

        for item in os.listdir(path):
            full_path = f'{path}/{item}'
            if os.path.isdir(full_path):
                if not item in ["3rdparty"]:
                    search_paths.append(full_path)
            else:
                if os.path.splitext(item)[1] in [".h", ".c", ".cpp", ".cc"]:
                    sources.append(full_path)

    return sources


def main():
    if len(sys.argv) <= 1:
        print(
            f"Usage: python {os.path.basename(__file__)} <base_dir> [copyright_file]")
        return

    sources = get_sources(sys.argv[1])

    if len(sources) == 0:
        print("No sources found")
        return

    if len(sys.argv) > 2:
        copyright_file = sys.argv[2]
    else:
        copyright_file = os.path.dirname(__file__) + "/copyright1.txt"

    with open(copyright_file, "r", encoding="utf-8-sig") as f:
        copyright_lines = f.readlines()

    copyright_comments = [
        "/****************************************************************************\n"]
    copyright_comments.append(" *\n")
    for line in copyright_lines:
        if not line.endswith("\n"):
            line += "\n"
        copyright_comments.append(f" * {line}")
    copyright_comments.append(" *\n")
    copyright_comments.append(
        " ****************************************************************************/\n")

    i = 1
    for source in sources:
        print(f"Process {i}/{len(sources)}: {source}")
        i += 1

        with open(source, "r", encoding="utf-8-sig") as f:
            lines = f.readlines()

        is_comment = False
        while len(lines) > 0:
            if lines[0].lstrip().startswith("/*"):
                is_comment = True
            elif lines[0].rstrip().endswith("*/"):
                is_comment = False
            elif not is_comment and lines[0].lstrip().rstrip() != "":
                break
            del (lines[0])

        with open(source, "w", encoding="utf-8") as f:
            f.writelines(copyright_comments)
            f.write("\n\n")
            f.writelines(lines)


if __name__ == "__main__":
    main()
