#include "Train.h"

void* run(void* trainPtr);

//pthread_mutex_t track1 = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t track2 = PTHREAD_MUTEX_INITIALIZER;

int main() {
  // initialize random seed
  srand(time(NULL));

  // Setup stations
  static const uint32_t NUM_STATIONS = 8;
  StationList stationList;
  for (uint32_t i = 0ul; i < NUM_STATIONS; i++) {
    Station* station = new Station(i, NUM_STATIONS);
    stationList.push_back(station);
  }

  // Setup Trains
  // Train(id, speed, initialStation, cargoCapacity, stationList)
  Train t1(1, 4, 0, 10, stationList);
  Train t2(2, 3, 1, 8, stationList);
  Train t3(3, 2, 2, 12, stationList);
  Train t4(4, 1, 3, 5, stationList);

  pthread_t thread1, thread2, thread3, thread4;
  int iret1, iret2, iret3, iret4;

  iret1 = pthread_create( &thread1, NULL, run, &t1);
  iret2 = pthread_create( &thread2, NULL, run, &t2);
  iret3 = pthread_create( &thread3, NULL, run, &t3);
  iret4 = pthread_create( &thread4, NULL, run, &t4);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  pthread_join(thread3, NULL);
  pthread_join(thread4, NULL);

  return 1;
}

void* run(void* trainPtr) {
  Train* t1 = reinterpret_cast<Train*>(trainPtr);

  t1->run();

  return NULL;
}
