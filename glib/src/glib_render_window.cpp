#include "glib_render_window.hpp"


glib::RenderWindow::RenderWindow(const Vector2i& size, const char* title):
                    RenderTarget(&m_sf_render_window) {
  m_sf_render_window.create(sf::VideoMode(size.x, size.y), title);
}


glib::RenderWindow::~RenderWindow() {
  m_sf_render_window.close();
}


void glib::RenderWindow::Resize(const Vector2i& size) {
  m_sf_render_window.setSize(sf::Vector2u(size.x, size.y));
}


bool glib::RenderWindow::PollEvent(sf::Event* sf_event) {
  assert(sf_event != nullptr);

  return m_sf_render_window.pollEvent(*sf_event);
}


void glib::RenderWindow::Display() {
  m_sf_render_window.display();
}


double glib::RenderWindow::ResetTime() {
  return m_sf_clock.restart().asSeconds();
}


sf::RenderWindow* glib::RenderWindow::GetSFMLRenderWindow() {
  return &m_sf_render_window;
}
