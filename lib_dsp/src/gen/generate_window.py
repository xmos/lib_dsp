import numpy
from numpy import log10, abs, pi
import scipy
from scipy import signal
import matplotlib
import matplotlib.pyplot
import matplotlib as mpl
import sys, getopt
import math

def main(argv):
   outputfile = ''
   fn = ''
   win_len = ''
   try:
      opts, args = getopt.getopt(sys.argv[1:],"o:f:l:",["output", "function", "length"])
   except getopt.GetoptError:
      print 'generate_window.py -o <outputfile> -w function -l length'
      print 'supported functions: boxcar'
      print '                     triangular'
      print '                     hanning'
      print '                     hamming'
      print '                     blackman'
      print '                     nuttall'
      print '                     blackman-nuttall'
      print '                     blackman-harris'
      sys.exit(2)
   for opt, arg in opts:
      if opt == '-h':
         print 'generate_window.py -o <outputfile> -w function -l length'
         print 'supported functions: boxcar'
         print '                     triangular'
         print '                     hanning'
         print '                     hamming'
         print '                     blackman'
         print '                     nuttall'
         print '                     blackman-nuttall'
         print '                     blackman-harris'
         sys.exit()
      elif opt in ("-o", "--output"):
         outputfile = arg
      elif opt in ("-f", "--function"):
         fn = arg
      elif opt in ("-l", "--length"):
         win_len = arg

   f = open(outputfile,'w')
   N = int(win_len)

   print 'Will generate file:', outputfile
   print 'With function: ', fn
   print 'Window length is ', N, ' '
   print 'LUT length is ', (N/2), ' '
   pi = 3.14159265434
   #check that N is even
   f.write('const int window[%d] = {\n' % (N/2))
   for n in range(0, N/2):
      val = 1.0
      if fn == 'boxcar':
         val = 1.0
      elif fn == 'triangular':
         L = N
         val = 1.0 - abs((n - (N-1.0)/2.0)/(L/2.0))
      elif fn == 'hanning':
         alpha = 0.5
         beta  =  1.0-alpha
         val = alpha - beta*math.cos(2*pi*n/(N-1))
      elif fn == 'hamming':
         alpha = 0.53836
         beta  = 1.0-alpha
         val = alpha - beta*math.cos(2*pi*n/(N-1))
      elif fn == 'blackman':
         alpha  = 0.16
         a0  = (1.0-alpha)/2.0
         a1  = 0.5
         a2  = alpha/2.0
         val = a0 - a1*math.cos(2*pi*n/(N-1)) + a2*math.cos(4*pi*n/(N-1))
      elif fn == 'nuttall':
         a0  = 0.355768
         a1  = 0.487396
         a2  = 0.144232
         a3  = 0.012604
         val = a0 - a1*math.cos(2*pi*n/(N-1)) + a2*math.cos(4*pi*n/(N-1))- a3*math.cos(6*pi*n/(N-1))
      elif fn == 'blackman-nuttall':
         a0  = 0.3635819
         a1  = 0.4891775
         a2  = 0.1365995
         a3  = 0.0106411
         val = a0 - a1*math.cos(2*pi*n/(N-1)) + a2*math.cos(4*pi*n/(N-1))- a3*math.cos(6*pi*n/(N-1))
      elif fn == 'blackman-harris':
         a0  = 0.35875
         a1  = 0.48829
         a2  = 0.14128
         a3  = 0.01168
         val = a0 - a1*math.cos(2*pi*n/(N-1)) + a2*math.cos(4*pi*n/(N-1))- a3*math.cos(6*pi*n/(N-1))
      f.write('%d,\n' % int(val*0x7fffffff));
   f.write( '};\n')

   f.close();

 
if __name__ == "__main__":
   main(sys.argv[1:])
