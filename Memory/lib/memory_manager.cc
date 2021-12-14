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
        fin.read(reinterpret_cast<char *>(mem), PageSize * sizeof(int));
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
        *this->mem =  new PageFrame[sz]();
        this->page_info = new PageInfo[sz]();
        this->free_list = new unsigned int[sz]();
        this->next_array_id = 0;
        this->mma_sz = sz;
    }
    MemoryManager::~MemoryManager(){
    }
    void MemoryManager::PageOut(int physical_page_id){
        //swap out the physical page with the indx of 'physical_page_id out' into a disk file
        int hd = page_info[physical_page_id].GetHolder();
        int vid = page_info[physical_page_id].GetVid();
        std::string fname = "/tmp/at" + std::to_string(hd) + std::to_string(vid);
        mem[physical_page_id]->WriteDisk(fname); // use a uniform rule of naming
        page_info[physical_page_id].ClearInfo();
    }
    void MemoryManager::PageIn(int array_id, int virtual_page_id, int physical_page_id){
        //swap the target page from the disk file into a physical page with the index of 'physical_page_id out'
        std::string fname = "/tmp/at" + std::to_string(array_id) + std::to_string(virtual_page_id);
        mem[physical_page_id]->ReadDisk(fname);
        page_info[physical_page_id].SetInfo(array_id, virtual_page_id);
        page_map[array_id].insert(std::make_pair(virtual_page_id, physical_page_id));
    }
    void MemoryManager::PageReplace(int array_id, int virtual_page_id){
        //implement your page replacement policy here
        
        // // implement a random algorithm first
        // srand((unsigned)time(NULL));
        // int ppid = rand()/(sizeof(page_info)/sizeof(page_info[0]));
        // PageOut(ppid);
        // PageIn(array_id, virtual_page_id, ppid);
        bool hit = false;
        //FIFO implementation
        if (FIFO) {
            for (int i = 0; i < page_queue[array_id].size(); i ++) {
                if (page_queue[array_id][i] == virtual_page_id) {
                    hit = true;
                }
            }
            if (!hit) {
                if (page_queue[array_id].size() < sizeof(page_info)/sizeof(page_info[0])) {// have idle physical memory
                    int ppid = page_queue.size();
                    page_queue[array_id].push_back(ppid);
                    PageIn(array_id, virtual_page_id, ppid);
                } else {
                    int ppid = page_queue[array_id].at(0);
                    page_queue[array_id].erase(page_queue[array_id].begin());
                    page_queue[array_id].push_back(ppid);
                    PageOut(ppid);
                    PageIn(array_id, virtual_page_id, ppid);
                }
            }
        } else {
        //CLOCK implementation
            for (int i = 0; i < page_queue[array_id].size(); i ++) {
                if (page_queue[array_id][i] == virtual_page_id) {
                    hit = true;
                    clock_bit[array_id][i] = true;
                }
            }
            if (!hit) {
                if (page_queue[array_id].size() < sizeof(page_info)/sizeof(page_info[0])) {// have idle physical memory
                    int ppid = page_queue.size();
                    page_queue[array_id].push_back(ppid);
                    clock_bit[array_id].push_back(false);
                    PageIn(array_id, virtual_page_id, ppid);
                } else {
                    while (clock_bit[array_id].at(0) = true) {
                        clock_bit[array_id].erase(clock_bit[array_id].begin());
                        clock_bit[array_id].push_back(false);
                        int p = page_queue[array_id].at(0);
                        page_queue[array_id].erase(page_queue[array_id].begin());
                        page_queue[array_id].push_back(p);
                    }
                    int ppid = page_queue[array_id].at(0);
                    page_queue[array_id].erase(page_queue[array_id].begin());
                    page_queue[array_id].push_back(ppid);
                    clock_bit[array_id].erase(clock_bit[array_id].begin());
                    clock_bit[array_id].push_back(false);
                    PageOut(ppid);
                    PageIn(array_id, virtual_page_id, ppid);
                }
            }
        }

        // //LRU implementation
        // bool hit = false;
        // for(int i = 0; i < page_queue[array_id].size(); i++) {
        //     if(page_queue[array_id][i] == virtual_page_id) {
        //         hit = true;
        //         int ppid = page_queue[array_id].at(i);
        //         page_queue[array_id].erase(page_queue[array_id].begin()+i);
        //         page_queue[array_id].push_back(ppid);
        //     }
        // }
        // if (!hit) {
        //     int ppid = page_queue[array_id].at(0);
        //     page_queue[array_id].erase(page_queue[array_id].begin());
        //     page_queue[array_id].push_back(ppid);
        //     PageOut(ppid);
        //     PageIn(array_id, virtual_page_id, ppid);
        // }

        


    }
    int MemoryManager::ReadPage(int array_id, int virtual_page_id, int offset){
        // for arrayList of 'array_id', return the target value on its virtual space
        return mem[page_map[array_id][virtual_page_id]]->ReadContent(offset);
        
    }
    void MemoryManager::WritePage(int array_id, int virtual_page_id, int offset, int value){
        // for arrayList of 'array_id', write 'value' into the target position on its virtual space
        mem[page_map[array_id][virtual_page_id]]->WriteContent(offset, value);
    }
    ArrayList* MemoryManager::Allocate(size_t sz){
        // when an application requires for memory, create an ArrayList and record mappings from its virtual memory space to the physical memory space
        ArrayList new_array_list(sz, this, next_array_id);
        ArrayList* nalp = &new_array_list;
        next_array_id++;
        return nalp;
    }
    void MemoryManager::Release(ArrayList* arr){
        // an application will call release() function when destroying its arrayList
        // release the virtual space of the arrayList and erase the corresponding mappings
        arr->~ArrayList();
        next_array_id--; // not sure
    }
} // namespce: proj3