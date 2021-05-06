# Copyright 2019-2021 XMOS LIMITED.
# This Software is subject to the terms of the XMOS Public Licence: Version 1.
import numpy as np
import matplotlib.pyplot as plt
from scipy import signal

		# show = True
		# if show:
		# 	w, h = signal.freqz_zpk(z, p, k, fs=rate)
		# 	plt.plot(w,20*np.log10(abs(h)))
		# 	plt.show()

def butter_highpass(cutoff, fs, order):
    nyq = 0.5 * fs
    normal_cutoff = cutoff / nyq
    z, p, k = signal.butter(order, normal_cutoff, output = 'zpk', btype='high', analog=False)
    return z, p, k

def butter_lowpass(cutoff, fs, order):
    nyq = 0.5 * fs
    normal_cutoff = cutoff / nyq
    z, p, k = signal.butter(order, normal_cutoff, output = 'zpk', btype='low', analog=False)
    return z, p, k


rate = 16000
cut_offs = [100., 200., 500., 1000., 2000., 4000., 5000., 6000.]
orders = [1, 2, 3, 4, 5, 6, 7, 8]

for order in orders:

	print("int32_t filter"+str(order) + "[" + str(len(cut_offs)*2) + "] = {")
	for cut_off in cut_offs:

		z, p, k = butter_highpass(cut_off, rate, order = order)

		sos = signal.zpk2sos(z, p, k)
		max_coef = np.amax(abs(sos))
		q_format_str = 'Q'+str(31-int(np.ceil(np.log2(max_coef))))

		print("{")
		for section_index in range(sos.shape[0]):
			for coef_index in range(3):
				print(q_format_str + '(' +str(sos[section_index, coef_index]) + '),'),
			for coef_index in range(4, 6):
				print(q_format_str + '(' +str(-sos[section_index, coef_index]) + '),'),
			print
		print("},")

		z, p, k = butter_lowpass(cut_off, rate, order = order)

		sos = signal.zpk2sos(z, p, k)
		max_coef = np.amax(abs(sos))
		q_format_str = 'Q'+str(31-int(np.ceil(np.log2(max_coef))))

		print("{"),
		for section_index in range(sos.shape[0]):
			for coef_index in range(3):
				print(q_format_str + '(' +str(sos[section_index, coef_index]) + '),'),
			for coef_index in range(4, 6):
				print(q_format_str + '(' +str(-sos[section_index, coef_index]) + '),'),
			print
		print("},")


	print("};")

