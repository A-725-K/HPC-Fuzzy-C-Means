import sys
import matplotlib.pyplot as plt

PREFIX = 'perftime_'
SUFFIX = '_xHost'
SCHED  = ['static', 'dynamic', 'guided']


def draw_comparison(data):
    xx = list(range(1, 11))
   
    plt.figure()
    plt.grid()
    plt.xlabel('#execution')
    plt.ylabel('Execution time [s]')
    plt.xticks(xx)
    #avgs = list(map(lambda l: float('{:.3f}'.format(sum(l)/len(l))), data.values()))
    #plt.yticks(avgs)
    plt.title('Comparison of different scheduling types -xHost')
    for sched, ts in data.items():
        plt.plot(xx, ts, label=sched)
    plt.legend(shadow=True, fancybox=True, title='Scheduling:')
    plt.show()


def process_file(pf):
    with open(pf, 'r') as f:
        content = f.read()
        data = list(map(lambda x: float(x), content.split('\n')[:-1]))
        return data, max(data), min(data), sum(data)/len(data)


def main(par_dir):
    data = {}
    maxs = {}
    mins = {}
    avgs = {}
    
    for sched in SCHED:
        data[sched], maxs[sched], mins[sched], avgs[sched] = process_file(par_dir + PREFIX + sched + SUFFIX)
    draw_comparison(data)

    for sched in SCHED:
        print('*** SCHEDULING STRATEGY: {} ***'.format(sched.title()))
        print('BEST TIME  : {:.3f} s'.format(mins[sched]))
        print('WORST TIME : {:.3f} s'.format(maxs[sched]))
        print('AVG TIME   : {:.3f} s\n'.format(avgs[sched]))


if __name__ == '__main__':
    if len(sys.argv) != 2:
        print('Usage: python3 plot_scheduling.py <parallel_dir>')
        exit(1)

    main(sys.argv[1])
