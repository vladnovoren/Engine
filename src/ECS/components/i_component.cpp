#include "i_component.hpp"

ECS::IComponent::IComponent(ComponentId id): id_(id) {}

ECS::ComponentId ECS::IComponent::GetId() const {
  return id_;
}
