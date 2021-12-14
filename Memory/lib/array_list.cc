#include "array_list.h"

#include "memory_manager.h"

namespace proj3 {
    ArrayList::ArrayList(size_t sz, MemoryManager* cur_mma, int id){
        this->array_id = id;
        this->mma = cur_mma;
        this->size = sz;
    }
    int ArrayList::Read (unsigned long idx){
        //read the value in the virtual index of 'idx' from mma's memory space
        return mma->ReadPage(array_id, idx/PageSize, idx%PageSize);
    }
    void ArrayList::Write (unsigned long idx, int value){
        //write 'value' in the virtual index of 'idx' into mma's memory space
        mma->WritePage(array_id, idx/PageSize, idx%PageSize, value);
    }
    ArrayList::~ArrayList(){
    }
} // namespce: proj3