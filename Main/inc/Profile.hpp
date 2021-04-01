#ifndef _PROFILE_HPP_
#define _PROFILE_HPP_

class Boundry {
  public: 
	char16_t upper, lower;
};

extern Boundry profile[];

extern int currentProfile;


Boundry defineBoundry(int upper, int lower);

void incrementProfile();


#endif // _PROFILE_HPP_