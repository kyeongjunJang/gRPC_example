//GRPC unary RPC client implementation for SingleChatService
#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "chat.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using chat::ChatService;
using chat::ChatMessage;

class SingleChatClient {
public:
    SingleChatClient(std::shared_ptr<Channel> channel) : stub_(ChatService::NewStub(channel)) {}

    void SendMessage(const std::string& name, const std::string& message) {
        ChatMessage request;
        request.set_name(name);
        request.set_message(message);
        ChatMessage response;
        ClientContext context;
        Status status = stub_->SendMessageSingle(&context, request, &response);
        if (status.ok()) {

        } else {
            std::cerr << "RPC failed." << std::endl;
        }
    }

private:
    std::unique_ptr<ChatService::Stub> stub_;
};

int main() {
    SingleChatClient client(grpc::CreateChannel("127.0.0.1:50054", grpc::InsecureChannelCredentials()));

    std::string user;
    std::cout << "Enter your name: ";
    std::cin >> user;

    while (true) {
        std::string message;
        std::cout << "Enter message: ";
        std::getline(std::cin, message);

        client.SendMessage(user, message);
    }
    return 0;
}