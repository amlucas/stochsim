#!/usr/bin/env python3

import argparse
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("csv", type=str, help="csv file to plot.")
    args = parser.parse_args()

    df = pd.read_csv(args.csv)
    time = df["time"].to_numpy()

    fig, ax = plt.subplots()

    for key in df.columns:
        if key == "time":
            continue
        x = df[key].to_numpy()
        ax.plot(time, x, label=key)

    ax.set_xlabel(r"$t$")
    ax.set_ylabel(r"$x$")
    ax.legend()
    plt.show()
