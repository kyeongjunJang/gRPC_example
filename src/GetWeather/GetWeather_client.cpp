#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "weather.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using weather::weatherRequest;
using weather::weatherResponse;
using weather::weatherService;

class WeatherClient {
public:
  WeatherClient(std::shared_ptr<Channel> channel) 
      : stub_(weatherService::NewStub(channel)) {}

  void GetWeatherInfo(const std::string& place) {
    weatherRequest request;
    request.set_place(place);

    weatherResponse reply;
    ClientContext context;

    Status status = stub_->GetWeather(&context, request, &reply);

    if (status.ok()) {
      std::cout << "Temperature: " << reply.temperature() << std::endl;
      std::cout << "Weather: " << reply.weather() << std::endl;
      std::cout << "Rain: " << reply.rain() << std::endl;
    } else {
      std::cerr << "RPC failed: " << status.error_message() << std::endl;
    }
  }

private:
  std::unique_ptr<weatherService::Stub> stub_;
};

int main(int argc, char** argv) {
  std::string target_str = "localhost:50051";

  WeatherClient client(grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));

  std::string place;
  std::cout << "Enter the place for weather information: (e.g. Seoul, Busan, Incheon)" << std::endl;
  std::getline(std::cin, place);

  client.GetWeatherInfo(place);
  return 0;
}
