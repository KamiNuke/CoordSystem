#include "Core/Application.h"
#include "LB1/LB1.h"
#include "LB2/LB2.h"
#include "LB3/LB3.h"

int main()
{
    Core::Application app{
        {
            "Coordinate Systems",
        }};

    app.PushLayer<App::LB1>();
    app.PushLayer<App::LB2>();
    app.PushLayer<App::LB3>();
    app.Run();

    return 0;
}
