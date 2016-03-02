#pragma once

#include <stdint.h>

struct Cargo {
  uint32_t cargoSize;
  uint32_t destStationId;
  uint32_t srcStationId;

  Cargo(uint32_t sz, uint32_t src, uint32_t dst) {
    cargoSize = sz;
    src = srcStationId;
    dst = destStationId;
  }

  Cargo() {
    cargoSize = 0;
    srcStationId = 0;
    destStationId = 0;
  }
};
