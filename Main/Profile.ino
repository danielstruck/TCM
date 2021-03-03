//#include "../inc/Profile.hpp"

typedef struct {
  char upper, lower;
} Boundry;

struct Boundry defineBoundry(char upper, char lower) {
  Boundry b;
  b.upper = upper;
  b.lower = lower;
  return b;
}

static Boundry profile[] = {
  defineBoundry(-54,72);
  defineBoundry(-40, 72);
  defineBoundry(-10, 56);
  defineBoundry(10, 32);
};

static char currentProfile = 0;

void incrementProfile() {
  currentProfile = (++currentProfile) % (sizeof(profile) / sizeof(Boundry));
}

void selectProfile() {
	// TODO
}
