#ifndef COMP_VELOCITY_HPP
#define COMP_VELOCITY_HPP

#include "i_component.hpp"
#include "glib_primitives.hpp"

namespace ECS {
  class Velocity: public IComponent {
   protected:
    glib::Vector2d value_;
   public:
    Velocity();
    ~Velocity() = default;

    glib::Vector2d GetValue() const;
    void SetValue(const glib::Vector2d& value);
  };
}

#endif /* comp_velocity.hpp */
