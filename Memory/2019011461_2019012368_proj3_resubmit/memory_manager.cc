#include "memory_manager.h"

#include "array_list.h"

namespace proj3 {
    PageFrame::PageFrame(){
    }
    int& PageFrame::operator[] (unsigned long idx){
        //each page should provide random access like an array
        return mem[idx];
    }
    void PageFrame::WriteDisk(std::string filename) {
        // write page content into disk files
        std::ofstream fout(filename);
        /*if (fout) {
            for (int i = 0; i < PageSize; i++) {
                fout << mem[i] << std::endl;
            }
        }
        fout.close();*/
        fout.write(reinterpret_cast<char *>(mem), PageSize * sizeof(int));
        fout.close();
    }
    void PageFrame::ReadDisk(std::string filename) {
        // read page content from disk files
        std::ifstream fin(filename);
        /*if (fin) {
            for (int i = 0; i < PageSize; i++) {
                fin >> mem[i];
            }
        }
        fin.close();*/
        if (!fin) {
            for (int i = 0; i < PageSize; i++) {
                mem[i] = 0;
            }
            return;
        }
        fin.read(reinterpret_cast<char *>(mem), PageSize * sizeof(int));
        fin.close();
    }

    PageInfo::PageInfo(){
    }
    void PageInfo::SetInfo(int cur_holder, int cur_vid){
        //modify the page states
        //you can add extra parameters if needed
        holder = cur_holder;
        virtual_page_id = cur_vid;
    }
    void PageInfo::ClearInfo(){
        //clear the page states
        //you can add extra parameters if needed
        holder = -1;
        virtual_page_id = -1;
    }

    int PageInfo::GetHolder(){return holder;}
    int PageInfo::GetVid(){return virtual_page_id;}
    

