#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "weather.grpc.pb.h"
#include "nlohmann/json.hpp"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using weather::weatherRequest;
using weather::weatherResponse;
using weather::weatherService;
using json = nlohmann::json;

class WeatherServiceImpl final : public weatherService::Service {
public:
  WeatherServiceImpl(const std::string& db_file) {
    // 데이터베이스 파일을 열어 JSON 데이터를 읽어옵니다.
    std::ifstream ifs(db_file);
    if (ifs.is_open()) {
      ifs >> weather_db_;
    } else {
      std::cerr << "Failed to open database file: " << db_file << std::endl;
    }
  }

  Status GetWeather(ServerContext* context, const weatherRequest* request, weatherResponse* reply) override {
    std::string place = request->place();

    if (weather_db_.contains(place)) {
      // JSON 객체를 문자열로 변환하여 protobuf 메시지에 설정합니다.
      reply->set_temperature(weather_db_[place]["temperature"].get<std::string>());
      reply->set_weather(weather_db_[place]["weather"].get<std::string>());
      reply->set_rain(weather_db_[place]["rain"].get<std::string>());
    } else {
      // 요청한 장소가 데이터베이스에 없을 경우 "N/A"로 응답합니다.
      reply->set_temperature("N/A");
      reply->set_weather("N/A");
      reply->set_rain("N/A");
    }
    return Status::OK;
  }

private:
  json weather_db_; // JSON 데이터베이스를 저장할 변수
};

void RunServer(const std::string& db_file) {
  std::string server_address("0.0.0.0:50051");
  WeatherServiceImpl service(db_file);

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  server->Wait();
}

int main(int argc, char** argv) {
  // 데이터베이스 파일 경로를 인자로 전달받을 수 있도록 수정
  std::string db_file = "../config/weather_db.json";
  if (argc > 1) {
    db_file = argv[1];
  }
  RunServer(db_file);
  return 0;
}
