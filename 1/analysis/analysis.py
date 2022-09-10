import pandas as pd
import matplotlib.pyplot as plt


def plotTime():
    df = pd.read_csv('time.csv')
    df.plot(kind='line', x='sorting_algorithm', y='10000', color='blue')
    df.plot(kind='line', x='sorting_algorithm',
            y='100000', color='green')
    plt.ylabel("time(seconds)")
    plt.xlabel("sorting algorithm")
    plt.title('Time Analysis')
    plt.show()


def plotFuncCalls():
    df = pd.read_csv('function_calls.csv')
    ax = plt.gca()
    df.plot(kind='line', x='sorting_algorithm', y='10000', color='blue', ax=ax)
    df.plot(kind='line', x='sorting_algorithm',
            y='100000', color='green', ax=ax)

    plt.title('Time Analysis')
    plt.show()


def plotHeap():
    x = [10000, 100000]
    y = [0.051, 0.841]
    default_x_ticks = range(len(x))
    plt.plot(default_x_ticks, y)
    plt.xticks(default_x_ticks, x)
    plt.xlabel("N")
    plt.ylabel("time in seconds")
    plt.title("Heap sort")
    plt.show()


def plotInsertion():
    x = [10000, 100000]
    y = [5.042, 552.690]
    default_x_ticks = range(len(x))
    plt.plot(default_x_ticks, y)
    plt.xticks(default_x_ticks, x)
    plt.xlabel("N")
    plt.ylabel("time in seconds")
    plt.title("Insertion sort")
    plt.show()


if __name__ == '__main__':
    plotTime()
    # plotFuncCalls()
    # plotHeap()
    # plotInsertion()
