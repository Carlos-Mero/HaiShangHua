#include "Game.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/variant/color.hpp>
#include <godot_cpp/classes/input_event_action.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/audio_stream_mp3.hpp>

using namespace godot;

Game::Game() {
  input_ = Input::get_singleton();
  loader_ = ResourceLoader::get_singleton();
  os_ = OS::get_singleton();

  entering_black_screen_ = false;
  exiting_black_screen_ = false;
  half_entering_black_screen_ = false;
  is_finished = false;
  is_showing_info = false;
  current_bksn_color = 0x000000ff;
  bkg_name_to = "bar_overview";
}

Game::~Game() {

}

void Game::_bind_methods() {
  ClassDB::bind_method(
      D_METHOD("start_game_from_begining"), &Game::start_game_from_begining);
  ClassDB::bind_method(
      D_METHOD("start_game_from_previous_archival"), &Game::start_game_from_previous_archival);
  ClassDB::bind_method(
      D_METHOD("save_game_state"), &Game::save_game_state);
  ClassDB::bind_method(
      D_METHOD("return_to_main_menu"), &Game::return_to_main_menu);
  ClassDB::bind_method(
      D_METHOD("change_bkg", "bkg_name"), &Game::black_screen_transition);
  ClassDB::bind_method(
      D_METHOD("change_to_intended_bkg"), &Game::change_to_intended_bkg);
  ClassDB::bind_method(
      D_METHOD("change_game_scene"), &Game::change_game_scene);
  ClassDB::bind_method(
      D_METHOD("exit_black_screen"), &Game::exit_black_screen);
  ClassDB::bind_method(
      D_METHOD("play_music", "music_name"), &Game::play_music);
  ClassDB::bind_method(
      D_METHOD("review_chapter_0"), &Game::review_chapter<0>);
  ClassDB::bind_method(
      D_METHOD("review_chapter_1"), &Game::review_chapter<1>);
  ClassDB::bind_method(
      D_METHOD("review_chapter_2"), &Game::review_chapter<2>);
  ClassDB::bind_method(
      D_METHOD("review_chapter_3"), &Game::review_chapter<3>);
  ClassDB::bind_method(
      D_METHOD("half_enter_black_screen"), &Game::half_enter_black_screen);
  ClassDB::bind_method(
      D_METHOD("switch_info"), &Game::switch_info);
}

