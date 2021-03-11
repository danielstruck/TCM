#include "inc/Profile.hpp"


Boundry profile[] = {
  defineBoundry(-54,72),
  defineBoundry(-40, 72),
  defineBoundry(-10, 56),
  defineBoundry(10, 32)
};

char currentProfile = 0;

struct Boundry{
  char upper, lower;
};

struct Boundry defineBoundry(char upper, char lower) {
  Boundry b;
  b.upper = upper;
  b.lower = lower;
  return b;
}

void incrementProfile() {
  currentProfile = (++currentProfile) % (sizeof(profile) / sizeof(Boundry));
}

void selectProfile() {
	// TODO
 
}
