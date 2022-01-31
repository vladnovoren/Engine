#ifndef GLIB_TEXT_HPP
#define GLIB_TEXT_HPP


#include "glib_color.hpp"
#include "glib_tools.hpp"
#include "glib_font.hpp"


namespace glib {
  enum class TextAlign {
    LEFT,
    CENTER,
    RIGHT
  };

  class Text {
   protected:
    TextAlign m_text_align = TextAlign::CENTER;
    sf::Text  m_sf_text;
   public:
    Text() = default;
    Text(const char* label, Font* font);
    ~Text() = default;

    sf::Text GetSFMLText() const;

    void UpdateOrigin();

    void SetAlign(TextAlign align);
    void SetFont(Font* font);
    void SetLabel(const char* label);
    void SetFontSize(unsigned int size);
    void SetColor(const ColorRGBA& color);
    void SetPosition(const Vector2i& position);

    Vector2i GetSize() const;

    void Move(const Vector2i& delta);
  };
}


#endif /* glib_text.hpp */
