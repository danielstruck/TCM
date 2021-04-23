#ifndef _PROFILE_HPP_
#define _PROFILE_HPP_

class Boundry {
  public: 
	char upper, lower;
	
	Boundry(int lower, int upper) {
	  this->upper = upper;
	  this->lower = lower;
	}
};

extern Boundry profile[];
extern char16_t currentProfile;


void incrementProfile();


#endif // _PROFILE_HPP_
