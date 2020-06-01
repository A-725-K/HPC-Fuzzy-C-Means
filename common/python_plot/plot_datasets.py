import sys
import time
import matplotlib.pyplot as plt


def plotDataset(X, Y, dims):
    dims_splitted = []
    for i in range(dims):
        dims_splitted.append([x[i] for x in X])

    plt.figure()
    plt.grid()
    plt.scatter(dims_splitted[0], dims_splitted[1], c=Y, cmap='jet', linewidths=[0.0001 for _ in range(len(X))])
    plt.show()
    

def main(filename):
    X = []
    Y = []
    dims = -1

    print('Reading dataset...', end='')
    idx = 1
    with open(filename, 'r') as f:
        line = f.readline()
        while line:
            fields = line.split(' ')
            if dims < 0:
                dims = len(fields) - 1
            Xi = [0 for _ in range(dims)]
            for i in range(dims):
                Xi[i] = float(fields[i])
            X.append(Xi)
            Y.append(int(fields[dims]))
            
            if idx % 50 == 0:
                print('.', end='')

            idx += 1
            line = f.readline()
        print()

    plotDataset(X, Y, dims)


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print('Usage: python3 draw_dataset.py <dataset_path>')
        exit(1)
    
    main(sys.argv[1])

