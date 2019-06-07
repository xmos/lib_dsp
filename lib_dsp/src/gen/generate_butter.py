# Copyright (c) 2019, XMOS Ltd, All rights reserved
import numpy as np
import matplotlib.pyplot as plt
from scipy import signal


def butter_highpass(cutoff, fs, order):
    nyq = 0.5 * fs
    normal_cutoff = cutoff / nyq
    z, p, k = signal.butter(order, normal_cutoff, output = 'zpk', btype='high', analog=False)
    return z, p, k

rate = 16000
hpf_cut_off = 100.

z, p, k= butter_highpass(hpf_cut_off, rate, order = 5)

show = False
if show:
	w, h = signal.freqz_zpk(z, p, k, fs=rate)
	plt.plot(w,20*np.log10(abs(h)))
	plt.show()

sos = signal.zpk2sos(z, p, k)
max_coef = np.amax(abs(sos))

q = int(np.ceil(np.log2(max_coef)))

# q_format_str = 'Q'+ str(q) + '_' +str(30-q)
q_format_str = 'Q'+str(31-q)


for section_index in range(sos.shape[0]):
	for coef_index in range(sos.shape[1]):
		if coef_index != 3:
			print(q_format_str + '(' +str(sos[section_index, coef_index]) + '),'),
	print



np.random.seed(1)

x = np.random.uniform(-.1,.1,128)
i = np.asarray(x*np.iinfo(np.int32).max,dtype=np.int32)
y = signal.sosfilt(sos, x)
# print x
# print i
# print y

for j in i:
	print str(j) + ',',
print



