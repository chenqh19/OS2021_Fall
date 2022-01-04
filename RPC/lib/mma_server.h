#ifndef MMA_SERVER_H
#define MMA_SERVER_H

#include <iostream>
#include <memory>
#include <string>
#include <cstdlib>

#include <grpc++/grpc++.h>
#include <grpc++/ext/proto_server_reflection_plugin.h>
#include <grpc++/health_check_service_interface.h>

#ifdef BAZEL_BUILD
#include "proto/mma.grpc.pb.h"
#else
#include "mma.grpc.pb.h"
#endif

#include "memory_manager.h"


// Logic and data behind the server's behavior.

namespace proj4 {


class MMAServerImpl final : public mma::MMA::Service {
// private:
//     MemoryManager mma;
// public: 
//     explicit MMAServerImpl(int phy_page_num): mma(phy_page_num){};

    grpc::Status ReadPage(grpc::ServerContext* context, const mma::ReadRequest *request, mma::ReadReply *reply) override;

    grpc::Status WritePage(grpc::ServerContext* context, const mma::WriteRequest *request, mma::WriteReply *reply) override;

    grpc::Status Allocate(grpc::ServerContext* context, const mma::AllocateRequest *request, mma::AllocateReply *reply) override;

    grpc::Status Release(grpc::ServerContext* context, const mma::ReleaseRequest *request, mma::ReleaseReply *reply) override;


};

// setup a server with UnLimited virtual memory space
void RunServerUL(size_t phy_page_num);

// setup a server with Limited virtual memory space
void RunServerL(size_t phy_page_num, size_t max_vir_page_num);

// shutdown the server setup by RunServerUL or RunServerL
void ShutdownServer();

} //namespace proj4

#endif