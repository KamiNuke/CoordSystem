#include "LB1.h"

#include <chrono>
#include <random>

#include "CoordinateSystems.h"
#include "imgui.h"
#include "Core/Application.h"

#define ARR_SIZE 100000

namespace App
{
    template <typename T>
    double distance2DCartesian(const CartesianPoint2D<T>& p1, const CartesianPoint2D<T>& p2)
    {
        const double x = p2.getX() - p1.getX();
        const double y = p2.getY() - p1.getY();
        return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
    }

    template <typename T>
    double distance3DCartesian(const CartesianPoint3D<T>& p1, const CartesianPoint3D<T>& p2)
    {
        const double x = p2.getX() - p1.getX();
        const double y = p2.getY() - p1.getY();
        const double z = p2.getZ() - p1.getZ();
        return std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2));
    }

    double distance2DPolar(const PolarPoint& p1, const PolarPoint& p2)
    {
        const double r1 = p1.getRadius();
        const double r2 = p2.getRadius();
        const double theta = p2.getTheta() - p1.getTheta();

        return std::sqrt(std::pow(r1, 2) + std::pow(r2, 2) - 2 * r1 * r2 * std::cos(theta));
    }


    double distance3DChord(const SphericalPoint& p1, const SphericalPoint& p2)
    {
        const double r1 = p1.getRadius();
        const double r2 = p2.getRadius();
        const double theta1 = p1.getPolarAngle();
        const double theta2 = p2.getPolarAngle();
        const double phi1 = p1.getTheta();
        const double phi2 = p2.getTheta();


        return std::sqrt(std::pow(r1, 2) + std::pow(r2, 2) - 2 * r1 * r2 * (
                         std::sin(theta1) * std::sin(theta2) * std::cos(phi1 - phi2)
                         + std::cos(theta1) * std::cos(theta2)
        ));
    }

    double distance3DArc(const SphericalPoint& p1, const SphericalPoint& p2)
    {
        const double radius = (p1.getRadius() + p2.getRadius()) / 2.0;

        const double phi1 = p1.getPolarAngle();
        const double phi2 = p2.getPolarAngle();
        const double theta1 = p1.getTheta();
        const double theta2 = p2.getTheta();

        return radius * std::acos(
            std::sin(phi1) * std::sin(phi2)
            * std::cos(theta1 - theta2)
            + std::cos(phi1) * std::cos(phi2)
        );
    }

    void firstPart2D()
    {
        IMGUI_DEBUG_LOG("2D\n");
        const CartesianPoint2D<double> cart2D {5.5, 24.1};
        const PolarPoint pol2D { PolarPoint::fromCartesian(cart2D) };

        IMGUI_DEBUG_LOG("%s", CartesianPoint2D<double>::GetDataAsString(cart2D).c_str());
        IMGUI_DEBUG_LOG("%s", PolarPoint::GetDataAsString(pol2D).c_str());

        IMGUI_DEBUG_LOG("%s", CartesianPoint2D<double>::GetDataAsString(
            CartesianPoint2D<double>::fromPolar(pol2D)
        ).c_str());
        IMGUI_DEBUG_LOG("%s", PolarPoint::GetDataAsString(pol2D).c_str());
    }

    void firstPart3D()
    {
        IMGUI_DEBUG_LOG("3D\n");
        const CartesianPoint3D<double> cart3D {5.5, 24.1, 55};
        const SphericalPoint pol3D { SphericalPoint::fromCartesian(cart3D) };

        IMGUI_DEBUG_LOG("%s\n", CartesianPoint3D<double>::GetDataAsString(cart3D).c_str());
        IMGUI_DEBUG_LOG("%s\n", SphericalPoint::GetDataAsString(pol3D).c_str());

        IMGUI_DEBUG_LOG("%s\n", CartesianPoint3D<double>::GetDataAsString(
            CartesianPoint3D<double>::fromSpherical(pol3D)
            ).c_str());
        IMGUI_DEBUG_LOG("%s\n\n", SphericalPoint::GetDataAsString(pol3D).c_str());
    }

    void secondPart2D()
    {
        const CartesianPoint2D<double> c1 {0.0, 2.0};
        const CartesianPoint2D<double> c2 {3.0, 4.0};
        const PolarPoint p1 { PolarPoint::fromCartesian(c1) };
        const PolarPoint p2 { PolarPoint::fromCartesian(c2) };

        IMGUI_DEBUG_LOG("Cartesian 2D distance:     %f\n", distance2DCartesian(c1, c2));
        IMGUI_DEBUG_LOG("Polar 2D distance:         %f\n", distance2DPolar(p1, p2));
    }

    void secondPart3D()
    {
        const SphericalPoint s1 {10.0, 0.5, 0.0};
        const SphericalPoint s2 {10.0, 1.5, 0.5};
        const CartesianPoint3D c1 = CartesianPoint3D<double>::fromSpherical(s1);
        const CartesianPoint3D c2 = CartesianPoint3D<double>::fromSpherical(s2);

        IMGUI_DEBUG_LOG("3D Chord distance:         %f\n", distance3DChord(s1, s2));
        IMGUI_DEBUG_LOG("3D Arc distance:           %f\n", distance3DArc(s1, s2));
        IMGUI_DEBUG_LOG("3D Cartesian distance:     %f\n\n", distance3DCartesian(c1, c2));
    }

    template <std::size_t N>
    void gen2DPolarData(std::vector<PolarPoint>& p1, std::vector<PolarPoint>& p2, std::mt19937& mt)
    {
        std::uniform_real_distribution<double> range{0.0, 100.0};

        p1.clear();
        p2.clear();
        p1.reserve(N);
        p2.reserve(N);

        for (std::size_t i = 0; i < N; ++i)
        {
            p1.emplace_back(range(mt), range(mt));
            p2.emplace_back(range(mt), range(mt));
        }
    }

    template <std::size_t N>
    void polarToCartesian2D(const std::vector<PolarPoint>& p, std::vector<CartesianPoint2D<double>>& c)
    {
        c.clear();
        c.reserve(N);
        for (std::size_t i = 0; i < N; ++i)
            c.emplace_back(CartesianPoint2D<double>::fromPolar(p[i]));
    }

    void thirdPart2D(std::mt19937& mt)
    {
        std::vector<PolarPoint> p1, p2;
        gen2DPolarData<ARR_SIZE>(p1, p2, mt);

        std::vector<CartesianPoint2D<double>> c1, c2;
        polarToCartesian2D<ARR_SIZE>(p1, c1);
        polarToCartesian2D<ARR_SIZE>(p2, c2);

        IMGUI_DEBUG_LOG("2D benchmark\n");

        // A POLAR
        {
            const auto start = std::chrono::high_resolution_clock::now();
            for (std::size_t i = 0; i < ARR_SIZE; ++i)
                distance2DPolar(p1[i], p2[i]);

            const auto end = std::chrono::high_resolution_clock::now();
            const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            IMGUI_DEBUG_LOG("Polar: %ld ms\n", duration.count());
        }

        // B CARTESIAN
        {
            const auto start = std::chrono::high_resolution_clock::now();
            for (std::size_t i = 0; i < ARR_SIZE; ++i)
                distance2DCartesian(c1[i], c2[i]);

            const auto end = std::chrono::high_resolution_clock::now();
            const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            IMGUI_DEBUG_LOG("Cartesian: %ld ms\n", duration.count());
        }
    }

    template <std::size_t N>
    void gen3DSphericalData(std::vector<SphericalPoint>& s1, std::vector<SphericalPoint>& s2, std::mt19937& mt)
    {
        std::uniform_real_distribution<double> range{0.0, 100.0};

        s1.clear();
        s2.clear();
        s1.reserve(N);
        s2.reserve(N);

        for (std::size_t i = 0; i < N; ++i)
        {
            double radius = range(mt);
            s1.emplace_back(radius, range(mt), range(mt));
            s2.emplace_back(radius, range(mt), range(mt));
        }
    }

    template <std::size_t N>
    void sphericalToCartesian3D(const std::vector<SphericalPoint>& s, std::vector<CartesianPoint3D<double>>& c)
    {
        c.clear();
        c.reserve(N);
        for (std::size_t i = 0; i < N; ++i)
            c.emplace_back(CartesianPoint3D<double>::fromSpherical(s[i]));
    }

    void thirdPart3D(std::mt19937& mt)
    {
        std::vector<SphericalPoint> s1, s2;
        gen3DSphericalData<ARR_SIZE>(s1, s2, mt);

        std::vector<CartesianPoint3D<double>> c1, c2;
        sphericalToCartesian3D<ARR_SIZE>(s1, c1);
        sphericalToCartesian3D<ARR_SIZE>(s2, c2);

        IMGUI_DEBUG_LOG("3D benchmark\n");

        // A chord
        {
            const auto start = std::chrono::high_resolution_clock::now();
            for (std::size_t i = 0; i < ARR_SIZE; ++i)
                distance3DChord(s1[i], s2[i]);

            const auto end = std::chrono::high_resolution_clock::now();
            const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            IMGUI_DEBUG_LOG("Chord: %ld ms\n", duration.count());
        }

        // B Arc
        {
            const auto start = std::chrono::high_resolution_clock::now();
            for (std::size_t i = 0; i < ARR_SIZE; ++i)
                distance3DArc(s1[i], s2[i]);

            const auto end = std::chrono::high_resolution_clock::now();
            const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            IMGUI_DEBUG_LOG("Arc: %ld ms\n", duration.count());
        }

        // C Cartesian
        {
            const auto start = std::chrono::high_resolution_clock::now();
            for (std::size_t i = 0; i < ARR_SIZE; ++i)
                distance3DCartesian(c1[i], c2[i]);

            const auto end = std::chrono::high_resolution_clock::now();
            const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            IMGUI_DEBUG_LOG("Cartesian: %ld ms\n", duration.count());
        }
    }

    void LB1::OnAttach()
    {
        IMGUI_DEBUG_LOG("FIRST PART\n");

        firstPart2D();
        std::cout << std::endl;
        firstPart3D();

        IMGUI_DEBUG_LOG("SECOND PART\n");

        secondPart2D();
        std::cout << std::endl;
        secondPart3D();

        IMGUI_DEBUG_LOG("THIRD PART\n");

        std::mt19937 mt{ std::random_device{}() };
        thirdPart2D(mt);
        std::cout << std::endl;
        thirdPart3D(mt);
    }

    void LB1::OnImGuiRender()
    {
        ImGui::ShowDebugLogWindow();
    }
}
