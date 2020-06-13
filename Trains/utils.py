import random
from collections import deque
from typing import Iterable, List, Any


def rotate_list(lst: Iterable, by: int) -> List[Any]:
    d = deque(lst)
    d.rotate(by)
    return list(d)


def random_combination(iterable, r):
    # Random selection from itertools.combinations(iterable, r)
    pool = tuple(iterable)
    n = len(pool)
    indices = sorted(random.sample(range(n), r))
    return tuple(pool[i] for i in indices)