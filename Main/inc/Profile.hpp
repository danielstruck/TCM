#ifndef _PROFILE_HPP_
#define _PROFILE_HPP_

struct Boundry {
  char upper, lower;
};

extern Boundry profile[];

extern char currentProfile;


struct Boundry defineBoundry(char upper, char lower);

void incrementProfile();

void selectProfile();


#endif // _PROFILE_HPP_