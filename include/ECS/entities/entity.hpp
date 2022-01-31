#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "i_component.hpp"
#include <cassert>
#include <stdexcept>

namespace ECS {
  class Entity {
   protected:
    IComponent* components_[size_t(ComponentId::N_COMPONENTS)] = {};
   public:
    Entity() = default;
    ~Entity();

    void SetComponent(IComponent* component);
    IComponent* GetComponent(ComponentId id);
  };
}

#endif /* entity.hpp */
