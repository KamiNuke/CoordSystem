#ifndef LB2_COORDINATESYSTEMS_H
#define LB2_COORDINATESYSTEMS_H

#include <cmath>
#include <iostream>



template <typename T>
class CartesianPoint2D;

class PolarPoint
{
public:
    PolarPoint(double radius, double angle)
        : m_radius{ radius }, m_theta{ angle }
    {}

    template <typename T>
    static PolarPoint fromCartesian(const CartesianPoint2D<T>& p)
    {
        const double radius { std::sqrt(std::pow(p.getX(), 2) + std::pow(p.getY(), 2)) };
        const double theta { std::atan2(p.getY(), p.getX()) };
        return { radius, theta };
    }

    friend std::ostream& operator<<(std::ostream& out, const PolarPoint& p)
    {
        out << "radius: " << p.getRadius() << "\ttheta: " << p.getTheta();
        return out;
    }

    static std::string GetDataAsString(const PolarPoint& p)
    {
        return std::format("radius: {:.2f}\ttheta: {:.2f}\n", p.getRadius(), p.getTheta());
    }

    [[nodiscard]] double getRadius() const { return m_radius; }
    [[nodiscard]] double getTheta() const { return m_theta; }
private:
    const double m_radius, m_theta;
};

template <typename T>
class CartesianPoint2D
{
public:
    CartesianPoint2D(T x, T y)
        : m_x{ x }, m_y{ y }
    {}

    static CartesianPoint2D fromPolar(const PolarPoint& p)
    {
        T x = p.getRadius() * std::cos(p.getTheta());
        T y = p.getRadius() * std::sin(p.getTheta());
        return {x, y};
    }

    friend std::ostream& operator<<(std::ostream& out, const CartesianPoint2D& p)
    {
        out << "x: " << p.getX() << "\ty: " << p.getY();
        return out;
    }

    static std::string GetDataAsString(const CartesianPoint2D& p)
    {
        return std::format("x: {:.2f}\ty: {:.2f}\n", p.getX(), p.getY());
    }

    [[nodiscard]] T getX() const { return m_x; }
    [[nodiscard]] T getY() const { return m_y; }
private:
    const T m_x, m_y;
};

template <typename T>
class CartesianPoint3D;

// Radius - це радіус-вектор rho
// azimuth - це азимутальний кут theta
// polarAngle = полярний кут phi
//
class SphericalPoint
{
public:
    SphericalPoint(double radius, double azimuth, double polarAngle)
        : m_rho{ radius }, m_theta{ azimuth }, m_polarAngle{ polarAngle }
    {}

    template <typename T>
    static SphericalPoint fromCartesian(const CartesianPoint3D<T>& p)
    {
        const double radius { std::sqrt(std::pow(p.getX(), 2) + std::pow(p.getY(), 2) + std::pow(p.getZ(), 2)) };

        if(radius == 0)
            return {0.0, 0.0, 0.0};

        const double theta { std::atan2(p.getY(), p.getX()) };
        const double phi { std::acos(p.getZ() / radius) };

        return { radius, theta, phi };
    }

    friend std::ostream& operator<<(std::ostream& out, const SphericalPoint& p)
    {
        out << "radius: " << p.getRadius() << "\ttheta: " << p.getTheta() << "\tphi: " << p.getPolarAngle();
        return out;
    }

    static std::string GetDataAsString(const SphericalPoint& p)
    {
        return std::format("radius: {:.2f}\ttheta: {:.2f}\tphi: {:.2f}", p.getRadius(), p.getTheta(), p.getPolarAngle());
    }

    [[nodiscard]] double getRadius() const { return m_rho; }
    [[nodiscard]] double getTheta() const { return m_theta; }
    [[nodiscard]] double getPolarAngle() const { return m_polarAngle; }
private:
    const double m_rho, m_theta, m_polarAngle;
};

template <typename T>
class CartesianPoint3D
{
public:
    CartesianPoint3D(T x, T y, T z)
        : m_x{ x }, m_y{ y }, m_z{ z }
    {}

    static CartesianPoint3D fromSpherical(const SphericalPoint& p)
    {
        T x = p.getRadius() * std::sin(p.getPolarAngle()) * std::cos(p.getTheta());
        T y = p.getRadius() * std::sin(p.getPolarAngle()) * std::sin(p.getTheta());
        T z = p.getRadius() * std::cos(p.getPolarAngle());
        return {x, y, z};
    }

    friend std::ostream& operator<<(std::ostream& out, const CartesianPoint3D& p)
    {
        out << "x: " << p.getX() << "\ty: " << p.getY() << "\tz: " << p.getZ();
        return out;
    }

    static std::string GetDataAsString(const CartesianPoint3D& p)
    {
        return std::format("x: {:.2f}\ty: {:.2f}\tz: {:.2f}", p.getX(), p.getY(), p.getZ());
    }

    [[nodiscard]] T getX() const { return m_x; }
    [[nodiscard]] T getY() const { return m_y; }
    [[nodiscard]] T getZ() const { return m_z; }

private:
    const T m_x, m_y, m_z;
};

#endif //LB2_COORDINATESYSTEMS_H