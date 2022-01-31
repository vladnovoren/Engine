#include "glib_events.hpp"


glib::Event::Event(EventType type): m_type(type) {}


glib::EventType glib::Event::Type() const {
  return m_type;
}


glib::Event::~Event() = default;


glib::MouseButtonEvent::MouseButtonEvent(const Vector2i& position):
                        m_position(position) {}


glib::MouseButtonEvent::~MouseButtonEvent() = default;


glib::LeftMouseButtonPressedEvent::LeftMouseButtonPressedEvent() {
  m_type = EventType::LEFT_MOUSE_BUTTON_PRESSED;
}


glib::LeftMouseButtonPressedEvent::LeftMouseButtonPressedEvent(const glib::Vector2i& position):
                                   MouseButtonEvent(position) {
  m_type = EventType::LEFT_MOUSE_BUTTON_PRESSED;
}


glib::RightMouseButtonPressedEvent::RightMouseButtonPressedEvent() {
  m_type = EventType::RIGHT_MOUSE_BUTTON_PRESSED;
}


glib::RightMouseButtonPressedEvent::RightMouseButtonPressedEvent(const glib::Vector2i& position):
                                    MouseButtonEvent(position) {
  m_type = EventType::RIGHT_MOUSE_BUTTON_PRESSED;
}


glib::LeftMouseButtonReleasedEvent::LeftMouseButtonReleasedEvent() {
  m_type = EventType::LEFT_MOUSE_BUTTON_RELEASED;
}


glib::LeftMouseButtonReleasedEvent::LeftMouseButtonReleasedEvent(const glib::Vector2i& position):
                                    MouseButtonEvent(position) {
  m_type = EventType::LEFT_MOUSE_BUTTON_RELEASED;
}


glib::RightMouseButtonReleasedEvent::RightMouseButtonReleasedEvent() {
  m_type = EventType::RIGHT_MOUSE_BUTTON_RELEASED;
}


glib::RightMouseButtonReleasedEvent::RightMouseButtonReleasedEvent(const glib::Vector2i& position):
                                     MouseButtonEvent(position) {
  m_type = EventType::RIGHT_MOUSE_BUTTON_RELEASED;
}


glib::CloseSysWindowEvent::CloseSysWindowEvent() {
  m_type = EventType::CLOSE_SYS_WINDOW;
}


glib::MouseMoveEvent::MouseMoveEvent() {
  m_type = EventType::MOUSE_MOVE;
}


glib::MouseMoveEvent::MouseMoveEvent(const Vector2i& new_position):
                      m_new_position(new_position) {
  m_type = EventType::MOUSE_MOVE;
}


glib::LeftMouseDragEvent::LeftMouseDragEvent() {
  m_type = EventType::LEFT_MOUSE_DRAG;
}


glib::LeftMouseDragEvent::LeftMouseDragEvent(const Vector2i& new_position):
                          MouseMoveEvent(new_position) {
  m_type = EventType::LEFT_MOUSE_DRAG;
}


glib::RightMouseDragEvent::RightMouseDragEvent() {
  m_type = EventType::RIGHT_MOUSE_DRAG;
}


glib::RightMouseDragEvent::RightMouseDragEvent(const Vector2i& new_position):
                          MouseMoveEvent(new_position) {
  m_type = EventType::RIGHT_MOUSE_DRAG;
}
