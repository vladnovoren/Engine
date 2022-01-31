#ifndef GLIB_PRIMITIVES_HPP
#define GLIB_PRIMITIVES_HPP


#include "glib_color.hpp"
#include "glib_tools.hpp"


namespace glib {
  template<typename RectType>
  struct Rect {
    Vector2<RectType> m_position;
    Vector2<RectType> m_size;

    Rect() = default;
    Rect(const glib::Vector2<RectType>& position,
         const glib::Vector2<RectType>& size):
    m_position(position), m_size(size) {}
    Rect(const RectType& position_x, const RectType& position_y,
         const RectType& width, const RectType& height):
    m_position(position_x, position_y), m_size(width, height) {}
    Rect(const Rect<float>& other):
    m_position(other.m_position), m_size(other.m_size) {}

    Rect(const Rect<unsigned int>& other):
    m_position(other.m_position), m_size(other.m_size) {}

    Rect(const Rect<int>& other):
    m_position(other.m_position), m_size(other.m_size) {}

    bool IsPointInRect(const glib::Vector2<RectType>& point) const {
      return m_position.x < point.x && point.x < m_position.x + m_size.x &&
             m_position.y < point.y && point.y < m_position.y + m_size.y;
    }

    Rect& operator=(const Rect& other) {
      m_position = other.m_position;
      m_size = other.m_size;
      return *this;
    }
  };


  template<typename RectType>
  sf::Rect<RectType> ToSFMLRect(const Rect<RectType>& rect) {
    return sf::Rect<RectType>(GLibToSFMLVector2(rect.m_position),
                              GLibToSFMLVector2(rect.m_size));
  }


  template<typename CircleType>
  struct Circle {
    Vector2<CircleType> m_center;
    CircleType m_radius;

    Circle() = default;
    Circle(const Vector2<CircleType>& center, const CircleType radius):
    m_center(center), m_radius(radius) {}

    bool IsPointInside(const Vector2<CircleType>& point)  {
      return (m_center.x - point.x) * (m_center.x - point.x) +
             (m_center.y - point.y) * (m_center.y - point.y) <=
             m_radius * m_radius;
    }
  };


  template<typename LineType>
  struct Line {
    Vector2<LineType> m_begin;
    Vector2<LineType> m_end;

    LineType m_thickness;

    Line() = default;
    Line(const Vector2<LineType>& begin, const Vector2<LineType>& end,
         LineType thickness):
    m_begin(begin), m_end(end), m_thickness(thickness) {}
  };


  using IntRect   = Rect<int>;
  using UIntRect  = Rect<unsigned int>;
  using FloatRect = Rect<float>;

  using IntLine   = Line<int>;
  using UIntLine  = Line<unsigned int>;
  using FloatLine = Line<float>;

  using IntCircle   = Circle<int>;
  using UIntCircle  = Circle<unsigned int>;
  using FloatCircle = Circle<float>;
}


#endif /* glib_primitives.hpp */
