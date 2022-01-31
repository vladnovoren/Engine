#ifndef TEXTURE_PACK_HPP
#define TEXTURE_PACK_HPP


#include "glib.hpp"
#include <unordered_map>


namespace texture_pack {
  class TexturePack {
    private:
      glib::Texture sprites_;
      std::vector<std::vector<glib::IntRect>> actions_;
    public:
      TexturePack(const std::string& sprites_file, const std::string& map_file);
      ~TexturePack();

      const glib::Texture& GetSprites() const;
      const glib::IntRect& GetRect(size_t action_index, size_t frame_index) const;
      size_t GetFramesNumber(size_t action_index) const;
  };
}

#endif /* texture_pack.hpp */