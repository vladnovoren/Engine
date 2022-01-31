#include "entity.hpp"

ECS::Entity::~Entity() {
  for (size_t i = 0; i < size_t(ComponentId::N_COMPONENTS); ++i) {
    delete components_[i];
  }
}

void ECS::Entity::SetComponent(IComponent* component) {
  assert(component != nullptr);

  int id = static_cast<int>(component->GetId());
  if (id < 0 || id >= static_cast<int>(ComponentId::N_COMPONENTS)) {
    throw std::out_of_range("Bad component id");
  }
  components_[static_cast<size_t>(component->GetId())] = component;
}

ECS::IComponent* ECS::Entity::GetComponent(ComponentId id) {
  int i_id = static_cast<int>(id);
  if (i_id < 0 || i_id >= static_cast<int>(ComponentId::N_COMPONENTS)) {
    throw std::out_of_range("Bad component id");
  }
  return components_[i_id];
}
