#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "i_component.hpp"

namespace ECS {
  class Entity {
   protected:
    IComponent* components_[size_t(ComponentId::N_COMPONENTS)] = {};
   public:
    
  };
}

#endif /* entity.hpp */
