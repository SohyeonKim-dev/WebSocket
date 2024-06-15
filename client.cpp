#include <iostream>
#include <boost/asio.hpp>
#include <boost/beast.hpp>

using namespace boost::asio;
namespace websocket = boost::beast::websocket;

int main() {
    try {
        boost::asio::io_context ioservice; 
        ip::tcp::resolver resolver(ioservice);
        ip::tcp::socket socket(ioservice); 
        
        // 기존 IP address로 작성한 부분 수정, client 이미지 docker hub 재업로드 
        // https://hub.docker.com/r/sohyeonkimdev/websocket_kubernetes/tags 
        auto const results = resolver.resolve(std::getenv("SERVER_ADDRESS"), std::getenv("SERVER_PORT"));

        // 서버 연결 connect 
        boost::asio::connect(socket, results.begin(), results.end()); 

        // Websocket stream
        websocket::stream<ip::tcp::socket> ws(std::move(socket)); 
        ws.handshake("localhost", "/");

        // 클라이언트에서 서버로 message 보내기 (ws write)
        std::string message = "Hello, server!";
        ws.write(boost::asio::buffer(message));

        // 클라이언트가 서버로부터 응답 받기 (ws read)
        boost::beast::multi_buffer buffer;
        ws.read(buffer);
        std::cout << "Received: " << boost::beast::buffers_to_string(buffer.data()) << std::endl;

        ws.close(websocket::close_code::normal);

    } catch (const boost::beast::system_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
