#ifndef GLIB_EVENTS_HPP
#define GLIB_EVENTS_HPP


#include "SFML/Graphics.hpp"
#include "glib_tools.hpp"


namespace glib {
  enum class EventType {
    CLOSE_SYS_WINDOW,
    LEFT_MOUSE_BUTTON_PRESSED,
    LEFT_MOUSE_BUTTON_RELEASED,
    MOUSE_MOVE,
    LEFT_MOUSE_DRAG,
    RIGHT_MOUSE_DRAG,
    RIGHT_MOUSE_BUTTON_PRESSED,
    RIGHT_MOUSE_BUTTON_RELEASED,
    N_EVENT_TYPES,
    UNDEFINED
  };


  class Event {
   protected:
    EventType m_type = EventType::UNDEFINED;
   public:
    Event() = default;
    Event(EventType type);
    virtual ~Event() = 0;

    EventType Type() const;
  };


  class MouseButtonEvent: public Event {
   public:
    Vector2i m_position;

    MouseButtonEvent() = default;
    MouseButtonEvent(const Vector2i& position);
    virtual ~MouseButtonEvent() = 0;
  };


  class LeftMouseButtonPressedEvent: public MouseButtonEvent {
   public:
    LeftMouseButtonPressedEvent();
    LeftMouseButtonPressedEvent(const Vector2i& position);
    ~LeftMouseButtonPressedEvent() override = default;
  };


  class RightMouseButtonPressedEvent: public MouseButtonEvent {
   public:
    RightMouseButtonPressedEvent();
    RightMouseButtonPressedEvent(const Vector2i& position);
    ~RightMouseButtonPressedEvent() override = default;
  };


  class LeftMouseButtonReleasedEvent: public MouseButtonEvent {
   public:
    LeftMouseButtonReleasedEvent();
    LeftMouseButtonReleasedEvent(const Vector2i& position);
    ~LeftMouseButtonReleasedEvent() override = default;
  };


  class RightMouseButtonReleasedEvent: public MouseButtonEvent {
   public:
    RightMouseButtonReleasedEvent();
    RightMouseButtonReleasedEvent(const Vector2i& position);
    ~RightMouseButtonReleasedEvent() override = default;
  };


  class CloseSysWindowEvent: public Event {
   public:
    CloseSysWindowEvent();
    ~CloseSysWindowEvent() override = default;
  };


  class MouseMoveEvent: virtual public Event {
   public:
    Vector2i m_new_position;

    MouseMoveEvent();
    MouseMoveEvent(const Vector2i& new_position);
    ~MouseMoveEvent() override = default;
  };


  class LeftMouseDragEvent: public MouseMoveEvent {
   public:
    LeftMouseDragEvent();
    LeftMouseDragEvent(const Vector2i& new_position);
    ~LeftMouseDragEvent() = default;
  };


  class RightMouseDragEvent: public MouseMoveEvent {
   public:
    RightMouseDragEvent();
    RightMouseDragEvent(const Vector2i& new_position);
    ~RightMouseDragEvent() = default;
  };
}


#endif /* glib_events.hpp */
