#include "glib.hpp"
#include "logging.hpp"

void dura() {
  INIT_LOGGER_TRACE("kok.txt");
  LOGGER_MESSAGE("message1");
}

int main() {
  INIT_LOGGER_TRACE();
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
  dura();
  LOGGER_MESSAGE("message2");
  return 0;
}
