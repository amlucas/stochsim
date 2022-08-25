import unittest
import pystochsim as ssm

class TestProblem(unittest.TestCase):

    def test_problem_fails_wrong_species(self):
        p = ssm.Problem(10, {"A": 100, "B": 10})
        with self.assertRaises(RuntimeError) as context:
            p.add_reaction(0.1, "A+B->C")

    def test_problem_fails_wrong_format(self):
        p = ssm.Problem(10, {"A": 100, "B": 10})
        with self.assertRaises(RuntimeError) as context:
            p.add_reaction(0.1, "A+->C")
