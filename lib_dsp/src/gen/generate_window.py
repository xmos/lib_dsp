# Copyright (c) 2015-2018, XMOS Ltd, All rights reserved
import sys
import numpy as np
from scipy import signal
import argparse

def parse_arguments():
    parser = argparse.ArgumentParser(help="Generates Q31 LUTs from scipy windows")
    parser.add_argument("-o", "--output-file", type=str)
    parser.add_argument("-f", "--function", type=str)
    parser.add_argument("-l", "--length", type=int)
    return parser.parse_args()


def main():
    args = parse_arguments()
    if args.length % 2 != 0:
        print("Error: Length must be even")
        sys.exit(1)

    window = signal.windows.get_window(args.function, args.length)

    # Split the window in half, then scale it to Q31
    half_int_window = np.array(
        window[:args.length//2] * np.iinfo(np.int32).max, dtype=np.int32
    )

    # Create a C array
    c_window = (
        f"const int window[120] = {{ {', '.join([str(i) for i in half_int_window])} }};"
    )

    # Write back to output file
    with open(args.output_file, 'w') as f:
        f.write(c_window + "\n")


if __name__ == "__main__":
    main()
