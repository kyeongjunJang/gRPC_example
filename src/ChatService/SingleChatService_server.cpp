//GRPC unary RPC server implementation for SingleChatService
#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "chat.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using chat::ChatService;
using chat::ChatMessage;

class SingleChatServiceImpl final : public ChatService::Service {
    Status SendMessageSingle(ServerContext* context, const ChatMessage* request, ChatMessage* response) override {
        std::cout << "Received message from [" << request->name() << "]: " << request->message() << std::endl;
        response->set_message("Server received your message: " + request->message());
        return Status::OK;
    }
};

void RunServer() {
    std::string server_address("127.0.0.1:50054");
    SingleChatServiceImpl service;
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

int main() {
    RunServer();
    return 0;
}