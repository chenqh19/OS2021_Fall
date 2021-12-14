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
        this->next_array_id = 0;
        this->mma_sz = sz;
    }
    MemoryManager::~MemoryManager(){

    }
    void MemoryManager::PageOut(int physical_page_id){
        //swap out the physical page with the indx of 'physical_page_id out' into a disk file
        int hd = page_info[physical_page_id].GetHolder();
        int vid = page_info[physical_page_id].GetVid();
        std::string fname = "./data/at" + std::to_string(hd) + std::to_string(vid);
        mem[physical_page_id]->WriteDisk(fname); // use a uniform rule of naming
        page_info[physical_page_id].ClearInfo();
        free_list[physical_page_id] = false;
        page_map[hd].erase(vid);
    }
    void MemoryManager::PageIn(int array_id, int virtual_page_id, int physical_page_id){
        //swap the target page from the disk file into a physical page with the index of 'physical_page_id out'
        free_list[physical_page_id] = true;
        std::string fname = "./data/at" + std::to_string(array_id) + std::to_string(virtual_page_id);
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
            for (int i = 0; i < page_queue.size(); i ++) {
                if (page_info[page_queue[i]].GetHolder() == array_id && page_info[page_queue[i]].GetVid() == virtual_page_id) {
                    hit = true;
                }
            }
            if (!hit) {
                if (page_queue.size() < mma_sz) {// have idle physical memory
                    int ppid;
                    for (int j = 0; j < mma_sz; j++) {
                        if (free_list[j] == false) {
                            ppid = j;
                            break;
                        }
                    }
                    page_queue.push_back(ppid);
                    PageIn(array_id, virtual_page_id, ppid);
                } else {
                    int ppid = page_queue.at(0);
                    page_queue.erase(page_queue.begin());
                    page_queue.push_back(ppid);
                    PageOut(ppid);
                    PageIn(array_id, virtual_page_id, ppid);
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
                    int ppid;
                    for (int j = 0; j < mma_sz; j++) {
                        if (free_list[j] == false) {
                            ppid = j;
                            break;
                        }
                    }
                    page_queue.push_back(ppid);
                    clock_bit.push_back(false);
                    PageIn(array_id, virtual_page_id, ppid);
                } else {
                    while (clock_bit.at(0) = true) {
                        clock_bit.erase(clock_bit.begin());
                        clock_bit.push_back(false);
                        int p = page_queue.at(0);
                        page_queue.erase(page_queue.begin());
                        page_queue.push_back(p);
                    }
                    int ppid = page_queue.at(0);
                    page_queue.erase(page_queue.begin());
                    page_queue.push_back(ppid);
                    clock_bit.erase(clock_bit.begin());
                    clock_bit.push_back(false);
                    PageOut(ppid);
                    PageIn(array_id, virtual_page_id, ppid);
                }
            }
        }
    }
    int MemoryManager::ReadPage(int array_id, int virtual_page_id, int offset){
        // for arrayList of 'array_id', return the target value on its virtual space
        if (!page_map[array_id].count(virtual_page_id)) {
            PageReplace(array_id, virtual_page_id);
        }
        return mem[page_map[array_id][virtual_page_id]]->ReadContent(offset);
        
    }
    void MemoryManager::WritePage(int array_id, int virtual_page_id, int offset, int value){
        // for arrayList of 'array_id', write 'value' into the target position on its virtual space
        if (!page_map[array_id].count(virtual_page_id)) {
            PageReplace(array_id, virtual_page_id);
        }
        mem[page_map[array_id][virtual_page_id]]->WriteContent(offset, value);
    }
    ArrayList* MemoryManager::Allocate(size_t sz){
        // when an application requires for memory, create an ArrayList and record mappings from its virtual memory space to the physical memory space
        ArrayList* nalp = new ArrayList(sz, this, next_array_id);
        std::map<int, int> map;
        page_map.insert(std::make_pair(next_array_id, map));
        next_array_id++;
        return nalp;
    }
    void MemoryManager::Release(ArrayList* arr){
        // an application will call release() function when destroying its arrayList
        // release the virtual space of the arrayList and erase the corresponding mappings
        arr->~ArrayList();
        std::map<int, std::map<int, int>>::iterator iter;
        int i = 0;
        if (!page_map.count(arr->array_id)) {
            return;
        }
        while(iter != page_map.end()) {
            if (page_map[arr->array_id].find(i) == page_map[arr->array_id].end()) {break;}
            int ppid = page_map[arr->array_id][i];
            page_info[ppid].ClearInfo();
            free_list[ppid] = false;
            // page_queue.erase(std::remove(page_queue.begin(), page_queue.end(), ppid), page_queue.end());
            int j = 0;
            for (std::vector<int>::iterator iter = page_queue.begin(); iter != page_queue.end(); iter++) {
                if (*iter == ppid) {
                    page_queue.erase(iter);
                    if (!FIFO) {
                        clock_bit.erase(clock_bit.begin()+j);
                    }
                    break;
                }
                j ++;
            }
            std::remove(("./data/at" + std::to_string(arr->array_id) + std::to_string(i)).c_str());
            i++;
        }
        page_map.erase(arr->array_id);
        // next_array_id--; // not sure
    }
} // namespce: proj3