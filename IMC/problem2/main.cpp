#include <string>

#include "CompPlayer.h"
#include "RealPlayer.h"

// utility function to check for various cases of rock-paper-scissor
bool isRealPlayerWinner (Choice choiceR, Choice choiceC) {
  switch(choiceR) {
    case PAPER:
      if (choiceC == ROCK) return true;
      else return false;
      // equivalence of the 2 values has already been checked
    case ROCK:
      if (choiceC == SCISSORS) return true;
      else return false;
    case SCISSORS:
      if (choiceC == PAPER) return true;
      else return false;
    default:
      break;
  }

  // base case to make the compiler happy
  return true;
}

int main() {
  CompPlayer comp;
  RealPlayer realP;

  while (true) {
    Choice realPlayersChoice = realP.play();
    Choice computersChoice = comp.play();

    std::cout << "computer chooses " << ChoiceDesc[computersChoice] << std::endl;

    if (computersChoice == realPlayersChoice) {
      std::cout << "Tie!" << std::endl;
    } else {
      if (isRealPlayerWinner(realPlayersChoice, computersChoice)) {
        std::cout << "You Win! :)" << std::endl;
      } else {
        std::cout << "Computer Wins! :-/" << std::endl;
      }
    }

    std::cout << "\n";
  }

  return 1;
}
