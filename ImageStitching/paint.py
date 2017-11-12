import numpy as np
import matplotlib.pyplot as plt

a1 = np.loadtxt('audio0.txt')/5e7
a2 = np.loadtxt('audio1.txt')/5e7
a3 = np.loadtxt('audio2.txt')/5e7
a4 = np.loadtxt('audio3.txt')/5e7
plt.figure()
plt.plot(range(len(a1)), a1, 'r')
plt.plot(range(len(a2)), a2, 'b')
plt.plot(range(len(a3)), a3, 'y')
plt.plot(range(len(a4)), a4, 'g')
plt.show()