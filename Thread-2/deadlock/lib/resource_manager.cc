#include <mutex>
#include <thread>
#include <chrono>
#include <condition_variable>
#include "resource_manager.h"

namespace proj2 {

int ResourceManager::request(RESOURCE r, int amount) {
    if (amount <= 0)  return 1;

    std::unique_lock<std::mutex> lk(this->resource_mutex[r]);
    while (true) {
        if (this->resource_cv[r].wait_for(
            lk, std::chrono::milliseconds(100),
            [this, r, amount] { return this->resource_amount[r] >= amount; }
        )) {
            // prevent here
            auto this_id = std::this_thread::get_id();
            bool enough = true;
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
                if (can_request != this_id) {
                    if (can_request != -1) {  
                        continue;
                    } else {
                        can_request = this_id;
                    }
                }
                break;
            } else {
                // sleep
                if (can_request == this_id) {
                    can_request = -1;
                }
                continue;
            }
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
    this->resource_amount[r] -= amount;
    this->required_amount[this_id][r] -= amount;
    this->resource_mutex[r].unlock();
    return 0;
}

void ResourceManager::release(RESOURCE r, int amount) {
    if (amount <= 0)  return;
    std::unique_lock<std::mutex> lk(this->resource_mutex[r]);
    this->resource_amount[r] += amount;
    this->resource_cv[r].notify_all();
}

void ResourceManager::budget_claim(std::map<RESOURCE, int> budget) {
    // This function is called when some workload starts.
    // The workload will eventually consume all resources it claims
    auto this_id = std::this_thread::get_id();// read thread id
    this->required_amount.insert(std::map<int, std::map<RESOURCE, int>>::value_type(this_id, budget));
}

} // namespace: proj2
