#ifndef COLOR_HPP
#define COLOR_HPP


#include "SFML/Graphics.hpp"
#include "glib_color.hpp"


namespace glib {
  struct ColorRGBA {
    double r = 0;
    double g = 0;
    double b = 0;
    double a = 1;

    ColorRGBA() = default;
    ColorRGBA(double r, double g, double b);
    ColorRGBA(double r, double g, double b, double a);

    ColorRGBA& operator *= (const double mul);
    ColorRGBA& operator *= (const ColorRGBA& right);
    ColorRGBA& operator += (const ColorRGBA& right);
    ColorRGBA operator + (const ColorRGBA& right) const;
    ColorRGBA operator * (const ColorRGBA& right) const;
    ColorRGBA operator * (const double scalar) const;
    friend ColorRGBA operator * (const double scalar, const ColorRGBA& color);
  };

  ColorRGBA operator * (const double scalar, const ColorRGBA& color);

  sf::Color GLibToSFMLColor(const ColorRGBA& color);
}


#endif /* glib_color.hpp */
