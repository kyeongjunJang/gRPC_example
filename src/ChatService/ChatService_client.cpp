#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "chat.grpc.pb.h"
#include <thread>

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReaderWriter; // bidirectional streaming
using grpc::Status;
using chat::ChatService;
using chat::ChatMessage;

class ChatClient {
public:
    ChatClient(std::shared_ptr<Channel> channel) : stub_(ChatService::NewStub(channel)) {}

    void SendMessage(const std::string& name) {
        ClientContext context;
        std::shared_ptr<ClientReaderWriter<ChatMessage, ChatMessage>> stream(stub_->SendMessage(&context));
        std::thread writer([stream, name]() {
            std::string message;
            while (std::getline(std::cin, message)) {
                std::cout << "Enter message: ";
                ChatMessage chat_message;
                chat_message.set_name(name);
                chat_message.set_message(message);
                stream->Write(chat_message);
            }
            stream->WritesDone();
        });

        writer.join();
        Status status = stream->Finish();
        if (!status.ok()) {
            std::cerr << "SendMessage rpc failed." << std::endl;
        }
    }

private:
    std::unique_ptr<ChatService::Stub> stub_;
};

int main() {
    ChatClient client(grpc::CreateChannel("127.0.0.1:50053", grpc::InsecureChannelCredentials()));

    std::string user;
    std::cout << "Enter your name: ";
    std::cin >> user;

    client.SendMessage(user);
    return 0;
}
