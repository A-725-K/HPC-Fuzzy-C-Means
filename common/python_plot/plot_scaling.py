import sys
import matplotlib.pyplot as plt


def read_strong(strong_ds):
    times = {}

    with open(strong_ds, 'r') as f:
        line = f.readline()
        while line:
            nproc = int(line)
            t = 0.0
            for _ in range(3):
                t += float(f.readline())
            times[nproc] = t / 3
            line = f.readline() 

    return times


def read_weak(weak_ds):
    times = {}

    with open(weak_ds, 'r') as f:
        line = f.readline()
        while line:
            sl = line.split()
            nproc = int(sl[0])
            sz = int(sl[1])
            t = 0.0
            for _ in range(3):
                t += float(f.readline())
            times[nproc] = t / 3
            line = f.readline()

    return times


def plot_strong(times):
    xx = [x for x in times.keys()]
    yy = [y for y in times.values()]

    plt.figure(1)
    plt.grid()
    plt.xlabel('Number of processors')
    plt.ylabel('Execution time [s]')
    plt.title('STRONG scaling')
    plt.xticks(xx, [str(x) for x in xx])
    plt.plot(xx, yy, 'r')
    plt.show()


def plot_speedup(times, seq_t):
    xx = [x for x in times.keys()]
    yy = [seq_t/float(y) for y in times.values()]
    
    plt.figure(2)
    plt.grid()
    plt.xlabel('Number of processors')
    plt.ylabel('Speedup')
    plt.title('STRONG scaling: Speedup')
    plt.xticks(xx, [str(x) for x in xx])
    plt.plot(xx, yy, 'g')
    plt.show()
    
    return yy


def plot_efficiency(sp, rs):
    xx = [int(x) for x in rs]
    yy = [s/xx[i] for i, s in enumerate(sp)]

    plt.figure(3)
    plt.grid()
    plt.xlabel('Number of processors')
    plt.ylabel('Efficiency')
    plt.title('STRONG scaling: Efficiency')
    plt.xticks(xx, [str(x) for x in xx])
    plt.plot(xx, yy, 'b')
    plt.show()


def plot_weak(weak_ds):
    times = read_weak(weak_ds)

    xx = [int(x) for x in times.keys()]
    yy = [y for y in times.values()]
    
    plt.figure(3)
    plt.grid()
    plt.xlabel('Number of processors')
    plt.ylabel('Execution time [s]')
    plt.title('WEAK scaling: 1000 point per processor')
    plt.xticks(xx, [str(x) for x in xx])
    plt.plot(xx, yy, 'y')
    plt.show()


def main(seq_ds, strong_ds, weak_ds):
    seq_data = None
    with open(seq_ds, 'r') as f:
        seq_data = f.read()
        seq_t = sum([float(x) for x in seq_data.split('\n')[:-1]]) / 10

    rs = read_strong(strong_ds)

    plot_strong(rs)
    sp = plot_speedup(rs, seq_t)
    plot_efficiency(sp, rs.keys())
    plot_weak(weak_ds)


if __name__ == "__main__":
    if len(sys.argv) != 4:
        print('Usage: python3 plot_scaling.py <sequential_path> <strong_scaling_path> <weak_scaling_path>')
        exit(1)
    
    main(sys.argv[1], sys.argv[2], sys.argv[3])

