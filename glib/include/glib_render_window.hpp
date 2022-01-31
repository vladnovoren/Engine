#ifndef GLIB_RENDER_WINDOW_HPP
#define GLIB_RENDER_WINDOW_HPP


#include "glib_render_target.hpp"


namespace glib {
  class RenderWindow: public RenderTarget {
    sf::RenderWindow m_sf_render_window;
    sf::Clock m_sf_clock;
  public:
    RenderWindow() = default;
    RenderWindow(const Vector2i& size, const char* title);
    ~RenderWindow() override;

    void Resize(const Vector2i& size);

    void Display() override;
    double ResetTime();

    bool PollEvent(sf::Event* sf_event);

    sf::RenderWindow* GetSFMLRenderWindow();
  };
}


#endif /* glib_render_window.hpp */
