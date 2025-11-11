#ifndef LB2_WEBSOCKETCLIENT_H
#define LB2_WEBSOCKETCLIENT_H
#include <string>
#include <atomic>
#include <thread>
#include <mutex>
#include <list>
#include <functional>

class WebSocketClient
{
public:
    /**
     *
     * @param host IP address of the websocket server
     * @param port Port that server uses
     * @param messageFunction Function that's being called during getting a message
     */
    WebSocketClient(std::string host, std::string port, const std::function<void(const std::string&)>& messageFunction);
    ~WebSocketClient();

    void Start();
    void Stop();

    // [[nodiscard]] std::string GetMessage() const;
    [[nodiscard]] bool IsRunning() const { return is_running.load(); }
private:
    void Run();

private:
    std::string m_host, m_port;
    std::thread m_thread;
    // mutable std::mutex m_mtx;
    std::atomic_bool is_running{ false };
    std::function<void(const std::string&)> m_messageFunction;
    // std::string m_message;
};

#endif //LB2_WEBSOCKETCLIENT_H