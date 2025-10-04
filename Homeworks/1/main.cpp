#include <iostream>
#include <cmath>
#include <random>
#include <array>
#include <chrono>

constexpr std::size_t ARR_SIZE = 100000;

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

    [[nodiscard]] T getX() const { return m_x; }
    [[nodiscard]] T getY() const { return m_y; }
    [[nodiscard]] T getZ() const { return m_z; }
private:
    const T m_x, m_y, m_z;
};

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
    const CartesianPoint2D<double> cart2D {5.5, 24.1};
    const PolarPoint pol2D { PolarPoint::fromCartesian(cart2D) };

    std::cout << cart2D << '\n' << pol2D << '\n';
    std::cout << CartesianPoint2D<double>::fromPolar(pol2D) << '\n' << pol2D << '\n';
}

void firstPart3D()
{
    const CartesianPoint3D<double> cart3D {5.5, 24.1, 55};
    const SphericalPoint pol3D { SphericalPoint::fromCartesian(cart3D) };
    
    std::cout << cart3D << '\n' << pol3D << '\n';
    std::cout << CartesianPoint3D<double>::fromSpherical(pol3D) << '\n' << pol3D << '\n';
}

void secondPart2D()
{
    const CartesianPoint2D<double> c1 {0.0, 2.0};
    const CartesianPoint2D<double> c2 {3.0, 4.0};
    const PolarPoint p1 { PolarPoint::fromCartesian(c1) };
    const PolarPoint p2 { PolarPoint::fromCartesian(c2) };
    
    std::cout << "Cartesian 2D distance: " << distance2DCartesian(c1, c2) << '\n';
    std::cout << "Polar 2D distance:\t" << distance2DPolar(p1, p2) << '\n';       
}

void secondPart3D()
{
    const SphericalPoint s1 {10.0, 0.5, 0.0};
    const SphericalPoint s2 {10.0, 1.5, 0.5};
    const CartesianPoint3D c1 = CartesianPoint3D<double>::fromSpherical(s1);
    const CartesianPoint3D c2 = CartesianPoint3D<double>::fromSpherical(s2);
    
    std::cout << "3D Chord distance:     " << distance3DChord(s1, s2) << '\n';
    std::cout << "3D Arc distance:       " << distance3DArc(s1, s2) << '\n';
    std::cout << "3D Cartesian distance: " << distance3DCartesian(c1, c2) << '\n';
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

    std::cout << "2D benchmark\n";

    // A POLAR
    {
        const auto start = std::chrono::high_resolution_clock::now();
        for (std::size_t i = 0; i < ARR_SIZE; ++i)
            distance2DPolar(p1[i], p2[i]);

        const auto end = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Polar: " << duration.count() << " ms\n";
    }

    // B CARTESIAN
    {
        const auto start = std::chrono::high_resolution_clock::now();
        for (std::size_t i = 0; i < ARR_SIZE; ++i)
            distance2DCartesian(c1[i], c2[i]);

        const auto end = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Cartesian: " << duration.count() << " ms\n";
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

    std::cout << "3D benchmark\n";

    // A chord
    {
        const auto start = std::chrono::high_resolution_clock::now();
        for (std::size_t i = 0; i < ARR_SIZE; ++i)
            distance3DChord(s1[i], s2[i]);

        const auto end = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Chord: " << duration.count() << " ms\n";
    }

    // B Arc
    {
        const auto start = std::chrono::high_resolution_clock::now();
        for (std::size_t i = 0; i < ARR_SIZE; ++i)
            distance3DArc(s1[i], s2[i]);

        const auto end = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Arc: " << duration.count() << " ms\n";
    }

    // C Cartesian
    {
        const auto start = std::chrono::high_resolution_clock::now();
        for (std::size_t i = 0; i < ARR_SIZE; ++i)
            distance3DCartesian(c1[i], c2[i]);

        const auto end = std::chrono::high_resolution_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Cartesian: " << duration.count() << " ms\n";
    }
}

int main()
{
    std::cout << "FIRST PART\n";

    firstPart2D();
    std::cout << std::endl;
    firstPart3D();

    std::cout << "\nSECOND PART\n";

    secondPart2D();    
    std::cout << std::endl;
    secondPart3D();

    std::cout << "\nTHIRD PART\n";

    std::mt19937 mt{ std::random_device{}() };
    thirdPart2D(mt);
    std::cout << std::endl;
    thirdPart3D(mt);

    return 0;
}
