#ifndef _TRAIN_H_
#define _TRAIN_H_

#include "Station.h"

#include <algorithm>
#include <ctime>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <vector>

/**
 * author: abajoria
 * date:   20140621
 *
 * description: this class abstracts out the behavior of a train
 **/

class Train {
  public:
    Train(uint32_t trainId, 
          uint32_t speed,
          uint32_t begin,
          uint32_t cargoCapacity,
          StationList m_stationList) ;

    void run();

  private:
    void updateStationId();

    uint32_t m_trainId;
    uint32_t m_speed;
    uint32_t m_stationId;
    uint32_t m_cargoCapacity;
    Cargo    m_cargo;
    StationList m_stationList;
};

Train::Train(uint32_t trainId, uint32_t speed, uint32_t begin, 
                 uint32_t capacity, StationList stationList)
  : m_trainId(trainId),
    m_speed(speed), 
    m_stationId(begin),
    m_cargoCapacity(capacity)
{
  m_stationList.resize(stationList.size());
  std::copy(stationList.begin(), stationList.end(), m_stationList.begin());
}

void Train::updateStationId() {
  uint32_t num_tracks = m_stationList.size();

  m_stationId = (m_stationId + 1) % num_tracks;
}

void Train::run() {
  while (true) {
    // load cargo
    // Here, I carry cargo from a single src at a time
    if ((rand() % 3) == 0 && m_cargo.cargoSize == 0) {
      uint32_t cargoSize = (rand()%15) + 1;
      uint32_t destId = rand() % m_stationList.size();
      destId = (destId == m_stationId) ? destId + 1 : destId;

      m_cargo.cargoSize = cargoSize;
      m_cargo.srcStationId = m_stationId;
      m_cargo.destStationId = destId;

      // loading from the station to the train
      m_stationList[m_stationId]->loadCargo(m_trainId, m_cargoCapacity, &m_cargo);
    }

    std::stringstream stream;
    stream << "Train " << m_trainId << " departing from " << m_stationId << std::endl;
    StdoutLoggerSingleton::getStdoutLogger()->write(stream.str());

    // move to next station
    m_stationList[m_stationId]->moveToNextStation(m_trainId, m_speed);
    updateStationId();

    // unload cargo
    if (m_cargo.destStationId == m_stationId) {
      m_stationList[m_stationId]->unloadCargo(m_trainId, &m_cargo);
    }
  }
}

#endif // _TRAIN_H_
