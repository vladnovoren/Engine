#ifndef CLOCK_HPP
#define CLOCK_HPP


#include "SFML/Graphics.hpp"


namespace glib {
  class Clock {
    sf::Clock m_sf_clock;
   public:
    double GetElapsedTime();
    double Restart();
  };
}


#endif /* glib_clock.hpp */
