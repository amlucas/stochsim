#!/usr/bin/env python3

import matplotlib.pyplot as plt
import pystochsim as ssm

tend = 14
problem = ssm.Problem(tend,
                      {"[X1]": 50000,
                       "[X2]": 500,
                       "Y1": 1000,
                       "Y2": 2000,
                       "Z1": 0,
                       "Z2": 0})

problem.add_reaction(0.1, "[X1] -> Y1")
problem.add_reaction(0.1, "[X2] + Y1 -> Y2 + Z1")
problem.add_reaction(0.00005, "2Y1 + Y2 -> 3Y1")
problem.add_reaction(5, "Y1 -> Z2")

solver = ssm.Solver.TauLeaping(problem)

fig, ax = plt.subplots()

time, states = ssm.run_trajectory(problem, solver)

ax.plot(time, states["Y1"], label=r"$Y_1$")
ax.plot(time, states["Y2"], label=r"$Y_2$")

ax.set_xlabel(r"$t$")
ax.set_ylabel(r"$N$")
ax.set_xlim(0, tend)
ax.set_ylim(0, )

leg = ax.legend()

plt.show()
