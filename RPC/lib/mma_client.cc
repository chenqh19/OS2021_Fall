#include "mma_client.h"
#include "array_list.h"

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

    MmaClient::MmaClient(std::shared_ptr<Channel> channel): stub_(mma::MMA::NewStub(channel)) {}

    int MmaClient::ReadPage(int array_id, int virtual_page_id, int offset) {
        mma::ReadRequest request;
        mma::ReadReply reply;
        request.set_array_id(array_id);
        request.set_virtual_page_id(virtual_page_id);
        request.set_offset(offset);
        ClientContext context;
        Status status = stub_->ReadPage(&context, request, &reply);
        if(status.ok()) {
            return reply.data();
        } else {
            //需要报错方式
            std::cerr << status.error_code() << ": " << status.error_message() << std::endl;
                throw std::runtime_error("RPC failed");
        }
    }

    void MmaClient::WritePage(int array_id, int virtual_page_id, int offset, int value) {
        mma::WriteRequest request;
        mma::WriteReply reply;
        request.set_array_id(array_id);
        request.set_virtual_page_id(virtual_page_id);
        request.set_offset(offset);
        request.set_value(value);
        ClientContext context;
        Status status = stub_->WritePage(&context, request, &reply);
        if(status.ok()) {
        } else {
            //需要报错方式
            std::cerr << status.error_code() << ": " << status.error_message() << std::endl;
                throw std::runtime_error("RPC failed");
        }
    }

    ArrayList* MmaClient::Allocate(size_t sz) {
        mma::AllocateRequest request;
        mma::AllocateReply reply;
        request.set_size(sz);
        ClientContext context;
        Status status = stub_->Allocate(&context, request, &reply);
        if(status.ok()) {
            return new ArrayList(reply.array_id(), this, sz);
        } else {
            //需要报错方式
            std::cerr << status.error_code() << ": " << status.error_message() << std::endl;
                throw std::runtime_error("RPC failed");
        }

    }

    void MmaClient::Free(ArrayList* arr) {
        mma::ReleaseRequest request;
        mma::ReleaseReply reply;
        request.set_array_id(arr->array_id);
        request.set_num_of_pages((arr->size+PageSize-1)/PageSize);
        ClientContext context;
        Status status = stub_->Release(&context, request, &reply);
        delete arr;
        if(status.ok()) {
            return;
        } else {
            //需要报错方式
        }
    }

}