#include "glib_texture.hpp"


glib::Texture::Texture(const sf::Texture& sf_texture):
               m_sf_texture(sf_texture) {}


void glib::Texture::SetSfTexture(const sf::Texture& sf_texture) {
  m_sf_texture = sf_texture;
}


bool glib::Texture::LoadFromFile(const char* file_name) {
  if (!m_sf_texture.loadFromFile(file_name)) {
    return false;
  }
  return true;
}


const sf::Texture& glib::Texture::GetSFMLTexture() const {
  return m_sf_texture;
}


glib::Vector2i glib::Texture::GetSize() const {
  sf::Vector2u sf_size = m_sf_texture.getSize();
  return Vector2i(sf_size.x, sf_size.y);
}
