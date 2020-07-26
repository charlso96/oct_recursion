from mpl_toolkits.mplot3d import Axes3D
from argparse import ArgumentParser
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np
from scipy.interpolate import griddata

def main():
    i = []
    j = []
    v = []
    parser = ArgumentParser()
    parser.add_argument("dataset", type=str,help="The input file.")
    args = parser.parse_args()
    filename = args.dataset

    # with open(filename, "r") as file:
    #     for line in file:
    #         list = line.split()
    #         i.append(float(list[0]))
    #         j.append(float(list[1]))
    #         v.append(float(list[2]))
    #
    fig = plt.figure()
    ax = fig.add_subplot(111,projection = '3d')

    ax.view_init(35,65)
    ax.set_xlabel('i')
    ax.set_ylabel('j')

    x,y,z = np.loadtxt(filename, unpack=True)
    xi = np.linspace(-1,1,1000)
    yi = np.linspace(-1,1,1000)
    Z = griddata((x,y),z,(xi[None,:],yi[:,None]), method = 'cubic')
    plt.contourf(xi,yi,Z)
    #ax.scatter(x,y,z, c='r',marker = 'o', s=0.003)
    plt.savefig('fig.png')

if __name__ == '__main__':
    main()
