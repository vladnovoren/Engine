#include "glib_clock.hpp"


double glib::Clock::GetElapsedTime() {
  return m_sf_clock.getElapsedTime().asSeconds();
}


double glib::Clock::Restart() {
  return m_sf_clock.restart().asSeconds();
}
