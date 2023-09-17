#ifndef GALLERY_H
#define GALLERY_H

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/scroll_container.hpp>
#include <godot_cpp/classes/resource_preloader.hpp>

#include "Assets.h"

namespace godot {

class Gallery: public Control {
  GDCLASS(Gallery, Control);
  // 这是画廊场景的根结点类型

  private:
    bool activated;
    double moving_status;
    ResourcePreloader * re_loader;
    GalleryPreviewContainer * preview_img;
    Input * input_;

    Sprite2D * BKGL;
    Vector2 BKGL_init_pos;
    Label * des_label;
    Vector2 des_label_init_pos;
    static const Vector2 loffset;

    Sprite2D * BKGR;
    Vector2 BKGR_init_pos;
    ScrollContainer * gallery_container;
    Vector2 gallery_container_init_pos;
    static const Vector2 roffset;

  protected:
    static void _bind_methods();

  public:
    Gallery();
    ~Gallery();

    void _process(double delta) override;
    void _ready() override;

    void set_reloader(ResourcePreloader * loader) {re_loader = loader;}
    bool is_activated() const {return activated;}
    void open_preview(const StringName& img_name);

    void enter();
    void exit();
    // 这两个函数分别用于实现Gallery界面的进入和退出动画
    // 其中需要先判断Gallery的activation_state，如不同则在进入时激活，退出时取消，如相同则不进行任何动作
    // 实现细节上需要增加背景的渐变模糊效果以及非线性动画（即运动先快后慢）

};

}

#endif
