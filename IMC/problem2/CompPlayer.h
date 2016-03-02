#ifndef _COMPPLAYER_H_
#define _COMPPLAYER_H_

#include <iostream>
#include <time.h>

#include "Choice.h"

/**
 * author: abajoria
 * date : 20140620
 **/

class CompPlayer {
  public:
    CompPlayer();

    Choice play();    
};

CompPlayer::CompPlayer() {
  srand(time(NULL)); // to ensure that we do not get repeatable results
}

Choice CompPlayer::play() {
  return ((Choice) (rand() % 3));
}

#endif // _COMPPLAYER_H_
