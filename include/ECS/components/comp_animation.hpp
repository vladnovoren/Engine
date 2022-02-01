#ifndef COMP_ANIMATION_HPP
#define COMP_ANIMATION_HPP

#include "i_component.hpp"

namespace ECS {
class AnimationInfo : public IComponent {
  protected:
    size_t action_index_;
    size_t frame_index_;

  public:
    size_t GetActionIndex() const;
    size_t GetFrameIndex() const;
    void SetActionIndex(size_t new_index);
    void SetFrameIndex(size_t new_index);
};
} // namespace ECS

#endif