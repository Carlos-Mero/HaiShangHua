#include "Gallery.h"
#include "Assets.h"
#include "Game.h"

#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_action.hpp>
#include <godot_cpp/classes/window.hpp>

#include <cmath>

using namespace godot;

const Vector2 Gallery::loffset = Vector2(-139, 0);
const Vector2 Gallery::roffset = Vector2(1594, 460);

Gallery::Gallery() {
  //activated = true;
  // FIXME DEBUG ONLY
  activated = false;
  moving_status = 1.0;
  BKGL_init_pos = Vector2(-97, 265);
  des_label_init_pos = Vector2(31, 198);
  BKGR_init_pos = Vector2(1285, 670);
  gallery_container_init_pos = Vector2(430, 220);
}

Gallery::~Gallery() {

}

void Gallery::_bind_methods() {
  ClassDB::bind_method(
      D_METHOD("open_preview", "img_name"), &Gallery::open_preview);
  ClassDB::bind_method(
      D_METHOD("enter_gallery"), &Gallery::enter);
  ClassDB::bind_method(
      D_METHOD("exit_gallery"), &Gallery::exit);
}

void Gallery::_process(double delta) {
  if (!activated) {
    if (moving_status < 1.0) {
        moving_status += delta;
        if (moving_status >= 1.0) {
          moving_status = 1.0;
          set_visible(false);
        }
        BKGL->set_position(BKGL_init_pos + moving_status * moving_status * loffset);
        des_label->set_position(des_label_init_pos + moving_status * moving_status * loffset);
        BKGR->set_position(BKGR_init_pos + moving_status * moving_status * roffset);
        gallery_container->set_position(gallery_container_init_pos + moving_status * moving_status * roffset);
      }
    return;
  }
  if (input_->is_action_just_pressed("ui_accept") ||
      input_->is_action_just_pressed("ui_undo")) {
    preview_img->set_visible(false);
    preview_img->set_size(Vector2(0, 0));
  }
  if (input_->is_action_just_pressed("ui_cancel")) {
    exit();
  }

  if (activated && moving_status > 0.0) {
    moving_status -= delta;
    moving_status = moving_status > 0.0 ? moving_status : 0.0;
    BKGL->set_position(BKGL_init_pos + moving_status * moving_status * loffset);
    des_label->set_position(des_label_init_pos + moving_status * moving_status * loffset);
    BKGR->set_position(BKGR_init_pos + moving_status * moving_status * roffset);
    gallery_container->set_position(gallery_container_init_pos + moving_status * moving_status * roffset);
  }
}

void Gallery::_ready() {
  preview_img = get_node<GalleryPreviewContainer>("PreviewContainer");
  get_node<Button>("BKGL/RectLightTrans/ExitButton")->connect(
      "pressed", Callable(this, "exit_gallery"));
  input_ = Input::get_singleton();

  get_node<Button>("ScrollContainer/VBoxContainer/CPT_1")->connect(
      "pressed", Callable(get_parent(), "review_chapter_0"));
  get_node<Button>("ScrollContainer/VBoxContainer/CPT_2")->connect(
      "pressed", Callable(get_parent(), "review_chapter_1"));
  get_node<Button>("ScrollContainer/VBoxContainer/CPT_3")->connect(
      "pressed", Callable(get_parent(), "review_chapter_2"));
  get_node<Button>("ScrollContainer/VBoxContainer/CPT_4")->connect(
      "pressed", Callable(get_parent(), "review_chapter_3"));

  BKGL = get_node<Sprite2D>("BKGL");
  BKGL->set_position(BKGL_init_pos + loffset);
  des_label = get_node<Label>("Label");
  des_label->set_position(des_label_init_pos + loffset);
  BKGR = get_node<Sprite2D>("BKGR");
  BKGR->set_position(BKGR_init_pos + roffset);
  gallery_container = get_node<ScrollContainer>("ScrollContainer");
  gallery_container->set_position(gallery_container_init_pos + roffset);
}

void Gallery::open_preview(const StringName& img_name) {
  preview_img->set_visible(true);
  preview_img->img_->set_texture(re_loader->get_resource(img_name));
  preview_img->img_->set_position(get_window()->get_size() / 2);
  preview_img->set_size(get_window()->get_size());
  preview_img->img_->set_scale(Vector2(1, 1));
}

void Gallery::enter() {
  if (moving_status != 1.0) {return;}
  activated = true;
  set_visible(true);
}

void Gallery::exit() {
  if (moving_status != 0.0) {return;}
  activated = false;
  preview_img->set_visible(false);
  static_cast<Game*>(get_parent())->exit_black_screen();
}
