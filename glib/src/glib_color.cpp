#include "glib_color.hpp"


glib::ColorRGBA::ColorRGBA(double r, double g, double b):
                 r(r), g(g), b(b) {}


glib::ColorRGBA::ColorRGBA(double r, double g, double b, double a):
                 r(r), g(g), b(b), a(a) {}



glib::ColorRGBA& glib::ColorRGBA::operator *= (const double mul) {
  r *= mul;
  g *= mul;
  b *= mul;
  return *this;
}


glib::ColorRGBA& glib::ColorRGBA::operator *= (const glib::ColorRGBA& right) {
  r *= right.r;
  g *= right.g;
  b *= right.b;
  return *this;
}


glib::ColorRGBA& glib::ColorRGBA::operator += (const glib::ColorRGBA& right) {
  r += right.r;
  g += right.g;
  b += right.b;
  return *this;
}


glib::ColorRGBA glib::ColorRGBA::operator + (const glib::ColorRGBA& right) const {
  return ColorRGBA(
    r + right.r,
    g + right.g,
    b + right.b
  );
}


glib::ColorRGBA glib::ColorRGBA::operator * (const glib::ColorRGBA& right) const {
  return ColorRGBA(
    r * right.r,
    g * right.g,
    b * right.b
  );
}


glib::ColorRGBA glib::ColorRGBA::operator * (const double scalar) const {
  return ColorRGBA(
    r * scalar,
    g * scalar,
    b * scalar
  );
}


glib::ColorRGBA glib::operator * (const double scalar, const glib::ColorRGBA& color) {
  return ColorRGBA(
    scalar * color.r,
    scalar * color.g,
    scalar * color.b
  );
}


sf::Color glib::GLibToSFMLColor(const ColorRGBA& color) {
  return sf::Color(color.r * 255, color.g * 255, color.b * 255, color.a * 255);
}
