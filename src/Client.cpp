#include "Client.hpp"
#include <iostream>

Client::Client(const std::string& host, uint16_t port)
    : socket_(io_context_) {
    boost::asio::ip::tcp::resolver resolver(io_context_);
    boost::asio::connect(socket_, resolver.resolve(host, std::to_string(port)));
}

std::unordered_map<std::string,std::string> Client::sendCommand(const BinaryProtocol::PacketRequest& packet) {
    try {
        auto binaryData = packet.toBinary();

        // Отправляем данные
        boost::asio::write(socket_, boost::asio::buffer(binaryData));

        std::vector<uint8_t> buffer(1024);
        boost::system::error_code error;

        // Читаем ответ сервера
        size_t len = socket_.read_some(boost::asio::buffer(buffer), error);

        if (error == boost::asio::error::eof) {
            std::cout << "Сервер закрыл соединение." << std::endl;
            throw boost::system::system_error(error);
        } else if (error) {
            throw boost::system::system_error(error);
        }

        BinaryProtocol::PacketResponse response = BinaryProtocol::PacketResponse::fromBinary({buffer.begin(), buffer.begin() + len});
        std::unordered_map<std::string,std::string> result;
        std::string buffer_name;
        std::string buffer_value;
        bool isValue = false;
        int i = -1;
        std::string input = response.payload;
        switch (response.header.command)
        {
        case BinaryProtocol::CommandType::OK:
            std::cout << "Ответ от сервера: OK" << std::endl;

            while(input[++i] != '\x03'){
                if (input[i] == '\x01') {
                    if(buffer_value.length())
                    {
                        result[buffer_name] = buffer_value;
                    } 
                    buffer_name.clear();
                    isValue = false;    
                } else if(input[i] == '\x02'){
                    buffer_value.clear();
                    isValue = true;
                } else if(isValue) {
                    buffer_value += input[i];
                } else {
                    buffer_name += input[i];
                }
            }
            result[buffer_name] = buffer_value;
            break;            
          
        default:
            std::cout << "Ответ от сервера: " << static_cast<int>(response.header.command) << std::endl;
            break;
        }
        // Корректно закрываем соединение
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        socket_.close();
        return result;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка клиента: " << e.what() << std::endl;
    }
}

