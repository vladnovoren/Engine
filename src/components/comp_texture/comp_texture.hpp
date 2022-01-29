#ifndef COMP_TEXTURE_HPP
#define COMP_TEXTURE_HPP


#include "i_component.hpp"
#include "glib.hpp"


namespace ECS {
  class GraphicComponent: public IComponent {
    protected:
      glib::RenderTexture texture;
    public:
      const glib::RenderTexture& GetTexture() const;
      void SetTexture(const glib::Texture& other);
      void SetTexture(const glib::Texture& sprites, const glib::IntRect& rect);
  };
}
#endif /* comp_texture.hpp */
