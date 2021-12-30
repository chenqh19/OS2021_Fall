#include "mma_client.h"
#include "memory_manager.h"

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>


namespace proj4{

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

MmaClient::MmaClient(std::shared_ptr<Channel> channel): stub_(MMA::NewStub(channel)) {}

int MmaClient::ReadPage(int array_id, int virtual_page_id, int offset) {
    ReadRequest request;
    ReadReply reply;
    request.set_array_id(array_id);
    request.set_virtual_page_id(virtual_page_id);
    request.set_offset(offset);
    ClientContext context;
    Status status = stub_->ReadPage(&context, request, &reply);
    if(status.ok) {
        return reply.data;
    } else {
        //需要报错方式
    }

}

void MmaClient::WritePage(int array_id, int virtual_page_id, int offset, int value) {
    WriteRequest request;
    WriteReply reply;
    request.set_array_id(array_id);
    request.set_virtual_page_id(virtual_page_id);
    request.set_offset(offset);
    request.set_value(value);
    ClientContext context;
    Status status = stub_->WritePage(&context, request, &reply);
    if(status.ok) {
    } else {
        //需要报错方式
    }
}

ArrayList* MmaClient::Allocate(size_t sz) {
    AllocateRequest request;
    AllocateReply reply;
    request.set_size_t((int) sz));
    ClientContext context;
    Status status = stub_->Allocate(&context, request, &reply);
    if(status.ok) {
        return new ArrayList(this, sz, reply.array_id());
    } else {
        //需要报错方式
    }

}

void MmaClient::Release(ArrayList* arr) {
    ReleaseRequest request;
    ReleaseReply reply;
    request.set_array_id(arr->array_id);
    ClientContext context;
    Status status = stub_->Release(&context, request, &reply);
    if(status.ok) {
    } else {
        //需要报错方式
    }
}







}