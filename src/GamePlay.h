#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/resource_preloader.hpp>
#include <godot_cpp/classes/input.hpp>

#include "Assets.h"

#include <functional>
#include <vector>

namespace godot {

class GamePlay: public Control {
  GDCLASS(GamePlay, Control);
  // 这是游戏内场景所对应的类型，用于实现游戏进行逻辑，同时操作场景内的画面变动
  // 预计会是实现起来最为困难与复杂的类型了

  private:
    Input * input_;
    static GamePlay * this_ptr;
    static GameState current_state_;
    // 这是当前的游戏进度。
    static ResourcePreloader * bkgloader_;
    static ResourcePreloader * charloader_;

    static Sprite2D * character_left;
    static Sprite2D * character_right;
    static Label * name_;
    static Label * content_;

    static std::vector<std::function<void()>> init_cp0();
    static std::vector<std::function<void()>> init_cp1();
    static std::vector<std::function<void()>> init_cp2();
    static std::vector<std::function<void()>> init_cp3();
    static const std::vector<std::function<void()>> cp0;
    static const std::vector<std::function<void()>> cp1;
    static const std::vector<std::function<void()>> cp2;
    static const std::vector<std::function<void()>> cp3;
    // 这里是游戏的具体章节，我们通过lambda函数数组的方式来进行实现

  protected:
    static void _bind_methods();

  public:
    static bool is_reviewing;

    GamePlay();
    ~GamePlay();
    void set_bkgloader(ResourcePreloader * bkg_) {bkgloader_ = bkg_;}
    void set_charloader(ResourcePreloader * cl_) {charloader_ = cl_;}
    void set_game_state(const GameState gs) {current_state_ = gs;}
    GameState get_game_state() const {return current_state_;}

    void _ready() override;
    void _process(double delta) override;
    void _gui_input(const Ref<InputEvent> &event) override;

    static void ss(
        const String& name,
        const String& dialog,
        const String& background,
        const String& left_character,
        const String& right_character,
        const String& background_music
        );
    // 这里表示的是每一步当中的游戏场景，需要通过lambda函数封装进入章节当中
    void step();
    // 这是游戏当中需要实现的重要方法
    // 表示游戏正常向前推进一步，并依次更新场景等各个相关变量
    void rstep();
    // rstep表示游戏进程后退一步
};

}

#endif
