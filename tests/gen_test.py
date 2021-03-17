#!/usr/bin/env python
# Copyright 2016-2021 XMOS LIMITED.
# This Software is subject to the terms of the XMOS Public Licence: Version 1.

import argparse
import os.path
import numpy
import ctypes
import datetime

###############################################################################

def parseArguments():
    parser = argparse.ArgumentParser(description="FFT result builder")

    parser.add_argument('--length_log2', type=int, default=3,
                        help='Length of the FFT log 2')

    parser.add_argument('--storage', type=int, default=180,
      help='Space avaliable for fft results in memory (kB)')

    parser.add_argument('--shift', type=int, default=1,
      help='The shift applied to the input to the fft to scale the inputs to avoid overflow')

    parser.add_argument('--seed', type=int, default=1380144756,
      help='The seed of the crc32')

    args = parser.parse_args()

    return args

###############################################################################

def crc32(d, x, poly):
	crc = d
	data = x
	for bit in range(32):
		xorBit = (crc & 1)
		crc  = ((crc >> 1) & 0x7fffffff) | ((data & 1) << 31)
		data = (data >> 1) & 0x7fffffff
		if (xorBit != 0):
	  		crc = crc ^ poly
	return crc



###############################################################################

def generate(storage, length_log2, shift, seed, directory):
  header = open (os.path.join(directory, "generated.h"), 'w')

  year = datetime.datetime.now().year
  header.write("// Copyright (c) " +str(year) +", XMOS Ltd, All rights reserved\n")

  fft_length = 1<<length_log2
  fft_count = storage*1024 / (8*fft_length)

  header.write("#define SEED "  + str(seed) + "\n")
  header.write("#define FFT_LENGTH "  + str(fft_length) + "\n")
  header.write("#define FFT_LENGTH_LOG2 "  + str(length_log2) + "\n")
  header.write("#define FFT_COUNT "  + str(fft_count) + "\n")
  header.write("#define DATA_SHIFT "  + str(shift) + "\n")
  header.write("#define FFT_SINE_LUT dsp_sine_"  + str(fft_length) + "\n")
  poly = 0xEB31D82E
  data = seed

  header.write( "dsp_complex_t output[" + str(fft_count) + "][" + str(fft_length) + "] = {\n")
  for t in range(fft_count):

  	fft_input = []
  	for i in range(fft_length):
	  	data = crc32(data, -1, poly)
	  	re = ctypes.c_int32(data).value>>shift
	  	data = crc32(data, -1, poly)
	  	im = ctypes.c_int32(data).value>>shift
	  	fft_input.append(complex(re, im))

	sp = numpy.fft.fft(fft_input)
	header.write( "{\n")
	for i in range(fft_length):
		re = int(sp[i].real/fft_length)
		im = int(sp[i].imag/fft_length)
		header.write( "{" + str(re)+", " + str(im) + "},\n")
	header.write( "},\n")
  header.write( "};\n")
  return

if __name__ == "__main__":
  args = parseArguments()
  generate(args.storage, args.length_log2, args.shift, args.seed, '.');

  
