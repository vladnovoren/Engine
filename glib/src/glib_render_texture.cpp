#include "glib_render_texture.hpp"


glib::RenderTexture::RenderTexture():
                     RenderTarget(&m_sf_render_texture) {}


glib::RenderTexture::RenderTexture(const Vector2i& size):
                     RenderTarget(&m_sf_render_texture) {
  m_sf_render_texture.create(size.x, size.y);
}


void glib::RenderTexture::Resize(const Vector2i& size) {
  m_sf_render_texture.create(size.x, size.y);
}


const sf::RenderTexture& glib::RenderTexture::GetSFMLRenderTexture() const {
  return m_sf_render_texture;
}


const glib::Texture& glib::RenderTexture::GetTexture() {
  m_texture.SetSfTexture(m_sf_render_texture.getTexture());
  return m_texture;
}


void glib::RenderTexture::Display() {
  m_sf_render_texture.display();
}
