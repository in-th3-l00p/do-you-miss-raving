//
// Created by intheloop on 11.01.2024.
//

#ifndef RAYGAME_MATH_H
#define RAYGAME_MATH_H

#include <cmath>

#define DEG_TO_RAD(angle) (angle * M_PI / 180.0f)
#define RAD_TO_DEG(angle) (angle * 180.0f / M_PI)

namespace engine::math {
    template <typename T>
    struct Mat2;

    template<typename T>
    struct Vec2 {
        T x, y;

        // Constructor
        Vec2(T x = 0, T y = 0) : x(x), y(y) {}

        [[nodiscard]] Vec2<float> normalize() const {
            float length = std::sqrt(x * x + y * y);
            if (length != 0) {
                return Vec2<float>(x / length, y / length);
            } else {
                // Handle division by zero if the vector is already of zero length
                return Vec2<float>(0, 0);
            }
        }

        Vec2 operator+=(const Vec2& vec) {
            x += vec.x;
            y += vec.y;
            return *this;
        }
        Vec2 operator-=(const Vec2& vec) {
            x -= vec.x;
            y -= vec.y;
            return *this;
        }
        // Operator overloading
        Vec2 operator+(const Vec2& vec) const {
            return Vec2(x + vec.x, y + vec.y);
        }

        Vec2 operator-(const Vec2& vec) const {
            return Vec2(x - vec.x, y - vec.y);
        }

        Vec2 operator*(T scalar) const {
            return Vec2(x * scalar, y * scalar);
        }

        Vec2 operator*(const Mat2<T>& mat) const {
            return Vec2(
                    x * mat.m[0][0] + y * mat.m[1][0],
                    x * mat.m[0][1] + y * mat.m[1][1]
            );
        }

        Vec2 operator/(T scalar) const {
            if (scalar != 0) {
                return Vec2(x / scalar, y / scalar);
            } else {
                return Vec2();
            }
        }

        bool operator==(const Vec2& rhs) const {
            return x == rhs.x && y == rhs.y;
        }

        bool operator!=(const Vec2& rhs) const {
            return *this != rhs;
        }

        [[nodiscard]] float getAngle() const {
            return std::atan2(y, x);
        }

        float getDistance(const Vec2 other) const {
            return std::sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
        }
    };

    template<typename T>
    struct Mat2 {
        T m[2][2];

        // Constructor
        explicit Mat2(T m00 = 1, T m01 = 0, T m10 = 0, T m11 = 1) {
            m[0][0] = m00; m[0][1] = m01;
            m[1][0] = m10; m[1][1] = m11;
        }

        // Operator overloading
        Mat2 operator*(const Mat2& rhs) const {
            Mat2 result;
            for (int i = 0; i < 2; ++i) {
                for (int j = 0; j < 2; ++j) {
                    result.m[i][j] = 0;
                    for (int k = 0; k < 2; ++k) {
                        result.m[i][j] += m[i][k] * rhs.m[k][j];
                    }
                }
            }
            return result;
        }

        Vec2<T> operator*(const Vec2<T>& vec) const {
            return Vec2<T>(
                    m[0][0] * vec.x + m[0][1] * vec.y,
                    m[1][0] * vec.x + m[1][1] * vec.y
            );
        }
    };

    inline Mat2<float> getRotationMatrix(float angle) {
        return Mat2<float>(
                std::cos(angle), -std::sin(angle),
                std::sin(angle), std::cos(angle)
                );
    }

    float getDegrees(const float i_degrees)
    {
        return static_cast<float>(fmod(360 + fmod(i_degrees, 360), 360));
    }

    float degToRad(const float i_degrees)
    {
        return 3.141592653589793116f * getDegrees(i_degrees) / 180;
    }

    template <typename T>
    inline T linearInterpolation(T value, T min1, T max1, T min2, T max2) {
        return (value - min1) * (max2 - min2) / (max1 - min1) + min2;
    }
}

#endif //RAYGAME_MATH_H
