#include "comp_animation.hpp"

size_t ECS::AnimationInfo::GetActionIndex() const {
    return action_index_;
}

size_t ECS::AnimationInfo::GetFrameIndex() const {
    return frame_index_;
}

void ECS::AnimationInfo::SetActionIndex(size_t new_index) {
    action_index_ = new_index;
}

void ECS::AnimationInfo::SetFrameIndex(size_t new_index) {
    frame_index_ = new_index;
}