#include "glib_text.hpp"


glib::Text::Text(const char* label, Font* font):
            m_sf_text(label, *(font->GetSFMLFont())) {
  UpdateOrigin();
}


sf::Text glib::Text::GetSFMLText() const {
  return m_sf_text;
}


void glib::Text::UpdateOrigin() {
  Vector2i size = GetSize();
  switch (m_text_align) {
    case TextAlign::LEFT:
      break;
    case TextAlign::CENTER:
      m_sf_text.setOrigin(0.5 * size.x, 0);
      break;
    case TextAlign::RIGHT:
      m_sf_text.setOrigin(size.x, 0);
      break;
  }
}


void glib::Text::SetAlign(TextAlign align) {
  m_text_align = align;
  UpdateOrigin();
}


void glib::Text::SetFont(Font* font) {
  m_sf_text.setFont(*font->GetSFMLFont());
  UpdateOrigin();
}


void glib::Text::SetLabel(const char* label) {
  m_sf_text.setString(label);
  UpdateOrigin();
}


void glib::Text::SetFontSize(unsigned int size) {
  m_sf_text.setCharacterSize(size);
  UpdateOrigin();
}


void glib::Text::SetColor(const ColorRGBA& color) {
  m_sf_text.setFillColor(GLibToSFMLColor(color));
}


glib::Vector2i glib::Text::GetSize() const {
  sf::FloatRect sf_rect = m_sf_text.getGlobalBounds();
  return glib::Vector2i(sf_rect.width, sf_rect.height);
}


void glib::Text::SetPosition(const Vector2i& position) {
  m_sf_text.setPosition(position.x, position.y);
}


void glib::Text::Move(const Vector2i& delta) {
  m_sf_text.move(delta.x, delta.y);
}
