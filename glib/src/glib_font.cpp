#include "glib_font.hpp"


glib::Font::Font(const char* file_path) {
  assert(file_path != nullptr);

  LoadFromFile(file_path);
}


bool glib::Font::LoadFromFile(const char* file_path) {
  assert(file_path != nullptr);

  return m_sf_font.loadFromFile(file_path);
}


sf::Font* glib::Font::GetSFMLFont() {
  return &m_sf_font;
}
