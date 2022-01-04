#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include <cstdlib>
#include "mma_client.h"

namespace proj4 {

    class MmaClient;

    class ArrayList{
        private:
        friend class MmaClient;
        size_t size;
        MmaClient* mma;
        int array_id;
        int page_num;
        ArrayList(size_t, MmaClient*, int);
        ~ArrayList();
        public:
        // you should not modify the public interfaces used in tests
        int Read (unsigned long);
        void Write (unsigned long, int);
    };

} // namespce: proj3
#endif