void Game::_ready() {
  background_ = get_node<FloatingSprite>("background");
  bksn_ = get_node<Sprite2D>("BlackScreen");
  game_play_ = get_node<GamePlay>("GamePlay");
  gallery_ = get_node<Gallery>("Gallery");
  main_menu_ = get_node<Control>("MainMenu");
  bkgloader_ = get_node<ResourcePreloader>("BkgLoader");
  charloader_ = get_node<ResourcePreloader>("CharLoader");
  bksn_countdown = get_node<Timer>("BlackScreenCountDown");
  music_player_ = get_node<AudioStreamPlayer>("MusicPlayer");

  game_play_->set_bkgloader(bkgloader_);
  game_play_->set_charloader(charloader_);
  gallery_->set_reloader(bkgloader_);

  auto start_game_from_begining_button_ = get_node<Button>("MainMenu/start_game_from_begining");
  auto start_game_from_previous_archival_button_ = get_node<Button>("MainMenu/start_game_from_previous_archival");
  auto gallery_button_ = get_node<Button>("MainMenu/gallery");
  // 初始化所需的指针

  // 加载各图像资源
  bkgloader_->add_resource("xincheng_waking_up", loader_->load("media/xincheng_waking_up.jpg"));
  bkgloader_->add_resource("xincheng_reading", loader_->load("media/xincheng_reading.jpeg"));
  bkgloader_->add_resource("bar_overview", loader_->load("media/bar_overview.jpg"));
  bkgloader_->add_resource("bar_inside_clear", loader_->load("media/bar_inside_clear.jpg"));
  bkgloader_->add_resource("bar_inside_1", loader_->load("media/bar_inside_1.jpg"));
  bkgloader_->add_resource("bar_inside_2", loader_->load("media/bar_inside_2.jpg"));
  bkgloader_->add_resource("bar_inside_3", loader_->load("media/bar_inside_3.jpg"));
  bkgloader_->add_resource("bar_inside_4", loader_->load("media/bar_inside_4.jpg"));
  bkgloader_->add_resource("bar_inside_5", loader_->load("media/bar_inside_5.jpg"));
  bkgloader_->add_resource("ling_turning_back", loader_->load("media/ling_turning_back.jpg"));
  bkgloader_->add_resource("ruyi_jp_patron", loader_->load("media/ruyi_jp_patron.jpg"));
  bkgloader_->add_resource("xincheng_flute", loader_->load("media/xincheng_flute.jpg"));
  bkgloader_->add_resource("ling_in_the_bar", loader_->load("media/ling_in_the_bar.jpg"));
  bkgloader_->add_resource("ling_in_the_bar_2", loader_->load("media/ling_in_the_bar_2.jpg"));
  bkgloader_->add_resource("xincheng_ling_hugging", loader_->load("media/xincheng_ling_hugging.jpg"));
  bkgloader_->add_resource("xincheng_ling_hugging_crying", loader_->load("media/xincheng_ling_hugging_crying.jpeg"));
  bkgloader_->add_resource("bar_inside_6", loader_->load("media/bar_inside_6.jpeg"));
  bkgloader_->add_resource("officer_died_on_the_street", loader_->load("media/jp_offier_dies_on_the_street.jpg"));
  bkgloader_->add_resource("bedroom", loader_->load("media/bedroom.jpg"));

  charloader_->add_resource("xincheng_thinking", loader_->load("media/xincheng_thinking.png"));
  charloader_->add_resource("xincheng_normal", loader_->load("media/xincheng_normal.png"));
  charloader_->add_resource("xincheng_evil_smile", loader_->load("media/xincheng_evil_smile.png"));
  charloader_->add_resource("xincheng_firm", loader_->load("media/xincheng_firm.png"));
  charloader_->add_resource("xincheng_gently_smile", loader_->load("media/xincheng_gently_smile.png"));
  charloader_->add_resource("xincheng_happy", loader_->load("media/xincheng_happy.png"));
  charloader_->add_resource("xincheng_serious", loader_->load("media/xincheng_serious.png"));
  charloader_->add_resource("xincheng_sad", loader_->load("media/xincheng_sad.png"));
  charloader_->add_resource("xincheng_serious_2", loader_->load("media/xincheng_serious_2.png"));
  charloader_->add_resource("xincheng_smile", loader_->load("media/xincheng_smile.png"));

  charloader_->add_resource("ling_normal", loader_->load("media/ling_normal.png"));
  charloader_->add_resource("ling_smile", loader_->load("media/ling_smile.png"));
  charloader_->add_resource("ling_firm", loader_->load("media/ling_firm.png"));

  charloader_->add_resource("ruyi_normal", loader_->load("media/ruyi_normal.png"));
  charloader_->add_resource("ruyi_charming", loader_->load("media/ruyi_charming.png"));

  charloader_->add_resource("patron_0", loader_->load("media/patron_0.png"));
  charloader_->add_resource("patron_1", loader_->load("media/patron_1.png"));
  charloader_->add_resource("jp_patron", loader_->load("media/jp_patron.png"));
  charloader_->add_resource("waiter", loader_->load("media/waiter.png"));

  music_set.push_back(MusicRes{"main_menu_bgm", loader_->load("bgm/main_menu.mp3")});
  music_set.push_back(MusicRes{"a_quiet_day", loader_->load("bgm/a_quiet_day.mp3")});
  music_set.push_back(MusicRes{"a_day_in_the_bar", loader_->load("bgm/a_day_in_the_bar.mp3")});
  music_set.push_back(MusicRes{"another_day_in_the_bar", loader_->load("bgm/another_day_in_the_bar.mp3")});
  music_set.push_back(MusicRes{"warm_hugging", loader_->load("bgm/warm_home_town.mp3")});
  music_set.push_back(MusicRes{"departure", loader_->load("bgm/departure.mp3")});

  // 在这里开始我们完成各个节点信号的连接工作
  bksn_countdown->connect(
      "timeout", Callable(this, "change_to_intended_bkg"));
  bksn_countdown->connect(
      "timeout", Callable(this, "exit_black_screen"));
  get_node<Timer>("SceneChangeCountDown")->connect(
      "timeout", Callable(this, "change_game_scene"));
  get_node<Button>("MainMenu/InfoButton")->connect(
      "pressed", Callable(this, "switch_info"));
  get_node<Label>("MainMenu/Info")->set_position(Vector2(899, 616));
  game_play_->connect(
      "call_change_bkg", Callable(this, "change_bkg"));
  game_play_->connect(
      "call_play_music", Callable(this, "play_music"));
  start_game_from_begining_button_->connect(
      "pressed",Callable(this, "start_game_from_begining"));
  start_game_from_previous_archival_button_->connect(
      "pressed",Callable(this, "start_game_from_previous_archival"));
  gallery_button_->connect(
      "pressed",Callable(gallery_, "enter_gallery"));
  gallery_button_->connect(
      "pressed",Callable(this, "half_enter_black_screen"));
  //

  // 初始化黑屏
  bksn_img = Image::create(
      get_window()->get_size().x,
      get_window()->get_size().y,
      false,
      Image::FORMAT_RGBA8);
  bksn_img->fill(Color::hex(current_bksn_color));
  bksn_imgt = ImageTexture::create_from_image(bksn_img);
  bksn_->set_texture(bksn_imgt);
  bksn_->set_position(get_window()->get_size() / 2);

  // 设置音乐为循环播放
  for (const auto& MR: music_set) {
    static_cast<Ref<AudioStreamMP3>>(MR.content)->set_loop(true);
  }

  // 进入封面图像：bar_overview
  black_screen_transition("bar_overview");
  play_music("main_menu_bgm");
}

