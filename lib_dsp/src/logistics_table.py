import numpy as np
import matplotlib.pyplot as plt

def logistics(x):
    return 1.0/(1.0 + np.exp(-x))

last = 0.5
errors = [0.0,
          0.02,
          0.04,
          0.05,
          0.05,
          0.05,
          0.05,
          0.05,
          0.05]
slopes = []
offsets = []
for i in range(1,9):
    value = float(i)
    end = logistics(value)
    if i != 8:
        error = errors[i] * (1-end)
        end += error
    else:
        error = 1.0 - end
        end = 1.0
    slope = end - last
    midpointerror = logistics(value + 0.5) - (end + slope/2.0)
    print error,last, slope, midpointerror
    offsets.append(int((1<<24) * (last-slope * (i-1))))
    slopes.append(int((1<<32) * slope))
    last = end

out = '    .word '
for i in slopes:
    out += str(i) + ','

out += '\n    .word '
for i in offsets:
    out += str(i) + ','

print out

pts = []
for i in range(0,100):
    value = float(i)/10.0
    inte = int(value)
    correct = logistics(value)
    if inte < 8:
        frac = value - inte
        y = offsets[inte]/float(1<<24) + slopes[inte]/float(1<<32) * value
        pts.append((correct,y))
    else:
        pts.append((correct,1.0))
    
plt.figure()
plt.plot(pts)
plt.grid(True)
plt.title('a')
plt.show()
