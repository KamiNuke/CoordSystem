#ifndef COORDSYSTEM_LB1_H
#define COORDSYSTEM_LB1_H

#include "Core/Layer.h"

namespace App
{
    class LB1 final : public Core::Layer
    {
    public:
        LB1() = default;

        ~LB1() override = default;

        void OnAttach() override;
        void OnImGuiRender() override;
    private:
        bool m_running = true;
    };
}


#endif //COORDSYSTEM_LB1_H