#ifndef COORDSYSTEM_LB3_H
#define COORDSYSTEM_LB3_H

#include "Core/Layer.h"
#include "WebSocketClient.h"

#include "imgui.h"

namespace App
{
    class LB3 final : public Core::Layer
    {
    public:
        /**
         * @param id: Унікальний ідентифікатор супутника (UUID).
         * @param xy: Координати супутника у декартових координатах (кілометри).
         * @param sentAt: Час відправки повідомлення супутником (мілісекунди з початку епохи Unix).
         * @param receivedAt: Час отримання повідомлення об'єктом (мілісекунди з початку епохи Unix).
         */
        struct SatelliteData
        {
            float x;
            float y;
            long double sentAt;
            long double receivedAt;

            [[nodiscard]] float GetDistance() const;
        };

        struct ObjectPosition
        {
            float x;
            float y;
        };

    public:
        LB3();
        ~LB3() override;
        void OnUpdate() override;
        void OnImGuiRender() override;

    private:
        void WebSocketButton();
        void ChangeParameters();
        void ShowGPS();
        void OnImPlotHover(const float x, const float y, const ImVec4 color = { 0.0f, 0.0f, 0.0f, 1.0f });
        void HandleMessage(const std::string& msg);

        /**
         * @param Використовується трилитерація
         */
        std::optional<ObjectPosition> CalculateAnalytical();

        /**
         * @param Використовується трилитерація
         */
        std::optional<ObjectPosition> CalculateNumerical();
    private:
        int emulationZoneSize { 200 };
        int messageFrequency { 1 };
        int satelliteSpeed { 120 };
        int objectSpeed { 20 };
        std::vector<std::pair<std::string, SatelliteData>> m_satellitesData;
        std::vector<std::pair<std::string, SatelliteData>> m_satellitesDataCopy;

        std::optional<ObjectPosition> m_analyticalPosition;
        std::optional<ObjectPosition> m_numericalPosition;

        std::unique_ptr<WebSocketClient> websocket;
        mutable std::mutex m_mtx;
    };
}

#endif //COORDSYSTEM_LB3_H
