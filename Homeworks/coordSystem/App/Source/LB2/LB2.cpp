#include "LB2.h"

#include <iostream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;            // from <nlohmann/json.hpp>

#include "CoordinateSystems.h"
#include "imgui.h"
#include "Core/Application.h"


#define PI 3.14
#define RADAR_RANGE 250
#define LIGHTSPEED 300000

namespace App
{
    void LB2::HandleMessage(const std::string& msg)
    {
        try
        {
            json j = json::parse(msg);

            if (!j.contains("scanAngle") && !j.contains("echoResponses"))
                throw std::runtime_error("Invalid JSON");

            int scanAngle = j["scanAngle"].get<int>();

            if (scanAngle == m_lastAngle)
                return;

            m_lastAngle = scanAngle;



            auto& echoes = j["echoResponses"];

            if (echoes.empty())
                return;

            if (!echoes.is_array())
                throw std::runtime_error("echoResponse isn't an array");


            std::lock_guard<std::mutex> lock(m_mtx);
            for (auto& echo : echoes)
            {
                double time = echo["time"];
                double power = echo["power"];
                double distanceKm = LIGHTSPEED * time / 2.0;


                m_data.push_back( {scanAngle, power, distanceKm} );

                if (m_data.size() > 10)
                    m_data.pop_front();
            }
        }
        catch (std::exception& e)
        {
            std::cerr << "JSON parse error: " << e.what() << '\n';
        }
    }

    LB2::LB2()
    {
        websocket = std::make_unique<WebSocketClient>("127.0.0.1", "4000", [this](const std::string& msg)
        {
            HandleMessage(msg);
        });
    }

    LB2::~LB2()
    {
        websocket->Stop();
    }

    void LB2::OnUpdate()
    {
        Layer::OnUpdate();

        std::lock_guard<std::mutex> lock(m_mtx);
        m_dataCopy = m_data;
    }

    void LB2::OnImGuiRender()
    {
        ImGui::Begin("LB2", nullptr, ImGuiWindowFlags_NoCollapse);

        ImGui::Separator();
        WebSocketButton();

        ImGui::Separator();

        ChangeParameters();

        ImGui::Separator();

        // Is this part of code bad? Yes
        // Should I fix it? Definitely yes
        // Will I do it? Absolutely not
        std::vector<double> x_coords;
        std::vector<double> y_coords;

        for (const auto& data : m_dataCopy)
        {
            const double angleRad = data.angle * (PI / 180.0);

            PolarPoint polar(data.distanceKm, angleRad);
            CartesianPoint2D<double> cartesian = CartesianPoint2D<double>::fromPolar(polar);

            x_coords.push_back(cartesian.getX());
            y_coords.push_back(cartesian.getY());
        }

        if (ImPlot::BeginPlot("Radar", ImVec2(-1, -1), ImPlotFlags_Equal))
        {
            ImPlot::SetupAxes("X (km)", "Y (km)");
            ImPlot::SetupAxisLimits(ImAxis_X1, -RADAR_RANGE, RADAR_RANGE);
            ImPlot::SetupAxisLimits(ImAxis_Y1, -RADAR_RANGE, RADAR_RANGE);

            if (!x_coords.empty())
            {
                std::size_t i{ 0 };
                for (const auto& data : m_dataCopy)
                {
                    const float power { static_cast<float>(data.power) };
                    ImVec4 color { 1.0f - power, power, 0.0f, 1.0f };

                    ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle, 5, color, IMPLOT_AUTO, color);
                    ImPlot::PlotScatter("target", &x_coords[i], &y_coords[i], 1);

                    if (ImPlot::IsPlotHovered())
                    {
                        const ImPlotPoint mouse = ImPlot::GetPlotMousePos();
                        const double dx { x_coords[i] - mouse.x };
                        const double dy { y_coords[i] - mouse.y };
                        const double distanceToCenter { std::sqrt(dx*dx + dy*dy) };
                        constexpr double hover_radius { 5.0 };

                        if (distanceToCenter < hover_radius)
                        {
                            ImPlot::Annotation(x_coords[i], y_coords[i], color, ImVec2(10,10), false,
                            "Angle: %d°\nPower: %.2f\nDistance: %.2f km",
                            data.angle, data.power, data.distanceKm);
                        }
                    }

                    ++i;
                }
            }

            ImPlot::EndPlot();
        }


        ImGui::End();
    }

    void LB2::ChangeParameters()
    {
        ImGui::Text("Radar Configuration");

        ImGui::SliderInt("Measurements Per Rotation", &measurementsPerRotation, 10, 1000);
        ImGui::SliderInt("Rotation Speed ", &rotationSpeed, 1, 100);
        ImGui::SliderInt("Target Speed (km/h)", &targetSpeed, 10, 1000);

        if (ImGui::Button("Apply Configuration"))
        {
            std::string cmd = "curl -X PUT http://localhost:4000/config -H \"Content-Type: application/json\" -d '{";
            cmd += "\"measurementsPerRotation\":" + std::to_string(measurementsPerRotation) + ",";
            cmd += "\"rotationSpeed\":" + std::to_string(rotationSpeed) + ",";
            cmd += "\"targetSpeed\":" + std::to_string(targetSpeed);
            cmd += "}'";
            system(cmd.c_str());
        }
    }

    void LB2::WebSocketButton()
    {
        if (ImGui::Button(websocket->IsRunning() ? "Stop WebSocket" : "Start WebSocket"))
        {
            if (!websocket->IsRunning())
                websocket->Start();
            else
                websocket->Stop();
        }

        ImGui::SameLine();
        ImGui::Text(websocket->IsRunning() ? "Status: Running" : "Status: Stopped");
    }
}
