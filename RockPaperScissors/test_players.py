import io
import unittest
import random

from RockPaperScissors.players import ManualPlayer, Choice, AutomatedPlayer

random.seed(42)


class MyTestCase(unittest.TestCase):
    def test_manual_player(self):
        fp = io.StringIO()
        fp.writelines("ROCK")
        fp.flush()
        fp.seek(0)

        player = ManualPlayer("human", fp)

        self.assertEqual(player.name(), "human")
        self.assertEqual(player.turn(), Choice.ROCK)

    def test_automated_player(self):
        player = AutomatedPlayer("computer")
        self.assertEqual(player.name(), "computer")
        self.assertEqual(player.turn(), Choice.SCISSORS)

    def test_choices(self):
        self.assertEqual(Choice.ROCK, Choice["ROCK"])
        self.assertEqual(str(Choice), "choices are: [ROCK, PAPER, SCISSORS]")


if __name__ == '__main__':
    unittest.main()
