#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <grpcpp/grpcpp.h>
#include "stock.grpc.pb.h"
#include "nlohmann/json.hpp"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerWriter; // ServerWriter is used to send multiple messages
using grpc::ServerBuilder; // ServerBuilder is used to create the server
using grpc::Status;
using grpc::StatusCode; // StatusCode is used to define the status of an RPC
using stock::stockRequest;
using stock::stockResponse;
using stock::stockService;
using json = nlohmann::json;

class StockServiceImpl final : public stockService::Service {
public:
    StockServiceImpl(const std::string& db_file) {
        std::ifstream db(db_file);
        if (!db.is_open()) {
            throw std::runtime_error("Failed to open database file.");
        }
        db >> stock_data_;
    }

    Status GetStock(ServerContext* context, const stockRequest* request,
                          ServerWriter<stockResponse>* writer) override {
        std::string code = request->code();
        
        if (stock_data_.find(code) == stock_data_.end()) {
            return Status(StatusCode::NOT_FOUND, "Stock code not found.");
        }

        const auto& prices = stock_data_[code];
        stockResponse response;

        // Ensure name is not empty before setting it
        if (prices.size() > 0 && prices[0].contains("name")) {
            response.set_name(prices[0]["name"].get<std::string>());
            writer->Write(response); // Send name only once
        }

        // Send the price and time data
        for (size_t i = 1; i < prices.size(); ++i) { // Start from index 1 if index 0 is used for name
            if (prices[i].contains("time") && prices[i].contains("price")) {
                response.clear_name(); // Ensure name is not included in subsequent messages
                response.set_price(prices[i]["price"].get<std::string>());
                response.set_time(prices[i]["time"].get<std::string>());
                writer->Write(response);
                // std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate real-time updates
            }
        }
        return Status::OK;
    }

private:
    json stock_data_;
};
void RunServer(const std::string& db_file) {
    std::string server_address("0.0.0.0:50051");
    StockServiceImpl service(db_file);

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait();
}


int main(int argc, char** argv) {
    std::string db_file = "../config/stock_db.json";
    RunServer(db_file);
    return 0;
}