#!/usr/bin/env python3

import argparse
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("csv", type=str, help="csv file to plot.")
    parser.add_argument("--show-only", type=str, nargs='+', default=None, help="if set, show only these variables.")
    parser.add_argument("--out", type=str, default=None, help="if set, save the figure to a file with this name.")
    args = parser.parse_args()

    to_show = args.show_only

    df = pd.read_csv(args.csv)
    time = df["time"].to_numpy()

    fig, ax = plt.subplots()

    for key in df.columns:
        if key == "time":
            continue

        if to_show is None or key in to_show:
            x = df[key].to_numpy()
            ax.plot(time, x, label=key)

    ax.set_xlabel(r"$t$")
    ax.set_ylabel(r"$x$")
    ax.set_xlim(0,time[-1])
    ax.set_ylim(0,)
    ax.legend()

    if args.out is None:
        plt.show()
    else:
        plt.savefig(args.out)
