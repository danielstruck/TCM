#include "inc/Profile.hpp"


Boundry profile[] = {
  defineBoundry(-54,72),
  defineBoundry(-40, 72),
  defineBoundry(-10, 56),
  defineBoundry(10, 32)
};

char16_t currentProfile = 0;

Boundry defineBoundry(int lower, int upper) {
  Boundry b;
  b.upper = upper;
  b.lower = lower;
  return b;
}

void incrementProfile() {
  currentProfile = (++currentProfile) % (sizeof(profile) / sizeof(Boundry));
}
