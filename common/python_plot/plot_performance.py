import sys
import matplotlib.pyplot as plt

RES_DIR = 'results'

# COMMON = 'common'
MPI    = 'mpi'
PAR    = 'parallel'
#SEQ   = 'sequential'

PREFIX = 'perftime'
OPT    = ['O0', 'O2', 'Ofast', 'xHost']
SCHED  = ['static', 'dynamic', 'guided']


def put_label(rects, ax):
    for rect in rects:
        height = rect.get_height()
        ax.annotate('{0:.3f} s'.format(height),
                    xy=(rect.get_x() + rect.get_width() / 2, height),
                    xytext=(0, 3),
                    textcoords='offset points',
                    ha='center', va='bottom')


def process_file(res_file):
    with open(res_file, 'r') as f:
        content = f.read()
        data = list(map(lambda x: float(x), content.split('\n')[:-1]))
        avg = sum(data)/len(data)
        return avg


def draw_barchart(times, title, color):
    xx = ['-' + x for x in times.keys()]
    yy = [float(y) for y in times.values()]


    fig, ax = plt.subplots()
    width = 0.5
    
    # create bars with labels
    rects = []
    for x, y in zip(xx, yy):
        rects += [ax.bar(x, y, width=width, color=color)]
    for r in rects:
        put_label(r, ax)

    ax.set_title(title)
    ax.set_xlabel('Compiler optimization flags')
    ax.set_ylabel('Execution time [s]')
    fig.tight_layout()
    plt.show()


def mkdir(d):
    return '/'.join([RES_DIR, d]) + '/'


def main():
    # parallel
    par_dir = mkdir(PAR)
    par_colors = ['g', '#ff00cd', 'b']
    for idx, sched in enumerate(SCHED):
        times = {}
        for opt in OPT:
            curr_file = par_dir + '_'.join([PREFIX, sched, opt])
            times[opt] = process_file(curr_file)
        draw_barchart(times, 'OpenMP parallel version, scheduling: ' + sched.title(), par_colors[idx])

    mpi_dir = mkdir(MPI)
    mpi_color = '#f60'
    times = {}
    for opt in OPT:
        curr_file = mpi_dir + '_'.join([PREFIX, MPI, opt])
        times[opt] = process_file(curr_file)
    draw_barchart(times, 'OpenMPI distributed version, #processors = 12', mpi_color)


if __name__ == '__main__':
    main()