void Game::_gui_input(const Ref<InputEvent> &event) {
  // 这个函数用于处理未被子节点接收的输入事件
}

void Game::_process(double delta) {
  // 这里我们用于处理黑屏的进入与退出功能
  if (entering_black_screen_) {
    entering_black_screen(delta);
  }
  if (half_entering_black_screen_) {
    half_entering_black_screen(delta);
  }
  if (exiting_black_screen_) {
    exiting_black_screen(delta);
  }
  if (is_showing_info) {
    if (input_->is_action_just_pressed("ui_accept")) {
      switch_info();
    }
  }
 
}

void Game::start_game_from_begining() {
  GamePlay::is_reviewing = false;
  game_play_->set_game_state(GameState{0, 0});
  enter_black_screen();
  get_node<Timer>("SceneChangeCountDown")->start();

  // 初始化结束后立刻进入第一段情节
  game_play_->step();
}

void Game::start_game_from_previous_archival() {
  GamePlay::is_reviewing = false;
  game_play_->set_game_state(save_state);
  enter_black_screen();
  get_node<Timer>("SceneChangeCountDown")->start();

  // 初始化结束后立刻进入第一段情节
  game_play_->step();
}

void Game::save_game_state() {

}

void Game::return_to_main_menu() {
  enter_black_screen();
  bkg_name_to = "bar_overview";
  save_state = game_play_->get_game_state();
  save_game_state();
  play_music("main_menu_bgm");
  get_node<Timer>("SceneChangeCountDown")->start();
}

void Game::entering_black_screen(double delta) {
  current_bksn_color += static_cast<int32_t>(delta * bksn_change_spd);
  if (current_bksn_color >= 0x000000ff) {
    current_bksn_color = 0x000000ff;
    entering_black_screen_ = false;
  }
  bksn_img->fill(Color::hex(current_bksn_color));
  bksn_imgt->update(bksn_img);
  bksn_->set_texture(bksn_imgt);
}

void Game::half_entering_black_screen(double delta) {
  current_bksn_color += static_cast<int32_t>(delta * bksn_change_spd);
  if (current_bksn_color >= 0x000000dd) {
    current_bksn_color = 0x000000dd;
    half_entering_black_screen_ = false;
  }
  bksn_img->fill(Color::hex(current_bksn_color));
  bksn_imgt->update(bksn_img);
  bksn_->set_texture(bksn_imgt);
}

void Game::exiting_black_screen(double delta) {
  current_bksn_color -= static_cast<int32_t>(delta * bksn_change_spd);
  if (current_bksn_color <= 0x00000000) {
    current_bksn_color = 0x00000000;
    exiting_black_screen_ = false;
  }
  bksn_img->fill(Color::hex(current_bksn_color));
  bksn_imgt->update(bksn_img);
  bksn_->set_texture(bksn_imgt);
}

void Game::black_screen_transition(const StringName& bkg_name) {
  if (bkg_name == StringName("None")) {
    enter_black_screen();
    return;
  } else if (current_bksn_color == 0x000000ff) {
    bkg_name_to = bkg_name;
    change_bkg(bkg_name);
    exit_black_screen();
    return;
  }
  if (bkg_name != bkg_name_to) {
    enter_black_screen();
    bkg_name_to = bkg_name;
    bksn_countdown->start();
  }
}

void Game::play_music(const StringName& music_name) {
  if (current_bgm_name == music_name) {
    return;
  } else {
    for (const auto& MR: music_set) {
      if (MR.name == music_name) {
        music_player_->set_stream(MR.content);
        current_bgm_name = MR.name;
        music_player_->play();
      }
    }
  }
}

void Game::switch_info() {
  is_showing_info = !is_showing_info;
  get_node<Sprite2D>("MainMenu/RectDarkTrans")->set_visible(is_showing_info);
  get_node<Label>("MainMenu/Info")->set_visible(is_showing_info);
}
