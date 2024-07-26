#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "service.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using service::HelloRequest;
using service::HelloResponse;
using service::MyService;

// Logic and data behind the server's behavior.
class MyServiceImpl final : public MyService::Service {
    Status SayHello(ServerContext* context, const HelloRequest* request, HelloResponse* reply) override {
        std::string prefix("Hello ");
        reply->set_message(prefix + request->name());
        return Status::OK;
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    MyServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

int main(int argc, char** argv) {
    RunServer();
    return 0;
}
