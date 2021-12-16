#ifndef MEMORY_MANAGER_H_
#define MEMORY_MANAGER_H_

#include <assert.h>
#include <map>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <time.h>
#include <vector>
#include <iostream>
#include <mutex>
#include <algorithm>
#include <unistd.h>

#define PageSize 1024

namespace proj3 {

class PageFrame {
public:
    PageFrame();
    int& operator[] (unsigned long);
    void WriteDisk(std::string);
    void ReadDisk(std::string);
    int ReadContent(int offset){return mem[offset];}
    void WriteContent(int offset, int value){mem[offset] = value;}
private:
    int mem[PageSize];
};

class PageInfo {
public:
    PageInfo();
    void SetInfo(int,int);
    void ClearInfo();
    int GetHolder();
    int GetVid();
private:
    int holder; //page holder id (array_id)
    int virtual_page_id; // page virtual #
    /*add your extra states here freely for implementation*/

};

class ArrayList;

class MemoryManager {
public:
    // you should not modify the public interfaces used in tests
    MemoryManager(size_t);
    int ReadPage(int array_id, int virtual_page_id, int offset);
    void WritePage(int array_id, int virtual_page_id, int offset, int value);
    ArrayList* Allocate(size_t);
    void Release(ArrayList*);
    ~MemoryManager();
    bool FIFO = false;
private:
    std::map<int, std::map<int, int>> page_map; // // mapping from ArrayList's virtual page # to physical page #
    PageFrame** mem; // physical pages, using 'PageFrame* mem' is also acceptable 
    PageInfo* page_info; // physical page info
    bool* free_list;  // use bitmap implementation to identify and search for free pages
    bool* lkpp;
    int next_array_id;
    size_t mma_sz;
    /*add your extra states here freely for implementation*/
    std::vector<int> page_queue;
    std::vector<bool> clock_bit;

    std::mutex m;
    std::mutex* mtx = &m;
    std::mutex m1;
    std::mutex* mtx1 = &m1;
    std::mutex m2;
    std::mutex* mtx2 = &m2;
    

    void PageIn(int array_id, int virtual_page_id, int physical_page_id);
    void PageOut(int physical_page_id);
    int PageReplace(int array_id, int virtual_page_id);
};

}  // namespce: proj3

#endif

