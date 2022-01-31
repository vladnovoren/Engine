#include "comp_position.hpp"

ECS::Position::Position(): IComponent(ComponentId::POSITION) {}

glib::Vector2d ECS::Position::GetValue() const {
  return value_;
}

void ECS::Position::SetValue(const glib::Vector2d& value) {
  value_ = value;
}
