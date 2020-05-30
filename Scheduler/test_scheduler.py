import itertools
import unittest

import networkx as nx

from Scheduler.scheduler import Machine, Colo, allocate


class MyTestCase(unittest.TestCase):
    machines = [Machine("m1", "a", 5), Machine("m2", "a", 9), Machine("m3", "b", 1)]
    colos = {"a": Colo('a', {"a": 0, 'b': 100}, machines[:2]), "b": Colo('b', {'a': 100, "b": 0}, machines[2:])}
    G = nx.Graph()

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        for c1, c2 in itertools.permutations(self.colos.keys(), 2):
            self.G.add_edge(c1, c2)

    def test_machine_available(self):
        self.assertEqual(self.machines[0].available, 5)
        self.assertEqual(self.machines[1].available, 1)
        self.assertEqual(self.machines[2].available, 9)

    def test_colo_capacity(self):
        self.assertEqual(self.colos['a'].capacity, 6)
        self.assertEqual(self.colos['b'].capacity, 9)

    def test_allocate(self):
        allocation = allocate(self.G, self.colos, 7)
        self.assertDictEqual({f'{k!r}': v for k, v in allocation.items()}, {'m3': 7})


if __name__ == '__main__':
    unittest.main()
