#ifndef LB2_WEBSOCKETCLIENT_H
#define LB2_WEBSOCKETCLIENT_H
#include <string>
#include <atomic>
#include <thread>
#include <mutex>
#include <list>

struct DockerData
{
    int angle;
    double power;
    double distanceKm;
};

class WebSocketClient
{
public:
    WebSocketClient(std::string host, std::string port);
    ~WebSocketClient();

    void Start();
    void Stop();

    [[nodiscard]] std::list<DockerData> GetData() const;
private:
    void Run();
    void JsonMessage(std::string_view msg);

private:
    std::string m_host, m_port;
    std::thread m_thread;
    mutable std::mutex m_mtx;
    std::atomic_bool is_running{ false };
    std::list<DockerData> m_data;
    int lastAngle{-1};
};

#endif //LB2_WEBSOCKETCLIENT_H