#ifndef GLIB_FONT_HPP
#define GLIB_FONT_HPP


#include "SFML/Graphics.hpp"
#include "glib_tools.hpp"
#include <cassert>


namespace glib {
  class Font {
   protected:
    sf::Font m_sf_font;
   public:
    Font() = default;
    Font(const char* file_path);

    Font(const Font& other) = delete;
    Font& operator=(const Font& other) = delete;

    bool LoadFromFile(const char* file_path);

    sf::Font* GetSFMLFont();
  };
}


#endif /* glib_font.hpp */
