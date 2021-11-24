#include <mutex>
#include <thread>
#include <chrono>
#include <algorithm>
#include <unistd.h>
#include <iostream>
#include <condition_variable>
#include "resource_manager.h"

namespace proj2 {

int ResourceManager::request(RESOURCE r, int amount) {
    if (amount <= 0)  return 1;

    // std::unique_lock<std::mutex> lk(this->resource_mutex[r]);//已经对resource_mutex[r]上锁了
    while (true) {
        if (
			true
			// this->resource_cv[r].wait_for(
            // lk, std::chrono::milliseconds(100),
            // [this, r, amount] { 
	    // return this->resource_amount[r] >= amount; 
	    // return true;
	    // })
        ) {
	    // std::cout << "trying" << std::endl;
            // prevent here
            auto this_id = std::this_thread::get_id();
            std::cout << this_id << std::endl;
	    if (can_request.size() != 0) {
		std::cout << can_request[0];
	    }
	    // unsigned int this_id = *(unsigned int*)&this_id_1;//把std::thread::id转换为int,不一定对
            bool enough = true;
            mtx->lock();
            if (std::find(can_request.begin(), can_request.end(), this_id) != can_request.end()) {
                mtx->unlock();
	    std::cout << "trying1" << std::endl;
                break;
            }
            std::map<RESOURCE, int> res = this->required_amount[this_id];
            std::map <RESOURCE, int>::iterator it;
            it = res.begin();
            while (it != res.end()) {
            // whether enough
                if (it->second > this->resource_amount[it->first]) {
                    enough = false;
                }
            it++;
            }
            if (enough) {
                can_request.push_back(this_id);
                it = res.begin();
                while (it != res.end()) {
                    this->resource_amount[it->first] -= it->second;
                it++;
                }
		mtx->unlock();
	    std::cout << "trying2" << std::endl;
                break;
            }
            mtx->unlock();
	    // std::cout << "trying3" << std::endl;
            usleep(100000); 
            continue;
        } else {
            auto this_id = std::this_thread::get_id();
            /* HINT: If you choose to detect the deadlock and recover,
                     implement your code here to kill and restart threads.
                     Note that you should release this thread's resources
                     properly.
             */
            if (tmgr->is_killed(this_id)) {
                return -1;
            }
        }
    }
    // this->resource_amount[r] -= amount;
    //增加了两句，不然没定义this_id
    // auto this_id_1 = std::this_thread::get_id();
    // unsigned int this_id = *(unsigned int*)&this_id_1;
    // this->required_amount[this_id][r] -= amount;

    // this->resource_mutex[r].unlock();
    return 0;
}

void ResourceManager::release(RESOURCE r, int amount) {
    if (amount <= 0)  return;
    std::cout << "release" << std::endl;
    // std::unique_lock<std::mutex> lk(this->resource_mutex[r]);
    mtx->lock();
    this->resource_amount[r] += amount;
    this->resource_cv[r].notify_all();
    mtx->unlock();
    // this->running -= 1;
    // if (this->running == 0) {
    //     can_request = -1;
    // }
}

void ResourceManager::budget_claim(std::map<RESOURCE, int> budget) {
    // This function is called when some workload starts.
    // The workload will eventually consume all resources it claims
    auto this_id = std::this_thread::get_id();// read thread id
    // unsigned int this_id = *(unsigned int*)&this_id_1;//把std::thread::id转换为int,不一定对
    this->required_amount.insert(std::map<std::thread::id, std::map<RESOURCE, int>>::value_type(this_id, budget));
}

} // namespace: proj2
