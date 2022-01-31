#ifndef GLIB_RENDER_TEXTURE_HPP
#define GLIB_RENDER_TEXTURE_HPP


#include "glib_render_target.hpp"


namespace glib {
  class RenderTexture: public RenderTarget {
    private:
      sf::RenderTexture m_sf_render_texture;
      Texture           m_texture;
    public:
      RenderTexture();
      RenderTexture(const Vector2i& size);
      ~RenderTexture() = default;

      void Resize(const Vector2i& size);
      const Texture& GetTexture();
      const sf::RenderTexture& GetSFMLRenderTexture() const;

      void Display() override;
  };
}


#endif /* glib_render_texture.hpp */
