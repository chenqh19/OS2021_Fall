#include "memory_manager.h"

#include "array_list.h"

namespace proj3 {
    PageFrame::PageFrame(){
    }
    int& PageFrame::operator[] (unsigned long idx){
        //each page should provide random access like an array
    }
    void PageFrame::WriteDisk(std::string filename) {
        // write page content into disk files
        for (int i = 0; i < PageSize; i++) {
            filename += std::to_string(mem[i]);
        }
    }
    void PageFrame::ReadDisk(std::string filename) {
        // read page content from disk files
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
    }
    MemoryManager::~MemoryManager(){
    }
    void MemoryManager::PageOut(int physical_page_id){
        //swap out the physical page with the indx of 'physical_page_id out' into a disk file
        mem[physical_page_id]->WriteDisk();
        page_info[physical_page_id].ClearInfo();
    }
    void MemoryManager::PageIn(int array_id, int virtual_page_id, int physical_page_id){
        //swap the target page from the disk file into a physical page with the index of 'physical_page_id out'
        mem[physical_page_id]->ReadDisk();
        page_info[physical_page_id].SetInfo(array_id, virtual_page_id);
        page_map[array_id].insert(std::make_pair(virtual_page_id, physical_page_id));

    }
    void MemoryManager::PageReplace(int array_id, int virtual_page_id){
        //implement your page replacement policy here

    }
    int MemoryManager::ReadPage(int array_id, int virtual_page_id, int offset){
        // for arrayList of 'array_id', return the target value on its virtual space
        
    }
    void MemoryManager::WritePage(int array_id, int virtual_page_id, int offset, int value){
        // for arrayList of 'array_id', write 'value' into the target position on its virtual space
    }
    ArrayList* MemoryManager::Allocate(size_t sz){
        // when an application requires for memory, create an ArrayList and record mappings from its virtual memory space to the physical memory space
    }
    void MemoryManager::Release(ArrayList* arr){
        // an application will call release() function when destroying its arrayList
        // release the virtual space of the arrayList and erase the corresponding mappings
    }
} // namespce: proj3