    MemoryManager::MemoryManager(size_t sz){
        //mma should build its memory space with given space size
        //you should not allocate larger space than 'sz' (the number of physical pages)
        this->page_map.clear();
        this->mem =  new PageFrame*[sz]();
        for (int j = 0; j < sz; j++) {
            mem[j] = new PageFrame();
        }
        this->page_info = new PageInfo[sz]();
        this->free_list = new bool[sz]();
        this->lkpp = new bool[sz]();
        for (int i = 0; i < sz; i++) {
            free_list[i] = false;
            lkpp[i] = false;
            page_info[i].SetInfo(-1, -1);
        }
        this->next_array_id = 0;
        this->mma_sz = sz;
    }
    MemoryManager::~MemoryManager(){

    }
    void MemoryManager::PageOut(int physical_page_id){
        //swap out the physical page with the indx of 'physical_page_id out' into a disk file
        int hd = page_info[physical_page_id].GetHolder();
        int vid = page_info[physical_page_id].GetVid();
        // std::string fname = "./data/at" + std::to_string(hd) + "at" + std::to_string(vid);
        // mem[physical_page_id]->WriteDisk(fname); // use a uniform rule of naming
        page_info[physical_page_id].ClearInfo();
        free_list[physical_page_id] = false;
        page_map[hd].erase(vid);
    }
    void MemoryManager::PageIn(int array_id, int virtual_page_id, int physical_page_id){
        //swap the target page from the disk file into a physical page with the index of 'physical_page_id out'
        free_list[physical_page_id] = true;
        // std::string fname = "./data/at" + std::to_string(array_id) + "at" + std::to_string(virtual_page_id);
        // mem[physical_page_id]->ReadDisk(fname);
        page_info[physical_page_id].SetInfo(array_id, virtual_page_id);
        page_map[array_id].insert(std::make_pair(virtual_page_id, physical_page_id));
    }
    int MemoryManager::PageReplace(int array_id, int virtual_page_id){
        //implement your page replacement policy here
        
        // // implement a random algorithm first
        // srand((unsigned)time(NULL));
        // int ppid = rand()/(sizeof(page_info)/sizeof(page_info[0]));
        // PageOut(ppid);
        // PageIn(array_id, virtual_page_id, ppid);
        bool hit = false;
        int ppid;
        //FIFO implementation
        if (FIFO) {
            mtx->unlock();
            for (int i = 0; i < page_queue.size(); i ++) {
                if (page_info[page_queue[i]].GetHolder() == array_id && page_info[page_queue[i]].GetVid() == virtual_page_id) {
                    hit = true;
                }
            }
            if (!hit) {
                if (page_queue.size() < mma_sz) {// have idle physical memory
                    for (int j = 0; j < mma_sz; j++) {
                        if (free_list[j] == false) {
                            ppid = j;
                            break;
                        }
                    }
                    page_queue.push_back(ppid);
                    PageIn(array_id, virtual_page_id, ppid);
                    std::string fname = "./data/at" + std::to_string(array_id) + "at" + std::to_string(virtual_page_id);
                    mem[ppid]->ReadDisk(fname);
                } else {
                    ppid = page_queue.at(0);
                    page_queue.erase(page_queue.begin());
                    page_queue.push_back(ppid);
                    int hd = page_info[ppid].GetHolder();
                    int vid = page_info[ppid].GetVid();
                    PageOut(ppid);
                    PageIn(array_id, virtual_page_id, ppid);
                    std::string fname = "./data/at" + std::to_string(hd) + "at" + std::to_string(vid);
                    mem[ppid]->WriteDisk(fname); // use a uniform rule of naming
                    std::string fname1 = "./data/at" + std::to_string(array_id) + "at" + std::to_string(virtual_page_id);
                    mem[ppid]->ReadDisk(fname1);
                }
            }
        } else {
        //CLOCK implementation
            for (int i = 0; i < page_queue.size(); i ++) {
                if (page_info[page_queue[i]].GetHolder() == array_id && page_info[page_queue[i]].GetVid() == virtual_page_id) {
                    hit = true;
                    clock_bit[i] = true;
                }
            }
            if (!hit) {
                if (page_queue.size() < mma_sz) {// have idle physical memory
                    for (int j = 0; j < mma_sz; j++) {
                        if (free_list[j] == false) {
                            ppid = j;
                            free_list[ppid] = true;
                            break;
                        }
                    }
                    page_queue.push_back(ppid);
                    clock_bit.push_back(false);
                    
                    PageIn(array_id, virtual_page_id, ppid);
                    mtx->unlock();
                    while (true) {
                        mtx1->lock();
                        if (!lkpp[ppid]) {
                            lkpp[ppid] = true;
                            mtx1->unlock();
                            break;
                        }
                        mtx1->unlock();
                        usleep(200);
                    }

                    // bool flag = 0;
                    // do {
                    //     flag = 0;
                    //     mtx2->lock();
                    //     for (int i = 0; i < locker.size(); i++) {
                    //         if (array_id == locker[i]) {
                    //             flag = 1;
                    //         }
                    //     }
                    //     if (flag == 0) {
                    //         locker.push_back(array_id);
                    //     } 
                    //     mtx2->unlock();
                    //     if (flag == 1) {
                    //         usleep(100);
                    //     }
                    // } while (flag == 1);

                    std::string fname = "./data/at" + std::to_string(array_id) + "at" + std::to_string(virtual_page_id);
                    mem[ppid]->ReadDisk(fname);

                    // mtx2->lock();
                    // std::remove(locker.begin(),locker.end(),array_id);
                    // mtx2->unlock();

                    mtx1->lock();
                    lkpp[ppid] = false;
                    mtx1->unlock();
                } else {
                    while (clock_bit.at(0) == true) {
                        clock_bit.erase(clock_bit.begin());
                        clock_bit.push_back(false);
                        int p = page_queue.at(0);
                        page_queue.erase(page_queue.begin());
                        page_queue.push_back(p);
                    }
                    ppid = page_queue.at(0);
                    page_queue.erase(page_queue.begin());
                    page_queue.push_back(ppid);
                    clock_bit.erase(clock_bit.begin());
                    clock_bit.push_back(false);
                    
                    int hd = page_info[ppid].GetHolder();
                    int vid = page_info[ppid].GetVid();
                    PageOut(ppid);
                    PageIn(array_id, virtual_page_id, ppid);

                    mtx->unlock();
                    while (true) {
                        mtx1->lock();
                        if (!lkpp[ppid]) {
                            lkpp[ppid] = true;
                            mtx1->unlock();
                            break;
                        }
                        mtx1->unlock();
                        usleep(200);
                    }
                    
                    // bool flag1 = 0;
                    // do {
                    //     flag1 = 0;
                    //     mtx2->lock();
                    //     for (int i = 0; i < locker.size(); i++) {
                    //         if (hd == locker[i]) {
                    //             flag1 = 1;
                    //         }
                    //     }
                    //     if (flag1 == 0) {
                    //         locker.push_back(hd);
                    //     } 
                    //     mtx2->unlock();
                    //     if (flag1 == 1) {
                    //         usleep(100);
                    //     }
                    // } while (flag1 == 1);

                    std::string fname = "./data/at" + std::to_string(hd) + "at" + std::to_string(vid);
                    mem[ppid]->WriteDisk(fname); // use a uniform rule of naming

                    // mtx2->lock();
                    // std::remove(locker.begin(),locker.end(),hd);
                    // mtx2->unlock();

                    // bool flag2 = 0;
                    // do {
                    //     flag2 = 0;
                    //     mtx2->lock();
                    //     for (int i = 0; i < locker.size(); i++) {
                    //         if (array_id == locker[i]) {
                    //             flag2 = 1;
                    //         }
                    //     }
                    //     if (flag2 == 0) {
                    //         locker.push_back(array_id);
                    //     } 
                    //     mtx2->unlock();
                    //     if (flag2 == 1) {
                    //         usleep(100);
                    //     }
                    // } while (flag2 == 1);

                    std::string fname1 = "./data/at" + std::to_string(array_id) + "at" + std::to_string(virtual_page_id);
                    mem[ppid]->ReadDisk(fname1);

                    // mtx2->lock();
                    // std::remove(locker.begin(),locker.end(),array_id);
                    // mtx2->unlock();

                    mtx1->lock();
                    lkpp[ppid] = false;
                    mtx1->unlock();
                }
            } else {
                mtx->unlock();
            }
        }
        return ppid;
    }
    int MemoryManager::ReadPage(int array_id, int virtual_page_id, int offset){
        // for arrayList of 'array_id', return the target value on its virtual space
        mtx->lock();
        int ppid;
        if (!page_map[array_id].count(virtual_page_id)) {
            ppid = PageReplace(array_id, virtual_page_id);
        } else {
            ppid = page_map[array_id][virtual_page_id];
            mtx->unlock();
        }
        int con = mem[ppid]->ReadContent(offset);
        return con;
    }
    void MemoryManager::WritePage(int array_id, int virtual_page_id, int offset, int value){
        // for arrayList of 'array_id', write 'value' into the target position on its virtual space
        mtx->lock();
        int ppid;
        if (!page_map[array_id].count(virtual_page_id)) {
            ppid = PageReplace(array_id, virtual_page_id);
        } else {
            ppid = page_map[array_id][virtual_page_id];
            mtx->unlock();
        }
        mem[ppid]->WriteContent(offset, value);
    }
    ArrayList* MemoryManager::Allocate(size_t sz){
        // when an application requires for memory, create an ArrayList and record mappings from its virtual memory space to the physical memory space
        mtx->lock();
        ArrayList* nalp = new ArrayList(sz, this, next_array_id);
        std::map<int, int> map;
        page_map.insert(std::make_pair(next_array_id, map));
        next_array_id++;
        mtx->unlock();
        return nalp;
    }
    void MemoryManager::Release(ArrayList* arr){
        // an application will call release() function when destroying its arrayList
        // release the virtual space of the arrayList and erase the corresponding mappings
        std::map<int, std::map<int, int>>::iterator iter;
        int i = 0;
        mtx->lock();
        if (!page_map.count(arr->array_id)) {
            return;
        }
        
        while(iter != page_map.end()) {
            if (page_map[arr->array_id].find(i) == page_map[arr->array_id].end()) {break;}
            
            int ppid = page_map[arr->array_id][i];
            page_info[ppid].ClearInfo();
            free_list[ppid] = false;
            int j = 0;
            
            for (std::vector<int>::iterator iter = page_queue.begin(); iter != page_queue.end(); iter++) {
                if (*iter == ppid) {
                    page_queue.erase(iter);
                    if (!FIFO) {
                        clock_bit.erase(clock_bit.begin()+j);
                    }
                    break;
                }
                j++;
            }
            i++;
        }
        page_map.erase(arr->array_id);
        mtx->unlock();
        for (int j = 0; j < mma_sz; j++) {
            std::remove(("./data/at" + std::to_string(arr->array_id) + "at" + std::to_string(j)).c_str());
        }
        // next_array_id--; // not sure
        arr->~ArrayList();
    }
} // namespce: proj3