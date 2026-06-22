from pathlib import Path

DIRECTORY_BLACKLIST = [Path("./libs"), Path("./.git"), Path("./build")]
VALID_LANGUAGES = {
    "cpp": [".cpp", ".h"],
    "python": [".py"],
    "slang": [".slang"]
}
OUTPUT_LABEL_PADDING = 32


def pad_string(inp: str, pad_to: int):
    return inp + (" "*(pad_to - len(inp)))


def main():
    valid_file_extensions = [ext for item in VALID_LANGUAGES.values() for ext in item]
    lines_distribution = {}

    total_line_count = 0
    total_file_count = 0

    functional_line_count = 0

    stack = [Path("./")]
    while len(stack) != 0:
        current_path = stack.pop()
        # print("Searching directory: " + str(current_path.absolute()))

        for item in current_path.iterdir():
            if item.is_dir() and (item not in DIRECTORY_BLACKLIST):
                stack.append(item)
            elif item.is_file() and (item.suffix in valid_file_extensions):
                with open(item, "r", encoding="utf-8") as file:
                    for line in file:
                        if (len(line.strip()) != 0) and not line.startswith("//"):
                            functional_line_count += 1
                        total_line_count += 1
                        lines_distribution[str(item.suffix)] = lines_distribution.setdefault(str(item.suffix), 0)+1
                total_file_count += 1
                # print("\tAppended file: " + str(item.name))

    print("___________________PROJECT STATUS____________________")
    print(pad_string("TOTAL FILE COUNT: ", OUTPUT_LABEL_PADDING) + str(total_file_count))
    print(pad_string("TOTAL LINE COUNT: ", OUTPUT_LABEL_PADDING) + str(total_line_count))
    for key, val in lines_distribution.items():
        print(pad_string("\t"+key, OUTPUT_LABEL_PADDING) + str(val))
    print(pad_string("FUNCTIONAL LINE COUNT: ", OUTPUT_LABEL_PADDING) + str(functional_line_count))


if __name__ == "__main__":
    main()
