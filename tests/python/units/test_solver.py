import unittest
import pystochsim as ssm

class TestSolver(unittest.TestCase):

    def test_decays_species_goes_to_zero(self):
        p = ssm.Problem(100, {"A": 10, "B": 0})
        p.add_reaction(1000, "A->2B")
        s = ssm.Solver.SSA(p)
        t, state = ssm.run_trajectory(solver=s, problem=p)

        self.assertEqual(state["A"][-1], 0)
        self.assertEqual(state["B"][-1], 20)
