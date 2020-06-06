import io
import unittest

from RockPaperScissors.game import Compare, RockPaperScissors
from RockPaperScissors.players import Choice, ManualPlayer


class MyTestCase(unittest.TestCase):
    def test_compare(self):
        self.assertEqual(Compare.IS_TIED, Compare.choices(Choice.ROCK, Choice.ROCK))
        self.assertEqual(Compare.IS_TIED, Compare.choices(Choice.PAPER, Choice.PAPER))
        self.assertEqual(Compare.IS_TIED, Compare.choices(Choice.SCISSORS, Choice.SCISSORS))

        self.assertEqual(Compare.WINS, Compare.choices(Choice.ROCK, Choice.SCISSORS))
        self.assertEqual(Compare.WINS, Compare.choices(Choice.PAPER, Choice.ROCK))
        self.assertEqual(Compare.WINS, Compare.choices(Choice.SCISSORS, Choice.PAPER))

        self.assertEqual(Compare.LOSES, Compare.choices(Choice.SCISSORS, Choice.ROCK))
        self.assertEqual(Compare.LOSES, Compare.choices(Choice.ROCK, Choice.PAPER))
        self.assertEqual(Compare.LOSES, Compare.choices(Choice.PAPER, Choice.SCISSORS))

    def test_game(self):
        player1 = ManualPlayer("player1", io.StringIO("ROCK"))
        player2 = ManualPlayer("player2", io.StringIO("PAPER"))
        game = RockPaperScissors("test", player1, player2)

        self.assertEqual(game.name(), "test")
        self.assertEqual(game.play(), Compare.LOSES)


if __name__ == '__main__':
    unittest.main()
