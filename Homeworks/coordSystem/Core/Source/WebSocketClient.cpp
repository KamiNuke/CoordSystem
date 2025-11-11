#include "WebSocketClient.h"
#include <utility>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

WebSocketClient::WebSocketClient(std::string host, std::string port, const std::function<void(const std::string&)>& messageFunction)
    : m_host{ std::move(host) }, m_port{ std::move(port) }, m_messageFunction{ messageFunction }
{
}

WebSocketClient::~WebSocketClient()
{
    Stop();
}

void WebSocketClient::Start()
{
    if(!is_running)
    {
        if (m_thread.joinable())
            m_thread.join();

        is_running = true;
        m_thread = std::thread(&WebSocketClient::Run, this);
    }
}

void WebSocketClient::Stop()
{
    is_running.store(false);

    if (m_thread.joinable())
        m_thread.join();
}

// std::string WebSocketClient::GetMessage() const
// {
//     std::unique_lock<std::mutex> lock(m_mtx);
//     return m_message;
// }

void WebSocketClient::Run()
{
    try
    {
        // The io_context is required for all I/O
        net::io_context ioc;

        // These objects perform our I/O
        tcp::resolver resolver{ioc};
        websocket::stream<tcp::socket> ws{ioc};

        // Look up the domain name
        auto const results = resolver.resolve(m_host, m_port);

        // Make the connection on the IP address we get from a lookup
        net::connect(ws.next_layer(), results);

        // Perform the websocket handshake
        ws.handshake(m_host + ":" + m_port, "/");

        is_running = true;

        // Continuous read loop
        while (is_running.load())
        {
            beast::flat_buffer buffer;
            ws.read(buffer);
            std::string msg = beast::buffers_to_string(buffer.data());
            m_messageFunction(msg);

        }

        ws.close(websocket::close_code::normal);
        std::cout << "Websocket stopped\n";
    }
    catch(std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    is_running = false;
}