#include "comp_graphics.hpp"

ECS::GraphicComponent::GraphicComponent(): IComponent(ComponentId::GRAPHIC_COMPONENT) {}

const glib::RenderTexture& ECS::GraphicComponent::GetTexture() const {
    return texture_;
}

void ECS::GraphicComponent::SetTexture(const glib::Texture& other) {
    const glib::Vector2i& other_size{other.GetSize()};
    texture_.Resize(other_size);
    texture_.CopyTexture(other, {0, 0}, {0, 0, other_size.x, other_size.y});
}

void ECS::GraphicComponent::SetTexture(const glib::Texture& sprites, const glib::IntRect& rect) {
    const glib::Vector2i& other_size{rect.m_position.x, rect.m_position.y};
    texture_.Resize(other_size);
    texture_.CopyTexture(sprites, {0, 0}, rect);
}