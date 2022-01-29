#include "comp_velocity.hpp"
#include "comp_position.hpp"

int main() {
  ECS::Velocity velocity;
  velocity.SetValue(glib::Vector2d(100, 100));
  glib::Vector2d value = velocity.GetValue();
  printf("%lf %lf\n", value.x, value.y);
  return 0;
}
