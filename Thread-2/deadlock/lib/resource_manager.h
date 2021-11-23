#ifndef DEADLOCK_LIB_RESOURCE_MANAGER_H_
#define DEADLOCK_LIB_RESOURCE_MANAGER_H_

#include <map>
#include <vector>
#include <mutex>
#include <thread>
#include <condition_variable>
#include "thread_manager.h"

namespace proj2 {

enum RESOURCE {
    GPU = 0,
    MEMORY,
    DISK,
    NETWORK
};

class ResourceManager {
public:
    ResourceManager(ThreadManager *t, std::map<RESOURCE, int> init_count): \
        resource_amount(init_count), tmgr(t) {}
    void budget_claim(std::map<RESOURCE, int> budget);
    int request(RESOURCE, int amount);
    void release(RESOURCE, int amount);
private:
    std::map<RESOURCE, int> resource_amount;
    std::map<RESOURCE, std::mutex> resource_mutex;
    std::map<RESOURCE, std::condition_variable> resource_cv;
    std::map<int, std::map<RESOURCE, int>> required_amount;
    ThreadManager *tmgr;
    int can_request ; // which process is requesting
    int running; // how many requests are allowed to run but not finished
    
};

}  // namespce: proj2

#endif