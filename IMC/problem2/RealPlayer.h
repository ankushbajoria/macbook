#ifndef _REALPLAYER_H_
#define _REALPLAYER_H_

#include "Choice.h"

#include <iostream>
#include <string>

/**
 * author: abajoria
 *
 * description: this class simulates the behavior of a real player
 * by getting a user input and returning a choice
 *
 * returns NUM_CHOICES if user input is "exit"
 **/

class RealPlayer {
  private:
    std::string m_helpMsg;
    bool sanityCheck(std::string input) {
      for (uint8_t i = 0; i < NUM_CHOICE; i++) {
        if (std::string(ChoiceDesc[i]) == input) {
          return true;
        }
      }

      if (input == "exit") {
        return true;
      }

      return false;
    }
  
  public:
    RealPlayer() {
      m_helpMsg = "Valid choices:\n1. ROCK\n2. PAPER\n3. SCISSORS\n4. exit\n";
    }

    Choice play() {
      std::string input = "";

      std::cout << m_helpMsg << std::endl;
      std::cout << "choice? : ";
      std::cin >> input;

      while (!sanityCheck(input)) {
        std::cout << "invalid choice!\n\n" << m_helpMsg << std::endl; 
        std::cout << "choice? : ";
        
        std::cin >> input;
      }
      
      if (input == "exit") {
        exit(0);
      }

      for (uint8_t i = 0; i < NUM_CHOICE; i++) {
        if (ChoiceDesc[i] == input) { return ((Choice) i); }
      }

      return NUM_CHOICE;
    }
};

#endif // _REALPLAYER_H_
