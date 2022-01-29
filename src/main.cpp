#include "glib.hpp"

int main() {
  glib::RenderWindow window(glib::Vector2i(800, 600), "Revenge");
  sf::Event event;
  bool is_opened = true;
  while (is_opened) {
    while (window.PollEvent(&event)) {
      if (event.type == sf::Event::Closed) {
        is_opened = false;
      }
      window.Clear(glib::ColorRGBA(rand(), rand(), rand(), rand()));
      window.Display();
    }
  }
  return 0;
}
