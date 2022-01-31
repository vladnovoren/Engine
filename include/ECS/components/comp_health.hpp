#ifndef COMP_HEALTH_HPP
#define COMP_HEALTH_HPP

#include "i_component.hpp"

namespace ECS {
  const double MAX_HEALTH = 100;

  class Health: public IComponent {
   protected:
    double value_ = MAX_HEALTH;
   public:
    double GetValue() const;
    void SetValue(double value);

    bool IsAlive() const;
  };
}

#endif /* comp_health.hpp */
