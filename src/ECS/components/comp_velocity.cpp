#include "comp_velocity.hpp"

ECS::Velocity::Velocity(): IComponent(ComponentId::VELOCITY) {}

glib::Vector2d ECS::Velocity::GetValue() const {
  return value_;
}

void ECS::Velocity::SetValue(const glib::Vector2d& value) {
  value_ = value;
}
