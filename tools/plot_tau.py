#!/usr/bin/env python3

import argparse
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("csv", type=str, help="csv file to plot.")
    parser.add_argument("--out", type=str, default=None, help="if set, save the figure to a file with this name.")
    args = parser.parse_args()

    df = pd.read_csv(args.csv)
    time = df["time"].to_numpy()
    tau = df["tau"].to_numpy()

    fig, ax = plt.subplots()
    ax.plot(time, tau, '.k')
    ax.set_xlabel(r"$t$")
    ax.set_ylabel(r"$\tau$")
    ax.set_xlim(0,time[-1])
    ax.set_ylim(0,)

    if args.out is None:
        plt.show()
    else:
        plt.savefig(args.out)
