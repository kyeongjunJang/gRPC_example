gRPC example
============= 

## Language and Build System
- Language: C++
- Build System: CMake

### 1. GetWeather: Single Request → Single Response
- Service Description: When a client requests weather information, the server responds with weather data for the specified location.
- Protocol: Defines an RPC method for single request and single response.

### 2. StockPriceTrend: Single Request → Stream Response (TBU)
- Service Description: Clients request stock price trends, and the server streams real-time price data for a specific stock.
- Protocol: Defines an RPC method for single request and streaming response.

### 3. SoccerStreaming: Stream Request → Stream Response (TBU)
- Service Description: Multiple clients watch live soccer matches and receive event updates .
- Protocol: Defines an RPC method for streaming request and streaming response.

<pre>
<code>
$ git clone https://github.com/kyeongjunJang/gRPC_example.git
$ cd gRPC_example
$ mkdir build && cd build
$ cmake ..
$ make

$ ./src/GetWeather/GetWeather_server

Run the Client: In the new terminal, execute the client code:

$ ./src/GetWeather/GetWeather_client 

If you have other gRPC examples you’d like to explore, feel free to adapt the steps similarly.
</code>
</pre>
