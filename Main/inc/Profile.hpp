#ifndef _PROFILE_HPP_
#define _PROFILE_HPP_

typedef struct boundry Boundry;

extern Boundry profile[];

extern int currentProfile;


Boundry defineBoundry(int upper, int lower);

void incrementProfile();


#endif // _PROFILE_HPP_