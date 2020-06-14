import argparse
import asyncio
import itertools
import logging
from collections import defaultdict
import random
from typing import Dict, NewType, List

import numpy as np

from Trains.utils import rotate_list, random_combination

random.seed(42)


UNIT_LOAD_TIME = 0.2
UNIT_UNLOAD_TIME = 0.15
DEFAULT_TRAIN_SPEED = 0.5
MAX_CAPACITY = 40

StationId = NewType('StationId', int)
TrainId = NewType('TrainId', int)


class Track:
    def __init__(self, a: StationId, b: StationId, length: int):
        self.a = a
        self.b = b
        self.length = length
        self.lock = asyncio.Lock()


class Train:
    def __init__(self, number: TrainId, capacity: int, speed: float = DEFAULT_TRAIN_SPEED):
        self.id = number
        self.name = f"Train [{self.id}]  "
        self.capacity = capacity
        self.cargo: Dict[StationId, int] = defaultdict(int)
        self.speed = speed

    @property
    def free_capacity(self):
        return self.capacity - sum(self.cargo.values(), 0)

    async def load_cargo(self, destination: StationId, share: int):
        await asyncio.sleep(int(np.ceil(share * UNIT_LOAD_TIME)))
        logging.info(f"{self.name} : loaded {share} units cargo destined to station {destination}")
        self.cargo[destination] += share
        return share

    async def unload_cargo(self, station: StationId):
        cargo = self.cargo[station]

        if cargo == 0:
            return 0

        await asyncio.sleep(int(np.ceil(cargo * UNIT_UNLOAD_TIME)))

        self.cargo[station] = 0
        return cargo

    async def move(self, track: Track):
        await track.lock.acquire()
        logging.info(f"{self.name} : moving on track {track.a} -> {track.b}")
        await asyncio.sleep(self.speed * track.length)
        track.lock.release()


class Station:
    def __init__(self, number: StationId):
        self.id = number
        self.name = f"Station [{self.id}]"
        self.cargo: Dict[StationId, int] = defaultdict(int)

    def drop_cargo(self, destination: StationId, amount: int):
        self.cargo[destination] += amount
        logging.info(f"{self.name} : villager dropped {amount} units of cargo destined to station {destination}")

    async def arrive(self, train: Train):
        logging.info(f"{self.name} : train {train.id} arrived")

        received = await train.unload_cargo(self.id)
        if received != 0:
            logging.info(f"{self.name} : received {received} units cargo delivered by train {train.id}")

        for dst in [k for k, v in self.cargo.items() if v != 0]:
            share = self.cargo[dst]
            share = min(train.free_capacity, share)

            if share == 0:
                break

            self.cargo[dst] -= share
            await train.load_cargo(dst, share)


async def move_train(train: Train, tracks: List[Track], stations: Dict[StationId, Station]):
    for track in itertools.cycle(tracks):
        await train.move(track)
        await stations[track.b].arrive(train)


async def villager_dropping_cargo(stations: Dict[StationId, Station]):
    while True:
        await asyncio.sleep(random.randint(1, 60))
        a, b = random_combination(stations.keys(), 2)
        cargo = random.randint(1, MAX_CAPACITY)
        stations[a].drop_cargo(b, cargo)


async def simulate(stations: Dict[StationId, Station], trains: List[Train]):
    tracks = []
    for a, b in zip(stations, rotate_list(stations, -1)):
        tracks.append(Track(a=a, b=b, length=random.randint(1, 10)))

    tasks = [villager_dropping_cargo(stations)]
    for t in trains:
        tasks.append(move_train(t, rotate_list(tracks, random.randint(-1 * len(tracks), 1 * len(tracks))), stations))

    await asyncio.gather(*tasks)
    return


def main():
    logging.basicConfig(level=logging.INFO, format='%(asctime)s %(message)s', datefmt='%H:%M:%S.%U')

    parser = argparse.ArgumentParser()
    parser.add_argument("--num-trains", type=int, required=True)
    parser.add_argument("--num-stations", type=int, required=True)

    args = parser.parse_args()

    if args.num_stations < 2:
        raise RuntimeError("num-stations must be >= 2")

    if args.num_trains < 1:
        raise RuntimeError("num-trains must be >= 1")

    stations = {}
    for i in range(args.num_stations):
        stations[StationId(i)] = Station(StationId(i))

    trains = []
    for i in range(args.num_trains):
        trains.append(Train(TrainId(i), capacity=random.randint(1, MAX_CAPACITY)))

    try:
        asyncio.run(simulate(stations, trains))
    except KeyboardInterrupt:
        logging.info("simulation complete")
        return


if __name__ == "__main__":
    main()
