#include <thread>
#include <vector>
#include <unistd.h>

#include "boat.h"

namespace proj2{
	
Boat::Boat(){
}

void Boat:: begin(int a, int b, BoatGrader *bg){
    // count the number of children and adults
    bool where_boat = false;
    while (true) {
        if (where_boat == false) {
            if (childrenLeft >= 2) {
                ChildRideToMolokai();
                where_boat = true;
            } else if (adultsLeft > 0) {
                AdultRideToMolokai();
                where_boat = true;
            }
        } else {
            if ( && passed) {
                return;
            } else {
                ChildRowToOahu();
                where_boat = false;
            }
        }
    }
} 
}

void Boat::Child() {


    mtx.lock();
    if (position == 1 || (position == 2 && childrenLeft >= 2)) {
        position -= 1;
        ChildRideToMolokai();
    } else if (adultsLeft == 0) {
        ChildRowToMolokai();
    }
    mtx.unlock();
}

void Boat::Adult() {
    mtx.lock();
    if (position == 2 && childrenLeft < 2) {
        position -= 2;
        AdultRideToMolokai();
    }
    mtx.unlock();
}