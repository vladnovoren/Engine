#include "glib_render_target.hpp"
#include "glib_render_window.hpp"
#include "glib_render_texture.hpp"


glib::RenderTarget::~RenderTarget() = default;


glib::RenderTarget::RenderTarget(sf::RenderTarget* sf_render_target):
                    m_sf_render_target(sf_render_target) {}


void glib::RenderTarget::Clear(const ColorRGBA& color) {
  m_sf_render_target->clear(GLibToSFMLColor(color));
}


void glib::RenderTarget::RenderLine(const glib::IntLine& line,
                                    const ColorRGBA& color,
                                    BlendMode blend_mode) {
  FloatLine float_line(line.m_begin, line.m_end, line.m_thickness);
  glib::Vector2f normal = float_line.m_end - float_line.m_begin;
  normal.Rotate(M_PI / 2);
  normal.Resize(line.m_thickness);

  glib::Vector2f v0 = float_line.m_begin + normal;
  glib::Vector2f v1 = float_line.m_begin - normal;
  glib::Vector2f v2 = float_line.m_end   - normal;
  glib::Vector2f v3 = float_line.m_end   + normal;

  sf::ConvexShape convex;
  convex.setPointCount(4);
  convex.setPoint(0, sf::Vector2f(v0.x, v0.y));
  convex.setPoint(1, sf::Vector2f(v1.x, v1.y));
  convex.setPoint(2, sf::Vector2f(v2.x, v2.y));
  convex.setPoint(3, sf::Vector2f(v3.x, v3.y));
  convex.setFillColor(GLibToSFMLColor(color));

  if (blend_mode == BlendMode::COPY) {
    sf::RenderStates render_states(sf::BlendMode(sf::BlendMode::Factor::One,
                                                 sf::BlendMode::Factor::Zero,
                                                 sf::BlendMode::Equation::Add));
    m_sf_render_target->draw(convex, render_states);
  } else {
    m_sf_render_target->draw(convex);
  }
}


void glib::RenderTarget::RenderRect(const glib::IntRect& rect,
                                    const ColorRGBA& color) {
  sf::RectangleShape sf_rect(sf::Vector2f(rect.m_size.x, rect.m_size.y));
  sf_rect.setPosition(sf::Vector2f(rect.m_position.x, rect.m_position.y));
  sf_rect.setFillColor(glib::GLibToSFMLColor(color));
  m_sf_render_target->draw(sf_rect);
}


void glib::RenderTarget::RenderCircle(const glib::IntCircle& circle,
                                      const ColorRGBA& color,
                                      BlendMode blend_mode) {
  sf::CircleShape sf_circle;
  sf_circle.setOrigin(sf::Vector2f(circle.m_radius, circle.m_radius));
  sf_circle.setRadius(circle.m_radius);
  sf_circle.setPosition(circle.m_center.x, circle.m_center.y);
  sf_circle.setFillColor(GLibToSFMLColor(color));

  if (blend_mode == BlendMode::COPY) {
    sf::RenderStates render_states(sf::BlendMode(sf::BlendMode::Factor::One,
                                                 sf::BlendMode::Factor::Zero,
                                                 sf::BlendMode::Equation::Add));
    m_sf_render_target->draw(sf_circle, render_states);
  } else {
    m_sf_render_target->draw(sf_circle);
  }
}


void glib::RenderTarget::RenderText(Text* text, const Vector2i& position) {
  assert((void*)this != nullptr);
  assert(text != nullptr);
  assert((void*)&position != nullptr);
  assert(m_sf_render_target != nullptr);

  text->SetPosition(position);
  sf::Text sf_text = text->GetSFMLText();
  m_sf_render_target->draw(sf_text);
}


void glib::RenderTarget::CopyTexture(const Texture& texture,
                                     const Vector2i& position,
                                     const IntRect& texture_rect) {
  sf::Sprite sprite(texture.GetSFMLTexture());
  sprite.setPosition(sf::Vector2f(position.x, position.y));
  if (texture_rect.m_size.x > 0 && texture_rect.m_size.y > 0) {
    sprite.setTextureRect(sf::IntRect(texture_rect.m_position.x,
                                      texture_rect.m_position.y,
                                      texture_rect.m_size.x,
                                      texture_rect.m_size.y));
  }
  m_sf_render_target->draw(sprite);
}


void glib::RenderTarget::CopyRenderTexture(const RenderTexture& render_texture,
                                           const Vector2i& position,
                                           const IntRect& render_texture_rect) {
  Texture texture(render_texture.GetSFMLRenderTexture().getTexture());
  CopyTexture(texture, position, render_texture_rect);
}
