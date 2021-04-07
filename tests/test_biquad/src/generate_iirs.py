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

header = "iir.h"
impl = "iir.xc"

max_biquad_order = 0

for order in orders:
	for cut_off in cut_offs:
		z, p, k = butter_highpass(cut_off, rate, order = order)
		sos = signal.zpk2sos(z, p, k)
		max_biquad_order = max(max_biquad_order, sos.shape[0])
		z, p, k = butter_lowpass(cut_off, rate, order = order)
		sos = signal.zpk2sos(z, p, k)
		max_biquad_order = max(max_biquad_order, sos.shape[0])

header_file = open(header, "w") 
impl_file = open(impl,"w") 
header_file.write("#include \"dsp.h\"\n")
impl_file.write("#include \"iir.h\"\n")
header_file.write("#define IIR_ORDERS "+str(len(orders))+'\n')
header_file.write("#define MAX_BIQUAD_ORDER "+str(max_biquad_order)+'\n')
header_file.write("#define IIRS_PER_ORDER "+str(len(cut_offs)*2)+'\n')
header_file.write("extern unsigned biquad_orders[IIR_ORDERS][IIRS_PER_ORDER];\n")


header_file.write("extern int32_t filter[IIR_ORDERS][IIRS_PER_ORDER][DSP_NUM_COEFFS_PER_BIQUAD*MAX_BIQUAD_ORDER];\n")
header_file.write("extern unsigned q_factors[IIR_ORDERS][IIRS_PER_ORDER];\n")


biquad_orders_str =("unsigned biquad_orders[IIR_ORDERS][IIRS_PER_ORDER]={")

filer_str = "int32_t filter[IIR_ORDERS][IIRS_PER_ORDER][DSP_NUM_COEFFS_PER_BIQUAD*MAX_BIQUAD_ORDER] = {\n"
q_factor_str = "unsigned q_factors[IIR_ORDERS][IIRS_PER_ORDER] = {"


for order in orders:
	filer_str += ("{\n")
	q_factor_str += ("{\n")
	biquad_orders_str += ("{\n")
	for cut_off in cut_offs:


		z, p, k = butter_highpass(cut_off, rate, order = order)
		n=30
		sos = signal.zpk2sos(z, p, k)
		max_coef = np.amax(abs(sos))
		q_factor = n-int(np.ceil(np.log2(max_coef)))
		q_format_str = 'Q'+str(q_factor)
		q_factor_str += (str(q_factor) +  ", ")
		filer_str += ("{\n")
		biquad_orders_str += str(sos.shape[0]) + ", "
		for section_index in range(sos.shape[0]):
			for coef_index in range(3):
				filer_str += (q_format_str + '(' +str(sos[section_index, coef_index]) + '),')
			for coef_index in range(4, 6):
				filer_str += (q_format_str + '(' +str(-sos[section_index, coef_index]) + '),')
		for section_index in range(sos.shape[0], max_biquad_order):
			for coef_index in range(5):
				filer_str += ('0.0,')
		filer_str += ("},\n")

		filer_str += ("// ")
		one =np.int64(1<<(q_factor+1)) + 1
		for section_index in range(sos.shape[0]):
			for coef_index in range(3):
				filer_str += ('(' +str(int(one*sos[section_index, coef_index])>>1) + '),')
			for coef_index in range(4, 6):
				filer_str +=  ('(' +str(int(-one*sos[section_index, coef_index])>>1) + '),')
		for section_index in range(sos.shape[0], max_biquad_order):
			for coef_index in range(5):
				filer_str += ('0,')
		filer_str += ("\n")

		z, p, k = butter_lowpass(cut_off, rate, order = order)

		sos = signal.zpk2sos(z, p, k)
		biquad_orders_str += str(sos.shape[0]) + ", "
		max_coef = np.amax(abs(sos))
		q_factor = n-int(np.ceil(np.log2(max_coef)))
		q_format_str = 'Q'+str(q_factor)
		q_factor_str += (str(q_factor) +  ", ")

		filer_str += ("{\n")
		for section_index in range(sos.shape[0]):
			for coef_index in range(3):
				filer_str += (q_format_str + '(' +str(sos[section_index, coef_index]) + '),')
			for coef_index in range(4, 6):
				filer_str += (q_format_str + '(' +str(-sos[section_index, coef_index]) + '),')
			filer_str += ("\n")
		for section_index in range(sos.shape[0], max_biquad_order):
			for coef_index in range(5):
				filer_str += ('0.0,')
		filer_str += ("},\n")

		print (abs(z),abs(p))

		q_sos = np.zeros(sos.shape)
		one =np.int64(1<<(q_factor+1)) + 1
		for section_index in range(sos.shape[0]):
			for coef_index in range(sos.shape[1]):
				q_sos[section_index, coef_index]= np.float64(int(one*sos[section_index, coef_index])>>1) / np.float64(np.int64(1<<(q_factor)))
			try:
				z,p,k= signal.sos2zpk(q_sos)
				print (abs(z),abs(p))
				print "good"
			except:
				print "bad"

	filer_str += ("},\n")
	q_factor_str += ("},\n")
	biquad_orders_str += ("},\n")

filer_str += "};\n"
q_factor_str += "};\n"
biquad_orders_str+= "};\n"

impl_file.write(filer_str)
impl_file.write(biquad_orders_str)
impl_file.write(q_factor_str)
