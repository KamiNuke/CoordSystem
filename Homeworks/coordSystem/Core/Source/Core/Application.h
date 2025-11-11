#ifndef COORDSYSTEM_APPLICATION_H
#define COORDSYSTEM_APPLICATION_H
#include <string>
#include <memory>
#include <vector>

#include "Window.h"
#include "Layer.h"
#include "ImGui/ImGuiLayer.h"

namespace Core
{
    struct ApplicationSpecification
    {
        std::string name{ "Application" };
        WindowSpecification windowSpec;
    };

    class Application
    {
    public:
        Application(const ApplicationSpecification& specification = ApplicationSpecification());
        ~Application();

        void Run();
        void Stop();

        template <typename TLayer>
        requires(std::is_base_of_v<Layer, TLayer>)
        void PushLayer()
        {
            m_LayerStack.push_back(std::make_unique<TLayer>());
            m_LayerStack[m_LayerStack.size() - 1]->OnAttach();
        }

        static Application& Get();

        Window& GetWindow() const { return *m_window; }
    private:
        SDL_AppResult SDL_AppEvent(const SDL_Event* event);
    private:
        ApplicationSpecification m_Specification;
        std::shared_ptr<Window> m_window;
        bool m_running = false;

        std::vector<std::unique_ptr<Layer>> m_LayerStack;
        ImGuiLayer* m_ImGuiLayer;
    };
}


#endif //COORDSYSTEM_APPLICATION_H