#include "Application.h"

#include <iostream>
#include <cassert>

namespace Core
{
    static Application* s_Application = nullptr;

    Application::Application(const ApplicationSpecification& specification)
    {
        s_Application = this;

        // Setup SDL
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
        {
            std::cerr << "Error: SDL_Init(): " << SDL_GetError() << '\n';
            assert(false);
        }

        // Decide GL+GLSL versions
    #if defined(IMGUI_IMPL_OPENGL_ES2)
        // GL ES 2.0 + GLSL 100 (WebGL 1.0)
        const char* glsl_version = "#version 100";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    #elif defined(IMGUI_IMPL_OPENGL_ES3)
        // GL ES 3.0 + GLSL 300 es (WebGL 2.0)
        const char* glsl_version = "#version 300 es";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    #elif defined(__APPLE__)
        // GL 3.2 Core + GLSL 150
        const char* glsl_version = "#version 150";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    #else
        // GL 3.0 + GLSL 130
        const char* glsl_version = "#version 130";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    #endif

        // Create window with graphics context
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        if (m_Specification.windowSpec.title.empty())
            m_Specification.windowSpec.title = m_Specification.name;

        m_window = std::make_shared<Window>(m_Specification.windowSpec);
        m_window->Create();

        m_ImGuiLayer = new ImGuiLayer();
    }

    Application::~Application()
    {
        m_ImGuiLayer->OnDetach();
        m_window->Destroy();
        SDL_Quit();
    }

    void Application::Run()
    {
        m_running = true;

        SDL_Event event;
        while (m_running)
        {
            while (SDL_PollEvent(&event))
            {
                m_ImGuiLayer->OnEvent(event);
                if (SDL_AppEvent(&event))
                    Stop();
            }


            for (const std::unique_ptr<Layer>& layer : m_LayerStack)
                layer->OnUpdate();

            m_ImGuiLayer->Begin();
            for (const std::unique_ptr<Layer>& layer : m_LayerStack)
                layer->OnImGuiRender();
            m_ImGuiLayer->End();

            for (const std::unique_ptr<Layer>& layer : m_LayerStack)
                layer->OnRender();


            m_window->Update();

            ImGuiIO& io = ImGui::GetIO();
            glViewport(0, 0, static_cast<int>(io.DisplaySize.x), static_cast<int>(io.DisplaySize.y));
            glClearColor(0, 0, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT);
        }
    }

    void Application::Stop()
    {
        m_running = false;
    }

    Application& Application::Get()
    {
        assert(s_Application);
        return *s_Application;
    }

    SDL_AppResult Application::SDL_AppEvent(const SDL_Event* event)
    {
        if (event->type == SDL_EVENT_QUIT)
            return SDL_APP_SUCCESS;

        if (event->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event->window.windowID == SDL_GetWindowID(m_window->GetNativeWindow()))
            return SDL_APP_SUCCESS;

        return SDL_APP_CONTINUE;
    }

}