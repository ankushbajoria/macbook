from abc import ABC, abstractmethod


class Player(ABC):
    @abstractmethod
    def name(self):
        pass

    @abstractmethod
    def turn(self):
        pass


class Game(ABC):
    @abstractmethod
    def name(self):
        pass

    @abstractmethod
    def play(self):
        pass
