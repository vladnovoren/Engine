#ifndef I_COMPONENT_HPP
#define I_COMPONENT_HPP

#include <stdint.h>
#include <climits>

namespace ECS {
  enum class ComponentId {
    UNDEFINED = -1,
    POSITION,
    VELOCITY
  };

  class IComponent {
   protected:
    ComponentId id_ = ComponentId::UNDEFINED;

    IComponent(ComponentId id);
   public:
    IComponent() = default;
    ~IComponent() = default;

    ComponentId GetId() const;
  };
}

#endif /* i_component.hpp*/

