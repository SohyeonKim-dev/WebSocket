#include <iostream>
#include <boost/asio.hpp>
#include <boost/beast.hpp>

using namespace boost::asio;
namespace websocket = boost::beast::websocket;

int main() {
    io_service ioservice;
    // port number 8080 설정 
    ip::tcp::acceptor acceptor(ioservice, ip::tcp::endpoint(ip::tcp::v4(), 8080));
    ip::tcp::socket socket(ioservice);

    acceptor.accept(socket);

    try {
        // websocket stream ws 선언 
        websocket::stream<ip::tcp::socket> ws{std::move(socket)}; 
        ws.accept();

        for (;;) {
            boost::beast::multi_buffer buffer;
            ws.read(buffer);
            ws.text(ws.got_text());
            ws.write(buffer.data());
            // 클라이언트로부터 받은 메세지를 다시 돌려보낸다 (에코)
        }
    } catch (const boost::beast::system_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
