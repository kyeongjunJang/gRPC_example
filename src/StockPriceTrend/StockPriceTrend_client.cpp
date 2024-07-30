#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "stock.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader; // ClientReader is used to read a stream of responses
using grpc::Status;
using stock::stockRequest;
using stock::stockResponse;
using stock::stockService;

class StockClient {
public:
    StockClient(std::shared_ptr<Channel> channel)
        : stub_(stockService::NewStub(channel)) {}

    void GetStockPrices(const std::string& ticker) {
        stockRequest request;
        request.set_code(ticker);

        // Create the context and reader
        ClientContext context;

        std::unique_ptr<ClientReader<stockResponse>> reader(stub_->GetStock(&context, request));

        // Process the response stream
        stockResponse response;
        bool namePrinted = false; // Flag to track if name has been printed

        while (reader->Read(&response)) {
            if (!namePrinted) {
                if (!response.name().empty()) {
                    std::cout << "Stock Name: " << response.name() << std::endl;
                    namePrinted = true; // Set the flag after printing the name
                }
            }
            if (!response.price().empty() && !response.time().empty()) {
                std::cout << "Time: " << response.time() << ", Price: " << response.price() << std::endl;
            }
        }
        // Check the status of the RPC
        Status status = reader->Finish();

        if (!status.ok()) {
            std::cerr << "RPC failed with status code: " << status.error_code() << std::endl;
            std::cerr << "Error message: " << status.error_message() << std::endl;
        }
    }

private:
    std::unique_ptr<stockService::Stub> stub_;
};

int main(int argc, char** argv) {
    std::string target_str = "localhost:50051";

    StockClient client(grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));

    std::string ticker;
    std::cout << "Enter stock ticker: ";
    std::cin >> ticker;

    client.GetStockPrices(ticker);

    return 0;
}