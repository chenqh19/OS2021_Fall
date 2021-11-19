#ifndef BOAT_H_
#define BOAT_H_

#include <stdio.h>
#include <thread>
#include <mutex>
#include <unistd.h>

#include "boatGrader.h"

namespace proj2{
class Boat{
public:
	Boat();
    ~Boat(){};
	void begin(int, int, BoatGrader*);
private:
	std::mutex mtx;
	std::mutex bt;
	int position;
};
}

#endif // BOAT_H_
