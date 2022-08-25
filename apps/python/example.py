#!/usr/bin/env python3

import matplotlib.pyplot as plt
import pyssm as ssm

tend = 100
problem = ssm.Problem(tend,
                      {"A": 1000, "B": 0, "C": 0})

problem.add_reaction(0.05, "A -> 2B")
problem.add_reaction(0.09, "B -> C")
problem.add_reaction(0.001, "B + C -> A")

solver = ssm.Solver.SSA(problem)

time, states = ssm.run_trajectory(problem, solver)


fig, ax = plt.subplots()
for key, vals in states.items():
    ax.plot(time, vals, label=key)
ax.set_xlabel(r"$t$")
ax.set_ylabel(r"$N$")
ax.set_xlim(0, tend)
ax.set_ylim(0, )
ax.legend()
plt.show()
