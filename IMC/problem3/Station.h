#pragma once

#include "Cargo.h"
#include "StdoutLoggerSingleton.h"

#include <iostream>
#include <pthread.h>
#include <sstream>
#include <unistd.h>
#include <vector>

/**
 * author : abajoria
 * date : 20140621
 *
 **/

class Station {
  private:
    static const uint32_t LOADING_TIME   = 2000000;
    static const uint32_t UNLOADING_TIME = 1500000;

    uint32_t m_id;
    uint32_t m_numStations;
    pthread_mutex_t m_lockOnTrack ;
    pthread_mutex_t m_lock ; 

    uint32_t getNextStationId() {
      return ((m_id + 1 ) % m_numStations);
    }

  public:
    Station(uint32_t id, uint32_t numStations)
    : m_id(id), 
      m_numStations(numStations)
    {
      pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
      m_lockOnTrack = lock;
      m_lock = lock;
    }

    void moveToNextStation(uint32_t trainId, uint32_t timeTaken);

    void loadCargo(uint32_t trainId, uint32_t capacity, Cargo* cargo);

    void unloadCargo(uint32_t trainId, Cargo* cargo);

};

void Station::moveToNextStation(uint32_t trainId, uint32_t timeTaken) {
  pthread_mutex_lock( & m_lockOnTrack );
  sleep(timeTaken);

  // print to stdout
  std::stringstream stream;
  stream << "Train " << trainId << " has reached Station " << getNextStationId() << std::endl;
  StdoutLoggerSingleton::getStdoutLogger()->write(stream.str());

  pthread_mutex_unlock( & m_lockOnTrack );
}

void Station::loadCargo(uint32_t trainId, uint32_t capacity, Cargo* cargo) {
  pthread_mutex_lock(& m_lock);
  uint32_t num_cargo_to_load = std::min(capacity, cargo->cargoSize);
  cargo->cargoSize = num_cargo_to_load;

  uint32_t time_to_load = num_cargo_to_load * LOADING_TIME;
  pthread_mutex_unlock(& m_lock);

  // This is outside any lock, to ensure multiple trains to load at same time
  usleep(time_to_load);

  pthread_mutex_lock(& m_lock); 
  std::stringstream stream;
  stream << "Cargo loaded to train " << trainId << " at " << cargo->srcStationId
          << " for " << cargo->destStationId << " with " << cargo->cargoSize << std::endl;
  StdoutLoggerSingleton::getStdoutLogger()->write(stream.str());
  pthread_mutex_unlock(& m_lock);
}

void Station::unloadCargo(uint32_t trainId, Cargo* cargo) {
  pthread_mutex_lock(& m_lock);
  uint32_t time_to_unload = UNLOADING_TIME * cargo->cargoSize;
  cargo->cargoSize = 0;
  pthread_mutex_unlock(& m_lock);

  // This is outside any lock, to ensure multiple trains to unload at the same time
  usleep(time_to_unload);

  pthread_mutex_lock(& m_lock); 
  std::stringstream stream;
  stream << "Cargo unloaded by train " << trainId << " at " << cargo->destStationId << std::endl;
  StdoutLoggerSingleton::getStdoutLogger()->write(stream.str());    
  pthread_mutex_unlock(& m_lock);
}

typedef std::vector<Station*> StationList;
