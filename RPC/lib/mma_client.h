#ifndef MMA_CLIENT_H
#define MMA_CLIENT_H

#include <memory>
#include <cstdlib>

#include <grpc++/grpc++.h>

#ifdef BAZEL_BUILD
#include "proto/mma.grpc.pb.h"
#else
#include "mma.grpc.pb.h"
#endif

#define PageSize 1024

namespace proj4 {

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

class ArrayList;

class MmaClient {
private: 
    std::unique_ptr<mma::MMA::Stub> stub_;
public:
    explicit MmaClient(std::shared_ptr<Channel> channel);
    int ReadPage(int array_id, int virtual_page_id, int offset);
    void WritePage(int array_id, int virtual_page_id, int offset, int value);
    ArrayList* Allocate(size_t);
    // void Release(ArrayList*);
    void Free(ArrayList*);
    



};

} //namespace proj4

#endif