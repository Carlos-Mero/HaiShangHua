#ifndef GAME_H
#define GAME_H

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/resource_preloader.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/classes/audio_stream_player.hpp>
#include <godot_cpp/classes/audio_stream.hpp>

#include "Assets.h"
#include "GamePlay.h"
#include "Gallery.h"

#include <vector>

namespace godot {

class Game: public Control {
  GDCLASS(Game, Control);
  // 这是游戏的基本类型，所有的宏观变量、操作游戏整体的算法都在这里实现。

  private:
    Input * input_;
    OS * os_;
    ResourceLoader * loader_;
    ResourcePreloader * bkgloader_;
    ResourcePreloader * charloader_;
    AudioStreamPlayer * music_player_;

    FloatingSprite * background_;
    Sprite2D * bksn_;
    Ref<ImageTexture> bksn_imgt;
    Ref<Image> bksn_img;
    bool entering_black_screen_, exiting_black_screen_;
    bool half_entering_black_screen_;
    int32_t current_bksn_color;
    StringName bkg_name_to;
    Timer * bksn_countdown;
    static constexpr double bksn_change_spd = 256 / 0.8;
    // 这是整个游戏里唯一的“背景”节点，各个不同场景下的背景均通过更改其内容来实现
    // 这样做的好处就是可以节省内存，同时省去了很多切换可见性的麻烦
    // 同时我们在这里声明了数个用于辅助实现黑屏功能的变量

    GamePlay * game_play_;
    Gallery * gallery_;
    Control * main_menu_;
    // 这里用于存储各个子场景的指针，并通过它们操作这些节点。
    // 由于游戏体量较小，我们不显式使用动态内存分配，所有的场景都在游戏开始时直接加载到游戏当中。

    GameState historical_state;
    // 这是过去所达到的最远的游戏进度，用于确定画廊当中所展示的内容。
    // 它会在游戏场景非正常行进（非调用step方法）而发生改变时检查并更新。
    GameState save_state;
    // 这是当前的游戏进度存档，在调用存储方法时进行储存和更新，并在调用读取存档时继续从这里开始游戏。
    // 上述两个GameState都需要在_ready函数当中被读取（如有）
    StringName current_bgm_name;
    bool is_finished;
    bool is_showing_info;
    // 表示游戏是否已经通关，同样需要进行存储。
    std::vector<MusicRes> music_set;

  protected:
    static void _bind_methods();

  public:

    inline void change_bkg(const StringName& bkg_name) {
      // 这个函数用于直接地立刻改变当前背景
      background_->set_texture(bkgloader_->get_resource(bkg_name));
      real_t scl_size = fmax(
        (this->get_window()->get_size().x - background_->get_rect().get_position().x) / background_->get_rect().get_size().x,
        (this->get_window()->get_size().y - background_->get_rect().get_position().y) / background_->get_rect().get_size().y);
      background_->set_scale(Vector2(scl_size, scl_size));
    }
    inline void change_to_intended_bkg() {change_bkg(bkg_name_to);}
    inline void change_game_scene() {
      main_menu_->set_visible(!main_menu_->is_visible());
      game_play_->set_visible(!game_play_->is_visible());
      if (main_menu_->is_visible()) {
        change_bkg("bar_overview");
        exit_black_screen();
      }
    }

    Game();
    // 我们会在这里实现Game类当中与引擎无关的成员变量的初始化工作。
    ~Game();

    void finished() {is_finished = true;}

    void _ready() override;
    // 在这里我们进行引擎内部对象的初始化，此时可以安全地使用`get_node`方法初始化指针。
    // 同时我们在这里完成各个信号的连接。
    void _gui_input(const Ref<InputEvent> &event) override;
    // 我们在这里接收并处理各类未被子节点处理的输入事件
    
    void _process(double delta) override;

    void start_game_from_begining();
    // 从头开始，进入游戏内场景
    void start_game_from_previous_archival();
    // save_state开始，进入到游戏内场景
    void return_to_main_menu();
    // 从游戏内场景回到主菜单
    void save_game_state();
    // 用于分别将historical_state和save_state存储到文件

    void enter_black_screen() {exiting_black_screen_ = false; entering_black_screen_ = true;};
    void half_enter_black_screen() {
      exiting_black_screen_ = false; half_entering_black_screen_ = true;
    }
    inline void entering_black_screen(double delta);
    inline void half_entering_black_screen(double delta);
    // 经过0.5秒渐变进入黑屏（若已在黑屏当中则无效果）
    void exit_black_screen() {exiting_black_screen_ = true; entering_black_screen_ = false;};
    inline void exiting_black_screen(double delta);
    // 经过0.5秒渐变离开黑屏（若不在黑屏中则无效果）
    void black_screen_transition(const StringName& bkg_name);
    // 经过0.5秒渐变进入黑屏，后切换背景，再经过0.5秒渐变进入背景

    void play_music(const StringName& music_name);
    // 这个方法用于根据音乐名称播放背景音乐

    void switch_info();

    template<int N>
    void review_chapter() {
      GamePlay::is_reviewing = true;
      game_play_->set_game_state(GameState{N, 0});
      gallery_->exit();
      enter_black_screen();
      get_node<Timer>("SceneChangeCountDown")->start();

      // 初始化结束后立刻进入第一段情节
      game_play_->step();
    }
};

}

#endif
