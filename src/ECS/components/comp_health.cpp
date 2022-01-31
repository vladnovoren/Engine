#include "comp_health.hpp"

double ECS::Health::GetValue() const {
  return value_;
}

void ECS::Health::SetValue(double value) {
  value_ = value;
}

bool ECS::Health::IsAlive() const {
  return value_ > 0;
}
