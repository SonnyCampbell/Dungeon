#pragma once
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>

class Vec2
{
public:
    Vec2() {}
    float e[2];
    Vec2(float e0, float e1)
    {
        e[0] = e0;
        e[1] = e1;
    }
    inline float x() const { return e[0]; }
    inline float y() const { return e[1]; }

    inline const Vec2 &operator+() const { return *this; }
    inline Vec2 operator-() const { return Vec2(-e[0], -e[1]); }
    inline float operator[](int i) const { return e[i]; }

    inline Vec2 &operator+=(const Vec2 &v2);
    inline Vec2 &operator-=(const Vec2 &v2);
    inline Vec2 &operator*=(const Vec2 &v2);
    inline Vec2 &operator/=(const Vec2 &v2);
    inline Vec2 &operator*=(const float t);
    inline Vec2 &operator/=(const float t);

    inline float length() const
    {
        return sqrt(e[0] * e[0] + e[1] * e[1]);
    }

    inline float squared_length() const
    {
        return e[0] * e[0] + e[1] * e[1];
    }

    inline void make_unit_vector();
    inline Vec2 Vec2::normalized_vector();
    inline void normalize();
    static inline Vec2 min(const Vec2 &v1, const Vec2 &v2);
    static inline Vec2 max(const Vec2 &v1, const Vec2 &v2);
    static inline Vec2 zero() { return Vec2(0.f, 0.f); }
    static inline float dot_product(const Vec2 &v1, const Vec2 &v2);
    static inline Vec2 clamp(const Vec2 &value, const Vec2 &min, const Vec2 &max);

    inline Vec2 major_axis();
};

inline float clip(float n, float lower, float upper)
{
    return std::max(lower, std::min(n, upper));
}

inline Vec2 Vec2::clamp(const Vec2 &value, const Vec2 &min, const Vec2 &max)
{
    return Vec2(clip(value.x(), min.x(), max.x()), clip(value.y(), min.y(), max.y()));
}

inline Vec2 Vec2::major_axis()
{
    auto this_x = this->x();
    auto this_y = this->y();
    auto abs_this_x = abs(this_x);
    auto abs_this_y = abs(this_y);
    if (abs_this_x > abs_this_y)
    {
        return Vec2(this_x / abs_this_x, 0.f);
    }
    else
    {
        return Vec2(0.f, this_y / abs_this_y);
    }
}

inline float Vec2::dot_product(const Vec2 &v1, const Vec2 &v2)
{
    return (v1.x() * v2.x()) + (v1.y() * v2.y());
}

inline Vec2 Vec2::min(const Vec2 &v1, const Vec2 &v2)
{
    return Vec2((double)v1.x() < (double)v2.x() ? v1.x() : v2.x(), (double)v1.y() < (double)v2.y() ? v1.y() : v2.y());
}

inline Vec2 Vec2::max(const Vec2 &v1, const Vec2 &v2)
{
    return Vec2((double)v1.x() > (double)v2.x() ? v1.x() : v2.x(), (double)v1.y() > (double)v2.y() ? v1.y() : v2.y());
}

inline std::istream &operator>>(std::istream &is, Vec2 &t)
{
    is >> t.e[0] >> t.e[1];
    return is;
}

inline std::ostream &operator<<(std::ostream &os, Vec2 &t)
{
    os << t.e[0] << " " << t.e[1];
    return os;
}

inline void Vec2::make_unit_vector()
{
    float k = 1.0 / sqrt(e[0] * e[0] + e[1] * e[1]);
    e[0] = k;
    e[1] = k;
}

inline Vec2 Vec2::normalized_vector()
{
    if (e[0] == 0 && e[1] == 0)
    {
        return Vec2::zero();
    }
    float k = 1.0 / sqrt(e[0] * e[0] + e[1] * e[1]);

    return Vec2(e[0] * k, e[1] * k);
}

inline void Vec2::normalize()
{
    if (e[0] == 0 && e[1] == 0)
    {
        return;
    }
    float k = 1.0 / sqrt(e[0] * e[0] + e[1] * e[1]);
    e[0] *= k;
    e[1] *= k;
}

inline Vec2 operator+(const Vec2 &v1, const Vec2 &v2)
{
    return Vec2(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1]);
}

inline Vec2 operator-(const Vec2 &v1, const Vec2 &v2)
{
    return Vec2(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1]);
}

inline Vec2 operator*(const Vec2 &v1, const Vec2 &v2)
{
    return Vec2(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1]);
}

inline Vec2 operator/(const Vec2 &v1, const Vec2 &v2)
{
    return Vec2(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1]);
}

inline Vec2 operator*(float t, const Vec2 &v)
{
    return Vec2(t * v.e[0], t * v.e[1]);
}
inline Vec2 operator/(Vec2 v, float t)
{
    return Vec2(v.e[0] / t, v.e[1] / t);
}
inline Vec2 operator*(Vec2 v, float t)
{
    return Vec2(t * v.e[0], t * v.e[1]);
}

inline float dot(const Vec2 &v1, const Vec2 &v2)
{
    return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1];
}

inline Vec2 &Vec2::operator+=(const Vec2 &v)
{
    e[0] += v.e[0];
    e[1] += v.e[1];
    return *this;
}

inline Vec2 &Vec2::operator*=(const Vec2 &v)
{
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    return *this;
}

inline Vec2 &Vec2::operator/=(const Vec2 &v)
{
    e[0] /= v.e[0];
    e[1] /= v.e[1];
    return *this;
}

inline Vec2 &Vec2::operator-=(const Vec2 &v)
{
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    return *this;
}

inline Vec2 &Vec2::operator*=(float t)
{
    e[0] *= t;
    e[1] *= t;
    return *this;
}

inline Vec2 &Vec2::operator/=(float t)
{
    float k = 1.0 / t;

    e[0] *= k;
    e[1] *= k;
    return *this;
}

inline Vec2 unit_vector(Vec2 v)
{
    return v / v.length();
}
