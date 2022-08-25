#!/usr/bin/env python3

import matplotlib.pyplot as plt
import pystochsim as ssm

tend = 2000
problem = ssm.Problem(tend,
                      {"PLac": 1,
                       "RNAP": 35,
	               "Ribosome": 350,
	               "PLacRNAP": 0,
	               "TrLacZ1": 0,
	               "TrLacZ2": 0,
	               "RbsLacZ": 0,
	               "TrLacY1": 0,
	               "TrLacY2": 0,
	               "RbsLacY": 0,
	               "RbsRibosomeLacZ": 0,
	               "RbsRibosomeLacY": 0,
	               "TrRbsLacY": 0,
	               "TrRbsLacZ": 0,
	               "LacY": 0,
	               "LacZ": 0,
	               "dgrLacY": 0,
	               "dgrLacZ": 0,
	               "dgrRbsLacY": 0,
	               "dgrRbsLacZ": 0,
	               "lactose": 0,
	               "product": 0,
	               "LacZlactose": 0})

problem.add_reaction(0.17,    "PLac + RNAP -> PLacRNAP")
problem.add_reaction(10,      "PLacRNAP -> PLac + RNAP")
problem.add_reaction(1,       "PLacRNAP -> TrLacZ1")
problem.add_reaction(1,       "TrLacZ1 -> RbsLacZ + PLac + TrLacZ2")
problem.add_reaction(0.015,   "TrLacZ2 -> TrLacY1")
problem.add_reaction(1,       "TrLacY1 -> RbsLacY + TrLacY2")
problem.add_reaction(0.36,    "TrLacY2 -> RNAP")
problem.add_reaction(0.17,    "Ribosome + RbsLacZ -> RbsRibosomeLacZ")
problem.add_reaction(0.17,    "Ribosome + RbsLacY -> RbsRibosomeLacY")
problem.add_reaction(0.45,    "RbsRibosomeLacZ -> Ribosome + RbsLacZ")
problem.add_reaction(0.45,    "RbsRibosomeLacY -> Ribosome + RbsLacY")
problem.add_reaction(0.4,     "RbsRibosomeLacZ -> TrRbsLacZ + RbsLacZ")
problem.add_reaction(0.4,     "RbsRibosomeLacY -> TrRbsLacY + RbsLacY")
problem.add_reaction(0.015,   "TrRbsLacZ -> LacZ")
problem.add_reaction(0.036,   "TrRbsLacY -> LacY")
problem.add_reaction(6.42e-5, "LacZ -> dgrLacZ")
problem.add_reaction(6.42e-5, "LacY -> dgrLacY")
problem.add_reaction(0.3,     "RbsLacZ -> dgrRbsLacZ")
problem.add_reaction(0.3,     "RbsLacY -> dgrRbsLacY")
problem.add_reaction(9.52e-5, "LacZ + lactose -> LacZlactose")
problem.add_reaction(431,     "LacZlactose -> product + LacZ")
problem.add_reaction(14,      "LacY -> lactose + LacY")

solver = ssm.Solver.TauLeaping(problem)

fig, ax = plt.subplots()

num_trajectories = 5

for i in range(num_trajectories):
    time, states = ssm.run_trajectory(problem, solver, min_dt_dump=5)
    plt.gca().set_prop_cycle(None)
    for name in ["LacY", "LacZ"]:
        ax.plot(time, states[name],
                label=name if i == 0 else None,
                alpha=1/num_trajectories**0.5)

ax.set_xlabel(r"$t$")
ax.set_ylabel(r"$N$")
ax.set_xlim(0, tend)
ax.set_ylim(0, )

leg = ax.legend()
for lh in leg.legendHandles:
    lh.set_alpha(1)

plt.show()
