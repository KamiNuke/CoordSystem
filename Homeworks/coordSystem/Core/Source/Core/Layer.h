#ifndef COORDSYSTEM_LAYER_H
#define COORDSYSTEM_LAYER_H

#include <SDL3/SDL_events.h>

namespace Core
{
    class Layer
    {
    public:
        virtual ~Layer() = default;

        virtual void OnEvent(SDL_Event& event) {}

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnImGuiRender() {}
        virtual void OnUpdate() {}
        virtual void OnRender() {}

    };
}

#endif //COORDSYSTEM_LAYER_H