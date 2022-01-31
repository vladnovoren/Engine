#ifndef GLIB_RENDER_TARGET_HPP
#define GLIB_RENDER_TARGET_HPP


#include "SFML/Graphics.hpp"
#include "glib_primitives.hpp"
#include "glib_texture.hpp"
#include "glib_text.hpp"


namespace glib {
  enum BlendMode {
    ADD,
    COPY
  };

  class RenderTexture;

  class RenderTarget {
    sf::RenderTarget* m_sf_render_target;
  public:
    RenderTarget() = default;
    RenderTarget(sf::RenderTarget* sf_render_target);
    virtual ~RenderTarget() = 0;

    void Clear(const ColorRGBA& color = ColorRGBA());

    void RenderLine(const IntLine& line, const ColorRGBA& color,
                    BlendMode blend_mode = BlendMode::ADD);
    void RenderRect(const IntRect& rect, const ColorRGBA& color);
    void RenderCircle(const IntCircle& circle, const ColorRGBA& color,
                      BlendMode blend_mode = BlendMode::ADD);

    void RenderText(Text* text, const Vector2i& position);

    void CopyTexture(const Texture& texture, const Vector2i& position,
                     const IntRect& texture_rect = IntRect());
    void CopyRenderTexture(const RenderTexture& render_texture,
                           const Vector2i& position,
                           const IntRect& texture_rect = IntRect());

    virtual void Display() = 0;
  };
}


#endif /* glib_render_target.hpp */
