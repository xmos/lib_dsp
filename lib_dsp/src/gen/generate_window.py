# Copyright 2015-2021 XMOS LIMITED.
# This Software is subject to the terms of the XMOS Public Licence: Version 1.
import sys
import numpy as np
from scipy import signal
import argparse

def parse_arguments():
    parser = argparse.ArgumentParser(description="Generates Q31 LUTs from scipy windows")
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

    # Window is not quite half - oddity with scipy windows, middle value is 1 so a
    # window with even length is non-symmetric
    half_int_window = np.array(
        window[:(args.length//2)+1] * np.iinfo(np.int32).max, dtype=np.int32
    )

    # Create a C array
    c_window = (
        f"// LUT for {args.function} window, generated by lib_dsp generate_window.py\n"
        f"const int window[{len(half_int_window)}] = "
        f"{{ {', '.join([str(i) for i in half_int_window])} }};"
    )

    # Write back to output file
    with open(args.output_file, 'w') as f:
        f.write(c_window + "\n")


if __name__ == "__main__":
    main()
