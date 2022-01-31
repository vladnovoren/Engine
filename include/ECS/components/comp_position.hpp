#ifndef COMP_POSITION_HPP
#define COMP_POSITION_HPP

#include "i_component.hpp"
#include "glib.hpp"

namespace ECS {
  class Position: public IComponent {
   protected:
    glib::Vector2d value_;
   public:
    Position();
    ~Position() = default;

    glib::Vector2d GetValue() const;
    void SetValue(const glib::Vector2d& value);
  };
}

#endif /* comp_position.hpp */
