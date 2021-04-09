#ifndef _PROFILE_HPP_
#define _PROFILE_HPP_

class Boundry {
  public: 
	char upper, lower;
};

extern Boundry profile[];

extern char16_t currentProfile;


Boundry defineBoundry(int upper, int lower);

void incrementProfile();


#endif // _PROFILE_HPP_
