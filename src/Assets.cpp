#include "Assets.h"

#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/classes/input_event_magnify_gesture.hpp>
#include <godot_cpp/classes/input_event_pan_gesture.hpp>
#include <godot_cpp/classes/label.hpp>

#include <cmath>

#ifdef _WIN32
#define M_PI Math_PI
#endif

using namespace godot;

FloatingSprite::FloatingSprite() {
  time_accumulated = 0.0;
}

FloatingSprite::~FloatingSprite() {

}

void FloatingSprite::_bind_methods() {

}

void FloatingSprite::_process(double delta) {
  if (!is_floating) {return;}

  time_accumulated += delta;
  time_accumulated = fmod(time_accumulated, 200 * M_PI);
  set_position(
      Vector2(100.0 * sin(time_accumulated * 0.2) - 100.0,
        100.0 * cos(time_accumulated * 0.15) - 100.0));
  real_t scl_size = fmax(
    (this->get_window()->get_size().x - get_rect().get_position().x) / get_rect().get_size().x,
    (this->get_window()->get_size().y - get_rect().get_position().y) / get_rect().get_size().y);
  set_scale(Vector2(scl_size, scl_size));
}

GalleryButton::GalleryButton() {

}

GalleryButton::~GalleryButton() {

}

void GalleryButton::_bind_methods() {
  ADD_SIGNAL(MethodInfo("call_preview_img", PropertyInfo(Variant::STRING_NAME, "img_name")));
  ClassDB::bind_method(
      D_METHOD("get_img_name"), &GalleryButton::get_corresponding_img);
  ClassDB::bind_method(
      D_METHOD("set_img_name", "img_name"), &GalleryButton::set_corresponding_img);
  ClassDB::add_property(
      "GalleryButton",
      PropertyInfo(Variant::STRING_NAME, "corresponding_img_name"),
      "set_img_name",
      "get_img_name");
  ClassDB::bind_method(
      D_METHOD("emit_preview_signal"), &GalleryButton::emit_preview_signal);
}

void GalleryButton::_ready() {
  connect(
      "pressed", Callable(this, "emit_preview_signal"));
  connect(
      "call_preview_img", Callable(
        get_parent()->get_parent()->get_parent()->get_parent(), "open_preview"));
}

GalleryPreviewContainer::GalleryPreviewContainer() {

}

GalleryPreviewContainer::~GalleryPreviewContainer() {

}

void GalleryPreviewContainer::_bind_methods() {

}

void GalleryPreviewContainer::_ready() {
  img_ = get_node<Sprite2D>("PreviewImage");
  get_node<Label>("Label")->set_position(
      Vector2(get_window()->get_size().x / 2, get_window()->get_size().y - 90)
      - get_node<Label>("Label")->get_size() / 2);
}

void GalleryPreviewContainer::_unhandled_input(const Ref<InputEvent> &event) {
  if (this->is_visible()) {
    if (event->is_class("InputEventPanGesture")) {
      img_->set_position(
        img_->get_position() -
        static_cast<Ref<InputEventPanGesture>>(event)->get_delta() *
        9.6);
    } else if (event->is_class("InputEventMagnifyGesture")) {
      scale_by(
        static_cast<Ref<InputEventMagnifyGesture>>(event)->get_factor());
    }
  }
}
