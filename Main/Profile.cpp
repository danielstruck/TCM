#include "inc/Profile.hpp"


// Default profiles, can be expanded to include more profiles
Boundry profile[] = {
  Boundry(-54,72),
  Boundry(-40, 72),
  Boundry(-10, 56),
  Boundry(10, 32)
};

// keeps track of the current profile
char16_t currentProfile = 0;

// Increments and wraps the current profile
void incrementProfile() {
  currentProfile = (++currentProfile) % (sizeof(profile) / sizeof(Boundry));
}
