// Dear ImGui: standalone example application for SDL3 + OpenGL
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include <iostream>

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
#include <SDL3/SDL.h>

#include "CoordinateSystems.h"
#include "WebSocketClient.h"
#include "third-party/implot/implot.h"
#include "third-party/implot/implot_internal.h"
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL3/SDL_opengles2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif

#define PI 3.14

#include <vector>

ImVec2 GetWindowSize(SDL_Window* window)
{
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    return {static_cast<float>(w), static_cast<float>(h)};
}

// Main code
int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cerr << "<host> <port>\n";
        return EXIT_FAILURE;
    }

    const char* host = argv[1];
    const char* port = argv[2];

    WebSocketClient websocket{host, port};

    // Setup SDL
    // [If using SDL_MAIN_USE_CALLBACKS: all code below until the main loop starts would likely be your SDL_AppInit() function]
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
    {
        printf("Error: SDL_Init(): %s\n", SDL_GetError());
        return EXIT_FAILURE;
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
    float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    SDL_WindowFlags window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_BORDERLESS;
    constexpr const char* window_name{"LABA2"};
    SDL_Window* window = SDL_CreateWindow(window_name, (int)(1280 * main_scale), (int)(800 * main_scale), window_flags);
    if (window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return 1;
    }
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (gl_context == nullptr)
    {
        printf("Error: SDL_GL_CreateContext(): %s\n", SDL_GetError());
        return 1;
    }

    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)
    io.ConfigDpiScaleFonts = true;          // [Experimental] Automatically overwrite style.FontScaleDpi in Begin() when Monitor DPI changes. This will scale fonts but _NOT_ scale sizes/padding for now.
    io.ConfigDpiScaleViewports = true;      // [Experimental] Scale Dear ImGui and Platform Windows when Monitor DPI changes.

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        // [If using SDL_MAIN_USE_CALLBACKS: call ImGui_ImplSDL3_ProcessEvent() from your SDL_AppEvent() function]
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
                done = true;
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // [If using SDL_MAIN_USE_CALLBACKS: all code below would likely be your SDL_AppIterate() function]
        if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
        {
            SDL_Delay(10);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // Set IMGUI window size to viewport size
        ImGui::SetNextWindowSize(GetWindowSize(window));
        ImGui::SetNextWindowPos({0,0});

        // Main Window
        {
            ImGui::Begin("window", nullptr, ImGuiWindowFlags_NoTitleBar);
            ImGui::Text(window_name); // Custom title name

            ImGui::SameLine(ImGui::GetContentRegionAvail().x); // pusht o the right side
            if (ImGui::Button("X"))
                done = true;

            ImGui::Separator();

            // WEBSOCKET IS HERE
            static bool websocketRunning = false;
            if (ImGui::Button(websocketRunning ? "Stop WebSocket" : "Start WebSocket"))
            {
                websocketRunning = !websocketRunning;
                if (websocketRunning)
                    websocket.Start();
                else
                    websocket.Stop();
            }

            ImGui::SameLine();
            ImGui::Text(websocketRunning ? "Status: Running" : "Status: Stopped");

            ImGui::Separator();

            // RADARCONF
            ImGui::Text("Radar Configuration");
            static int measurementsPerRotation { 360 };
            static int rotationSpeed { 60 };
            static int targetSpeed { 100 };

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

            ImGui::Separator();

            std::list<DockerData> list{websocket.GetData()};

            std::vector<double> x_coords;
            std::vector<double> y_coords;

            for (const auto& data : list)
            {
                double angleRad = data.angle * (PI / 180.0);

                PolarPoint polar(data.distanceKm, angleRad);
                CartesianPoint2D<double> cartesian = CartesianPoint2D<double>::fromPolar(polar);

                x_coords.push_back(cartesian.getX());
                y_coords.push_back(cartesian.getY());
            }

            if (ImPlot::BeginPlot("Radar", ImVec2(-1, -1)))
            {
                ImPlot::SetupAxes("X (km)", "Y (km)");
                ImPlot::SetupAxisLimits(ImAxis_X1, -250, 250);
                ImPlot::SetupAxisLimits(ImAxis_Y1, -250, 250);

                if (!x_coords.empty())
                {
                    // Is this code bad? Yes
                    // Should I fix it? Definitely yes
                    // Will I do it? Absolutely not
                    std::size_t i{ 0 };
                    for (const auto& data : list)
                    {
                        const float power { static_cast<float>(data.power) };
                        ImVec4 color { 1.0f - power, power, 0.0f, 1.0f };

                        ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle, 5, color, IMPLOT_AUTO, color);
                        ImPlot::PlotScatter("target", &x_coords[i], &y_coords[i], 1);

                        if (ImPlot::IsPlotHovered())
                        {
                            ImPlotPoint mouse = ImPlot::GetPlotMousePos();
                            double dx = x_coords[i] - mouse.x;
                            double dy = y_coords[i] - mouse.y;
                            double dist = std::sqrt(dx*dx + dy*dy);

                            if (dist < 5.0)
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

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }

        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    // [If using SDL_MAIN_USE_CALLBACKS: all code below would likely be your SDL_AppQuit() function]
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
