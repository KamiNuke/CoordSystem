#ifndef COORDSYSTEM_WINDOW_H
#define COORDSYSTEM_WINDOW_H
#include <string>
#include <cstdint>
#include <SDL3/SDL.h>

namespace Core
{
    struct WindowSpecification
    {
        std::string title;
        uint32_t width = 1280;
        uint32_t height = 720 ;
        bool isResizable = true;
        bool VSync = true;
    };

    class Window
    {
    public:
        Window(const WindowSpecification& specification = WindowSpecification());
        ~Window();

        void Create();
        void Destroy();

        void Update();

        SDL_Window* GetNativeWindow() const { return m_window; }
        SDL_GLContext GetContext() const { return m_gl_context; }
    private:
        WindowSpecification m_Specification;
        SDL_GLContext m_gl_context;
        SDL_Window* m_window = nullptr;
    };
}



#endif //COORDSYSTEM_WINDOW_H