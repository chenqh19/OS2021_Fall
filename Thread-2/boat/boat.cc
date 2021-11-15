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