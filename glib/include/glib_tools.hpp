#ifndef GLIB_TOOLS_HPP
#define GLIB_TOOLS_HPP


#include <cmath>
#include <cassert>
#include <iostream>
#include "SFML/Graphics.hpp"


#define PRINT_LINE_LOCATION \
printf("file: %s\nfunction: %s\nline: %d\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);



namespace glib {
  size_t RoundUpDiv(const size_t divident, const size_t divider);


  template<typename T>
  T Min(const T& first, const T& second) {
    return first < second ? first : second;
  }


  template<typename T>
  T Max(const T& first, const T& second) {
    return first > second ? first : second;
  }


  template<typename VectorType>
  class Vector2 {
  public:
    friend class Graphics;
    VectorType x = 0;
    VectorType y = 0;
    Vector2() = default;

    Vector2(const Vector2& other) {
      x = other.x;
      y = other.y;
    }

    constexpr Vector2(VectorType x, VectorType y) {
      this->x = x;
      this->y = y;
    }

    double Length() const {
      return sqrt(x * x + y * y);
    }

    Vector2<VectorType> GetProjection(const Vector2<VectorType>& direction) {
      Vector2<VectorType> dir_normalized = direction;
      dir_normalized.Normalize();
      return dir_normalized * (*this * dir_normalized);
    }

    void Normalize() {
      double length = Length();
      if (length) {
        x /= length;
        y /= length;
      }
    }

    void Resize(double new_length) {
      Normalize();
      (*this) *= new_length;
    }

    void Reflect(const Vector2& normal) {
      (*this) = 2 * (normal * *this) * normal - (*this);
    }

    double Angle() const {
      return atan2(y, x);
    }

    void Rotate(const double d_phi) {
      double length = Length();
      double angle = Angle() + d_phi;
      x = cos(angle) * length;
      y = sin(angle) * length;
    }

    Vector2& operator=(const Vector2& other) {
      x = other.x;
      y = other.y;
      return *this;
    }

    Vector2& operator=(Vector2& other) {
      x = other.x;
      y = other.y;
      return *this;
    }

    Vector2& operator += (const Vector2& right) {
      x += right.x;
      y += right.y;
      return *this;
    }

    Vector2& operator -= (const Vector2& right) {
      x -= right.x;
      y -= right.y;
      return *this;
    }

    Vector2& operator *= (const double scalar) {
      x *= scalar;
      y *= scalar;
      return *this;
    }

    Vector2 operator - () const {
      return Vector2(-x, -y);
    }

    bool operator == (const Vector2& right) const {
      return x == right.x && y == right.y;
    }

    Vector2 operator + (const Vector2& right) const {
      return Vector2(
        x + right.x,
        y + right.y
      );
    }

    Vector2 operator - (const Vector2& right) const {
      return Vector2(
        x - right.x,
        y - right.y
      );
    }

    Vector2 operator * (const double scalar) const {
      return Vector2(
        x * scalar,
        y * scalar
      );
    }

    double operator * (const Vector2& right) const {
      return x * right.x +
            y * right.y;
    }

    void Print() const {
      std::cout << x << ' ' << y << '\n';
    }
  };

  template<typename T>
  Vector2<T> operator * (const double scalar, const Vector2<T>& vector) {
    Vector2<T> res = vector;
    res *= scalar;
    return res;
  }

  template<typename VectorType>
  double Vector2Cos(const Vector2<VectorType>& first, const Vector2<VectorType>& second) {
    VectorType numerator = first * second;
    if (!numerator)
      return 0;
    return numerator / (first.Length() * second.Length());
  }

  template<typename VectorType>
  double Vector2Sin(const Vector2<VectorType>& first, const Vector2<VectorType>& second) {
    VectorType cos = Vector2Cos(first, second);
    return sqrt(1 - cos * cos);
  }

  template<typename VectorType>
  sf::Vector2<VectorType> GLibToSFMLVector2(const Vector2<VectorType>& glib_vector2) {
    return sf::Vector2<VectorType>(glib_vector2.x, glib_vector2.y);
  }

  template<typename VectorType>
  Vector2<VectorType> SFMLToGLibVector2(const sf::Vector2<VectorType>& sf_vector2) {
    return Vector2<VectorType>(sf_vector2.x, sf_vector2.y);
  }


  using Vector2i = Vector2<int>;
  using Vector2u = Vector2<unsigned int>;
  using Vector2f = Vector2<float>;
  using Vector2d = Vector2<double>;
}


#endif /* glib_tools.hpp */
