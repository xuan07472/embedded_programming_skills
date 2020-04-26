#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt

def show_data(data_file):
    data = np.fromfile(data_file, dtype=np.int16)
    plt.plot(data)
    plt.show()

if __name__ == '__main__':
    import sys

    if len(sys.argv) < 2:
        print('useage: %s xxx.DAT' % str(sys.argv[0]))
        sys.exit()

    show_data(str(sys.argv[1]))
