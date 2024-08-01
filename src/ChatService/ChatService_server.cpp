#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "chat.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReaderWriter; // bidirectional streaming
using grpc::Status;
using chat::ChatService; // generated from chat.proto
using chat::ChatMessage; // generated from chat.proto

class ChatServiceImpl final : public ChatService::Service {
    Status SendMessage(ServerContext* context, ServerReaderWriter<ChatMessage, ChatMessage>* stream) override {
        ChatMessage message;
        while (stream->Read(&message)) {
            std::cout << "Received message from [" << message.name() << "]: " << message.message() << std::endl;
            stream->Write(message);
        }
        return Status::OK;
    }
};

void RunServer() {
    std::string server_address("127.0.0.1:50053");
    ChatServiceImpl service;
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
