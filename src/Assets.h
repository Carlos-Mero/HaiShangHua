#ifndef ASSETS_H
#define ASSETS_H

// `Assets.h`和`Assets.cpp`两个文件用于定义一系列易于实现的简单类型。
// 它们会在游戏的许多方面上派上用场，因为实现起来容易且所需代码较少，就不单独新建文件了。

#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/audio_stream.hpp>

namespace godot {

struct GameState {
  // 表示游戏的进度：章节数/步数
  int64_t chapter_;
  int64_t step_;
};

struct MusicRes {
  // 音乐
  String name;
  Ref<AudioStream> content;
};

inline bool operator < (GameState a, GameState b) {
  return a.chapter_ < b.chapter_ || (a.chapter_ == b.chapter_ && a.step_ < b.step_);
}


class FloatingSprite: public Sprite2D {
  GDCLASS(FloatingSprite, Sprite2D);
  // 这个类型用于实现某些地方场景图像的缓慢移动效果

  private:
    double time_accumulated;
    bool is_floating;

  protected:
    static void _bind_methods();

  public:
    FloatingSprite();
    ~FloatingSprite();

    void _process(double delta) override;
    void start_floating() {is_floating = true;}
    void stop_floating() {is_floating = false; set_position(Vector2(0, 0));}
};

class GalleryButton: public Button {
  GDCLASS(GalleryButton, Button);

  private:
    StringName corresponding_img;

  protected:
    static void _bind_methods();

  public:
    GalleryButton();
    ~GalleryButton();
    void set_corresponding_img(const StringName& imgn) {corresponding_img = imgn;}
    StringName get_corresponding_img() const {return corresponding_img;}
    void emit_preview_signal() {emit_signal("call_preview_img", corresponding_img);}
    void _ready() override;
};

class GalleryPreviewContainer: public Control {
  GDCLASS(GalleryPreviewContainer, Control);
  private:

  protected:
    static void _bind_methods();

  public:
    Sprite2D * img_;
    GalleryPreviewContainer();
    ~GalleryPreviewContainer();
    void _ready() override;
    void _unhandled_input(const Ref<InputEvent> &event) override;
    inline void scale_by(float factor) {img_->set_scale(img_->get_scale() * factor);}
};

}

#endif
