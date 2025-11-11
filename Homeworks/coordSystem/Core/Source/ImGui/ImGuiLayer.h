#ifndef COORDSYSTEM_IMGUILAYER_H
#define COORDSYSTEM_IMGUILAYER_H

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

#include "implot.h"
#include "implot_internal.h"

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL3/SDL_opengles2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif

#include "Core/Layer.h"

namespace Core
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() = default;

        void OnAttach() override;
        void OnDetach() override;
        void OnEvent(SDL_Event& event) override;

        void Begin();
        void End();
    private:

    };
}


#endif //COORDSYSTEM_IMGUILAYER_H