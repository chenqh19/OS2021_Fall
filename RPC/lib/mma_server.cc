#include "mma_server.h"
#include "memory_manager.h"

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

namespace proj4 {

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class MMAServerImpl final : public MMA::Service {
private:
    MemoryManager mma;
public: 
    explicit MMAServerImpl(int phy_page_num): mma(phy_page_num){};

    Status ReadPage(ServerContext* context, const ReadRequest *request, ReadReply *reply) override {
        int data = mma.ReadPage(request->array_id(), request->virtual_page_id(), request->offset());
        reply->set_data(data);
        return Status::OK;
    };

    Status WritePage(ServerContext* context, const WriteRequest *request, WriteReply *reply) override {
        mma.WritePage(request->array_id(), request->virtual_page_id(), request->offset(), request->value());
        return Status::OK;
    };

    Status Allocate(ServerContext* context, const AllocateRequest *request, AllocateReply *reply) override {
        int array_id = mma.Allocate(request->size());
        reply->set_array_id(array_id);
        return Status::OK;
    };

    Status Release(ServerContext* context, const ReleaseRequest *request, ReleaseReply *reply) override {
        mma.Release(request->array_id());
        return Status::OK;
    };

}

void RunServerUL(size_t phy_page_num) {
    std::string server_address("0.0.0.0:50051");
    MMAServerImpl service((int) phy_page_num);
    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

void RunServerL(size_t phy_page_num, size_t max_vir_page_num) {
    std::string server_address("0.0.0.0:50051");
    MMAServerImpl service((int) phy_page_num);
    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();

}

void ShutdownServer() {

}




}