import argparse
import itertools
from pathlib import Path
from typing import List, Dict

import networkx as nx
import numpy as np
import pandas as pd


class Machine:
    max_load_factor = 10

    def __init__(self, name, colo, load_factor):
        self.name = name
        self.colo = colo
        self.load_factor = load_factor

    def __str__(self):
        return f"'Machine {self.name} at colo {self.colo} with load_factor {self.load_factor}'"

    def __repr__(self):
        return self.name

    @property
    def available(self):
        return self.max_load_factor - self.load_factor


class Colo:
    def __init__(self, name: str, latency: Dict[str, int], machines: List[Machine]):
        self.name = name
        self.latency = latency
        self.machines: List[Machine] = sorted(machines, key=lambda x: x.available, reverse=True)

    def __str__(self):
        latency_str = ", ".join([f"{k}:{v}" for k, v in self.latency.items()])
        return f"'colo {self.name}, latency: {latency_str}'"

    def __repr__(self):
        return self.name

    @property
    def capacity(self):
        return sum([x.available for x in self.machines])

    def distance(self, other: str):
        return self.latency.get(other, np.inf)


class NoAllocationPossibleError(Exception):
    pass


def allocate(graph: nx.Graph, colos: Dict[str, Colo], workload: int) -> Dict[Machine, int]:
    optimal_network_distance = np.inf
    optimal_traversal_list = []

    for c in colos:
        workload_cp = workload
        traversal_list = []
        network_distance = 0

        for node in nx.bfs_tree(graph, c, depth_limit=1):  # every colo is ideally connected to every other colo
            workload_cp -= colos[node].capacity
            traversal_list.append(node)
            network_distance += colos[c].distance(node)

            if workload_cp <= 0:
                break

        if workload_cp <= 0 and optimal_network_distance > network_distance:
            optimal_traversal_list = traversal_list
            optimal_network_distance = network_distance

        if optimal_network_distance == 0:
            # cannot get better than this, lets break the loop and return
            break

    allocation = {}
    for c in optimal_traversal_list:
        for m in colos[c].machines:
            allocation[m] = min(workload, m.available)
            workload -= allocation[m]

            if workload == 0:
                return allocation

    raise NoAllocationPossibleError


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--machine-info", type=str, required=True,  help="machine -> [colo, load_factor]")
    parser.add_argument("--intercolo", type=str, required=True, help="intercolo distance csv")

    args = parser.parse_args()

    assert Path(args.machine_info).exists()
    assert Path(args.intercolo).exists()

    machine_info = pd.read_csv(Path(args.machine_info), index_col="machine")
    intercolo = pd.read_csv(args.intercolo, index_col="colo").to_dict()

    machines = [Machine(name, row["colo"], row["load"]) for name, row in machine_info.iterrows()]
    machines.sort(key=lambda x: x.colo)
    colos = {name: Colo(name, latency, [x for x in machines if x.colo == name]) for name, latency in intercolo.items()}
    graph = nx.Graph()

    for c1, c2 in itertools.permutations(colos.keys(), 2):
        graph.add_edge(c1, c2, weight=colos[c2].capacity / colos[c1].distance(c2))

    while True:
        try:
            workload = int(input("enter required workload: "))
            allocated_machines = allocate(graph, colos, workload)
            print(allocated_machines)
        except EOFError:
            break
        except ValueError:
            continue
        except NoAllocationPossibleError:
            print("no allocation possible")
            continue


if __name__ == "__main__":
    main()
