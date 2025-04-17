import argparse
import json
import os.path


def build_arguments_parser():
    parser = argparse.ArgumentParser(description="Utility to merge compile_commands.json for autocomplete by clangd")

    parser.add_argument("-O", "--output", type=str, required=True)
    parser.add_argument("input", type=argparse.FileType("r"), nargs="+")

    return parser


def main():
    print("Merging compile commands...")

    args = build_arguments_parser().parse_args()

    # NB(apachee): Marker to signal that original compile_commans might have been overwritten
    marker_file = f"{args.output}.marker"

    if os.path.exists(marker_file):
        print("Marker file present, merging skipped")
        return

    assert not os.path.exists(marker_file), "Trying to merge files with marker already present"

    compile_commands = []

    for input_file in args.input:
        partial_compile_commands = json.load(input_file)
        compile_commands.extend(partial_compile_commands)

    with open(args.output, 'w') as output_file:
        json.dump(compile_commands, output_file, indent=4)

    with open(marker_file, 'w') as output_file:
        pass

    print("Successfully merged compile commands")


if __name__ == "__main__":
    main()
