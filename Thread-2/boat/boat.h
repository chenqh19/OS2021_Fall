#ifndef BOAT_H_
#define BOAT_H_

#include<stdio.h>
#include <thread>
#include <mutex>
#include<condition_variable>
#include <unistd.h>

#include "boatGrader.h"

namespace proj2{
class Boat{
	
public:
	Boat();
    ~Boat(){};
	void begin(int, int, BoatGrader*);
	void AdultInitialize(BoatGrader*);
    void ChildInitialize(BoatGrader*);
private:	
	std::mutex m;
	std::condition_variable cv1,cv2,cv3,cv4;
	int children_on_oahu;
    int adults_on_oahu;
    int children_on_molokai;
    int adults_on_molokai;
	bool finished;
	bool boat_on_oahu;
	int children_on_boat;
	

};
}
#endif // BOAT_H_
