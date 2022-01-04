#include "mma_server.h"
#include "memory_manager.h"

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

namespace proj4 {

MemoryManager* mma;
std::unique_ptr<grpc::Server> server;

grpc::Status ReadPage(ServerContext* context, const mma::ReadRequest *request, mma::ReadReply *reply) {
    int data = mma->ReadPage(request->array_id(), request->virtual_page_id(), request->offset());
    reply->set_data(data);
    return Status::OK;
}

grpc::Status WritePage(ServerContext* context, const mma::WriteRequest *request, mma::WriteReply *reply) {
    mma->WritePage(request->array_id(), request->virtual_page_id(), request->offset(), request->value());
    return Status::OK;
}

grpc::Status Allocate(ServerContext* context, const mma::AllocateRequest *request, mma::AllocateReply *reply) {
    int array_id = mma->Allocate(request->size());
    reply->set_array_id(array_id);
    return Status::OK;
}

grpc::Status Release(ServerContext* context, const mma::ReleaseRequest *request, mma::ReleaseReply *reply) {
    mma->Release(request->array_id(), request->num_of_pages());
    return Status::OK;
}

void RunServerUL(size_t phy_page_num) {
    mma = new MemoryManager(phy_page_num);
    // mma = std::unique_ptr<size_t>(new size_t(phy_page_num));
    std::string server_address("0.0.0.0:50051");
    MMAServerImpl service;
    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    server = builder.BuildAndStart();
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

void RunServerL(size_t phy_page_num, size_t max_vir_page_num) {
    mma = new MemoryManager(phy_page_num, max_vir_page_num);
    // mma = std::unique_ptr<size_t, size_t>(new size_t(phy_page_num), new size_t(max_vir_page_num));
    std::string server_address("0.0.0.0:50051");
    MMAServerImpl service;
    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    server = builder.BuildAndStart();
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();

}

void ShutdownServer() {

}




}