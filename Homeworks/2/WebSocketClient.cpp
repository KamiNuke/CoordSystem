#include "WebSocketClient.h"
#include <utility>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>
#include <nlohmann/json.hpp>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
using json = nlohmann::json;            // from <nlohmann/json.hpp>

#define LIGHTSPEED 300000

WebSocketClient::WebSocketClient(std::string host, std::string port)
    : m_host{ std::move(host) }, m_port{ std::move(port) }
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
        is_running = true;
        m_thread = std::thread(&WebSocketClient::Run, this);
    }
}

void WebSocketClient::Stop()
{
    is_running = false;
    if (m_thread.joinable())
        m_thread.join();
}

std::list<DockerData> WebSocketClient::GetData() const
{
    std::unique_lock<std::mutex> lock(m_mtx);
    return m_data;
}

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

        // Continuous read loop
        while (is_running)
        {
            beast::flat_buffer buffer;
            ws.read(buffer);
            std::string msg = beast::buffers_to_string(buffer.data());

            JsonMessage(msg);
        }

        ws.close(websocket::close_code::normal);
        std::cout << "Websocket stopped\n";
    }
    catch(std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}



void WebSocketClient::JsonMessage(std::string_view msg)
{
    try
    {
        json j = json::parse(msg);

        if (!j.contains("scanAngle") && !j.contains("echoResponses"))
            throw std::runtime_error("Invalid JSON");

        int scanAngle = j["scanAngle"].get<int>();

        if (scanAngle == lastAngle)
            return;

        lastAngle = scanAngle;

        auto& echoes = j["echoResponses"];

        if (!echoes.is_array())
            throw std::runtime_error("echoREspone isn't an array");


        for (auto& echo : echoes)
        {
            double time = echo["time"];
            double power = echo["power"];
            double distanceKm = LIGHTSPEED * time / 2.0;

            // std::cout << "Angle: " << scanAngle
            //           << "\tDistance: " << distanceKm
            //           << "\tPower: " << power << '\n';

            std::unique_lock<std::mutex> lock(m_mtx);
            m_data.push_back( {scanAngle, power, distanceKm} );

            if (m_data.size() > 1000)
                m_data.pop_front();
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "JSON parse error: " << e.what() << '\n';
    }
}