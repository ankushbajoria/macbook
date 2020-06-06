import logging
from enum import Enum, auto
from typing import List

from RockPaperScissors.interfaces import Game, Player
from RockPaperScissors.players import Choice
from RockPaperScissors.utils import PrettyPrintEnumMeta


class Compare(Enum, metaclass=PrettyPrintEnumMeta):
    WINS = auto()
    LOSES = auto()
    IS_TIED = auto()

    @staticmethod
    def choices(choice_1: Choice, choice_2: Choice):
        to_int = {Choice.ROCK: 0, Choice.PAPER: 1, Choice.SCISSORS: 2}

        result_matrix = [[Compare.IS_TIED, Compare.LOSES, Compare.WINS],
                         [Compare.WINS, Compare.IS_TIED, Compare.LOSES],
                         [Compare.LOSES, Compare.WINS, Compare.IS_TIED]]

        return result_matrix[to_int[choice_1]][to_int[choice_2]]


class RockPaperScissors(Game):
    def __init__(self, name: str, player1: Player, player2: Player):
        super(RockPaperScissors, self).__init__()
        self.__name = name
        self.player1 = player1
        self.player2 = player2

    def name(self):
        return self.__name

    def play(self):
        choice_1 = self.player1.turn()
        choice_2 = self.player2.turn()

        logging.info(f"{self.player1.name()} chooses {str(choice_1)}")
        logging.info(f"{self.player2.name()} chooses {str(choice_2)}")

        return Compare.choices(choice_1, choice_2)
