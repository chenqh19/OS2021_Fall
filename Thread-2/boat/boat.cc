#include <thread>
#include <vector>
#include <unistd.h>
#include <windows.h>


#include "boat.h"
 
namespace proj2{

void Boat::AdultInitialize(BoatGrader *bg){
    
    bg->initializeAdult();
    adults_on_oahu += 1;
    Sleep(100);
    std::unique_lock<std::mutex> lk(m);
    cv1.wait(lk);
    bg->AdultRowToMolokai();
    adults_on_oahu -= 1;
    adults_on_molokai += 1;
    boat_on_oahu = false;
    if(children_on_oahu == 0 && adults_on_oahu == 0)
    {
        finished = true;
    }
    else{
        cv2.notify_one();
    }
    

}

void Boat::ChildInitialize(BoatGrader *bg){
    
    bg->initializeChild();
    children_on_oahu +=1;
    Sleep(100);
    bool current_on_oahu = true;
    while(!finished){
        if(current_on_oahu){
            std::unique_lock<std::mutex> lk(m);
            cv3.wait(lk);
            if(boat_on_oahu) {
                if(children_on_oahu >= 2 ){
                    children_on_oahu -= 1;
                    if(children_on_boat == 0){
                        children_on_boat = 1;
                        bg->ChildRowToMolokai();
                        cv3.notify_one();
                    }
                    else{
                        children_on_boat = 0;
                        bg->ChildRideToMolokai();
                        boat_on_oahu = false;
                        cv2.notify_one();
                    }
                    children_on_molokai += 1;
                    current_on_oahu = false;
                }
                else if(children_on_boat == 1 ){
                        children_on_oahu -= 1;
                        children_on_molokai += 1;
                        current_on_oahu = false;
                        children_on_boat = 0;
                        bg->ChildRideToMolokai();
                        boat_on_oahu = false;
                        if(adults_on_oahu == 0){
                            finished = true;       
                        }
                        else{
                            cv2.notify_one();
                        }
                            
                }
                else if(children_on_boat == 0){
                        children_on_oahu -= 1;
                        children_on_molokai += 1;
                        current_on_oahu = false;
                        bg->ChildRideToMolokai();
                        boat_on_oahu = false;
                        finished = true;
                        if(adults_on_oahu == 0){
                            finished = true;       
                        }
                        else{
                            cv2.notify_one();
                        }
                }
            }  
        }
        else{
            std::unique_lock<std::mutex> lk(m);
            cv2.wait(lk);
            if(!boat_on_oahu){
                children_on_molokai -= 1;
                bg->ChildRowToOahu();
                children_on_oahu += 1;
                current_on_oahu = true;
                boat_on_oahu = true;
                if(children_on_oahu <= 1 && adults_on_oahu >= 1){
                    cv1.notify_one();
                    //std::cout<<std::this_thread::get_id()<<std::endl;
                }
                else{
                    cv3.notify_one();
                }
            }
        }
    }
    
}
	
Boat::Boat(){
}

void Boat:: begin(int a, int b, BoatGrader *bg){
    finished = false;
    boat_on_oahu = true;
    children_on_oahu = 0;
    adults_on_oahu = 0;
    children_on_molokai = 0;
    adults_on_molokai = 0;
    std::vector<std::thread*> people;
    for(int i = 0 ; i < a; i++){
        people.push_back(new std::thread(&proj2::Boat::AdultInitialize, this, bg));
    }
    for(int i = 0 ; i < b; i++){
        people.push_back(new std::thread(&proj2::Boat::ChildInitialize, this, bg));
    }
    Sleep(1000);
    cv3.notify_one();
    Sleep(1000);
    
} 
}