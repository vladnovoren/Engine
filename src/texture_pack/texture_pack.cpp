#include "texture_pack.hpp"

#include <fstream>
#include <iostream>

texture_pack::TexturePack::TexturePack(const std::string &sprites_file, const std::string &map_file) {
  try {
    sprites_.LoadFromFile(sprites_file.c_str());
  } catch (...) { // TODO: insert exception here and throw in load from file
                  // TODO: logging
  }
  std::ifstream map_file_stream;
  try {
    map_file_stream.open(map_file, std::ios::in);
  } catch (...) {
    // TODO: log
  }
  size_t actions_cnt = 0;
  try {
    map_file_stream >> actions_cnt;
  } catch (const std::ifstream::failure &e) {
    // TODO: log
  }
  actions_.resize(actions_cnt);
  for (size_t i = 0; i < actions_cnt; ++i) {
    std::string skip_word;
    map_file_stream >> skip_word;
    size_t frames_cnt = 0;
    actions_[i].resize(frames_cnt);
    for (size_t j = 0; j < frames_cnt; ++j) {
      try {
        map_file_stream >> actions_[i][j].m_position.x >> actions_[i][j].m_position.y >> actions_[i][j].m_size.x >> actions_[i][j].m_size.y;
      } catch (const std::ifstream::failure &e) {
        // TODO: log
      }
    }
  }
  map_file_stream.close();
}

const glib::IntRect &
texture_pack::TexturePack::GetRect(size_t action_index, size_t frame_index) const {
  if (action_index >= actions_.size()) {
    throw std::out_of_range("Action index is out of range");
    // TODO: log
  }
  if (frame_index >= actions_[action_index].size()) {
    throw std::out_of_range("Frame index of action index is out of range");
    // TODO: log
  }
  return actions_[action_index][frame_index];
}

const glib::Texture &texture_pack::TexturePack::GetSprites() const {
  return sprites_;
}

size_t texture_pack::TexturePack::GetFramesNumber(size_t action_index) const {
  if (action_index >= actions_.size()) {
    throw std::out_of_range("Action index is out of range");
    // TODO: log
  }
  return actions_[action_index].size();
}