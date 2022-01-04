#include "mma_server.h"
#include "memory_manager.h"

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

namespace proj4 {

MemoryManager* mmaServer;
std::unique_ptr<grpc::Server> server;

grpc::Status MMAServerImpl::ReadPage(grpc::ServerContext* context, const mma::ReadRequest *request, mma::ReadReply *reply) {
    int data = mmaServer->ReadPage(request->array_id(), request->virtual_page_id(), request->offset());
    reply->set_data(data);
    return grpc::Status::OK;
}

grpc::Status MMAServerImpl::WritePage(grpc::ServerContext* context, const mma::WriteRequest *request, mma::WriteReply *reply) {
    mmaServer->WritePage(request->array_id(), request->virtual_page_id(), request->offset(), request->value());
    return grpc::Status::OK;
}

grpc::Status MMAServerImpl::Allocate(grpc::ServerContext* context, const mma::AllocateRequest *request, mma::AllocateReply *reply) {
    int array_id = mmaServer->Allocate(request->size());
    // if(mma->remained_virtual_pages == 0) {
    //     return Status::CANCELLED;
    // }
    reply->set_array_id(array_id);
    // mma->remained_virtual_pages -= 1;
    return grpc::Status::OK;
}

grpc::Status MMAServerImpl::Release(grpc::ServerContext* context, const mma::ReleaseRequest *request, mma::ReleaseReply *reply) {
    mmaServer->Release(request->array_id(), request->num_of_pages());
    return grpc::Status::OK;
}

void RunServerUL(size_t phy_page_num) {
    mmaServer = new MemoryManager(phy_page_num);
    // mma = std::unique_ptr<size_t>(new size_t(phy_page_num));
    std::string server_address("0.0.0.0:50051");
    MMAServerImpl service;
    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    server = builder.BuildAndStart();
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

void RunServerL(size_t phy_page_num, size_t max_vir_page_num) {
    mmaServer = new MemoryManager(phy_page_num, max_vir_page_num);
    // mma = std::unique_ptr<size_t, size_t>(new size_t(phy_page_num), new size_t(max_vir_page_num));
    std::string server_address("0.0.0.0:50051");
    MMAServerImpl service;
    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    server = builder.BuildAndStart();
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();

}

void ShutdownServer() {
    server->Shutdown();

}




}