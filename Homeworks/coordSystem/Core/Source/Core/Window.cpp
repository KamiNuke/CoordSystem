#include "Window.h"

#include <iostream>
#include <assert.h>
#include <memory>

Core::Window::Window(const WindowSpecification& specification)
    : m_Specification(specification)
{
}

Core::Window::~Window()
{
    Destroy();
}

void Core::Window::Create()
{
    constexpr SDL_WindowFlags window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
    m_window = SDL_CreateWindow(m_Specification.title.c_str(), m_Specification.width, m_Specification.height, window_flags);
    if (!m_window)
    {
        std::cerr << "Error: SDL_CreateWindow(): " << SDL_GetError() << '\n';
        assert(false);
    }

    m_gl_context = SDL_GL_CreateContext(m_window);
    if (!m_gl_context)
    {
        std::cerr << "Error: SDL_GL_CreateContext(): " << SDL_GetError() << '\n';
        assert(false);
    }

    SDL_GL_MakeCurrent(m_window, m_gl_context);
    SDL_GL_SetSwapInterval(m_Specification.VSync); // Enable vsync
    SDL_SetWindowPosition(m_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(m_window);
}

void Core::Window::Destroy()
{
    SDL_GL_DestroyContext(m_gl_context);

    if (m_window)
        SDL_DestroyWindow(m_window);

    m_window = nullptr;
}

void Core::Window::Update()
{
    SDL_GL_SwapWindow(m_window);
}
