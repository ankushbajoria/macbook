import random
from typing import Any

from RockPaperScissors.interfaces import Player
from enum import Enum, auto

from RockPaperScissors.utils import PrettyPrintEnumMeta


class InvalidChoiceException(Exception):
    pass


class Choice(Enum, metaclass=PrettyPrintEnumMeta):
    ROCK = auto()
    PAPER = auto()
    SCISSORS = auto()

    @staticmethod
    def from_int(val: int):
        if val == 0:
            return Choice.ROCK
        elif val == 1:
            return Choice.PAPER
        elif val == 2:
            return Choice.SCISSORS

        raise InvalidChoiceException()


class ManualPlayer(Player):
    def __init__(self, name: str, fp: Any):
        super(ManualPlayer, self).__init__()
        self.__name = name
        self.__fp = fp

    def name(self):
        return self.__name

    def turn(self) -> Choice:
        try:
            choice = self.__fp.readline().strip()
            return Choice[choice]
        except Exception:
            raise


class AutomatedPlayer(Player):
    def __init__(self, name: str):
        super(AutomatedPlayer, self).__init__()
        self.__name = name

    def name(self) -> str:
        return self.__name

    def turn(self) -> Choice:
        val = random.randint(0, 2)
        return Choice.from_int(val)


if __name__ == "__main__":
    pass
