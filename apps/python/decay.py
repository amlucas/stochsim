#!/usr/bin/env python3

import matplotlib.pyplot as plt
import pystochsim as ssm

tend = 50
problem = ssm.Problem(tend,
                      {"A": 100, "B": 0})

problem.add_reaction(0.1, "A -> B")

solver = ssm.Solver.SSA(problem)

fig, ax = plt.subplots()

num_trajectories = 200

for i in range(num_trajectories):
    time, states = ssm.run_trajectory(problem, solver)

    plt.gca().set_prop_cycle(None)

    for key, vals in states.items():
        ax.plot(time, vals, label=key if i == 0 else None, alpha=1/num_trajectories**0.5)

ax.set_xlabel(r"$t$")
ax.set_ylabel(r"$N$")
ax.set_xlim(0, tend)
ax.set_ylim(0, )

leg = ax.legend()
for lh in leg.legendHandles:
    lh.set_alpha(1)

plt.show()
