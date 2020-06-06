import argparse
import datetime
import logging
import random

from RockPaperScissors.game import RockPaperScissors, Compare
from RockPaperScissors.players import ManualPlayer, AutomatedPlayer, Choice

random.seed(datetime.datetime.now().timestamp())


class PromptInput():
    def __init__(self):
        pass

    @staticmethod
    def readline() -> str:
        print(f"\n{Choice}")
        result = input("your input: ")
        return result


def result_to_string(player1: str, player2: str, result: Compare):
    if result == Compare.WINS:
        result_str = "wins against"
    elif result == Compare.LOSES:
        result_str = "loses to"
    else:
        result_str = "ties with"

    return f"{player1} {result_str} {player2}"


def main():
    logging.basicConfig(level=logging.INFO, format='%(asctime)s %(message)s')

    parser = argparse.ArgumentParser()
    parser.add_argument('-n', "--number-of-turns", type=int, required=True)
    args = parser.parse_args()

    fp_proxy = PromptInput()

    player1 = ManualPlayer("human", fp_proxy)
    player2 = AutomatedPlayer("computer")

    game = RockPaperScissors("game", player1, player2)

    for i in range(0, args.number_of_turns):
        result = game.play()
        print(result_to_string(player1.name(), player2.name(), result))


if __name__ == "__main__":
    main()
