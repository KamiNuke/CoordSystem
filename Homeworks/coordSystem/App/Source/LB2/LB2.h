#ifndef COORDSYSTEM_LB2_H
#define COORDSYSTEM_LB2_H

#include "Core/Layer.h"
#include "WebSocketClient.h"

namespace App
{
    class LB2 final : public Core::Layer
    {
    public:
        struct DockerData
        {
            int angle;
            double power;
            double distanceKm;
        };
    public:
        LB2();
        ~LB2() override;

        void OnUpdate() override;
        void OnImGuiRender() override;
    private:
        void ChangeParameters();
        void WebSocketButton();
        void HandleMessage(const std::string& msg);
    private:
        int measurementsPerRotation { 360 };
        int rotationSpeed { 60 };
        int targetSpeed { 100 };
    private:
        std::unique_ptr<WebSocketClient> websocket;
        std::list<DockerData> m_data;
        std::list<DockerData> m_dataCopy;
        int m_lastAngle{-1};

        mutable std::mutex m_mtx;
    };
}


#endif //COORDSYSTEM_LB2_H