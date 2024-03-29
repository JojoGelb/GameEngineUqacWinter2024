#pragma once

#include <simplege/pch/precomp.h>

namespace SimpleGE::Graphic
{
  using enum GFXAPI::KeyCode;

  static const std::unordered_map<std::string, GFXAPI::KeyCode> KeyboardKeys = {
      {"space", KeySpace},
      {"apostrophe", KeyApostrophe},
      {"comma", KeyComma},
      {"minus", KeyMinus},
      {"period", KeyPeriod},
      {"slash", KeySlash},
      {"0", Key0},
      {"1", Key1},
      {"2", Key2},
      {"3", Key3},
      {"4", Key4},
      {"5", Key5},
      {"6", Key6},
      {"7", Key7},
      {"8", Key8},
      {"9", Key9},
      {"semicolon", KeySemicolon},
      {"equal", KeyEqual},
      {"a", KeyA},
      {"b", KeyB},
      {"c", KeyC},
      {"d", KeyD},
      {"e", KeyE},
      {"f", KeyF},
      {"g", KeyG},
      {"h", KeyH},
      {"i", KeyI},
      {"j", KeyJ},
      {"k", KeyK},
      {"l", KeyL},
      {"m", KeyM},
      {"n", KeyN},
      {"o", KeyO},
      {"p", KeyP},
      {"q", KeyQ},
      {"r", KeyR},
      {"s", KeyS},
      {"t", KeyT},
      {"u", KeyU},
      {"v", KeyV},
      {"w", KeyW},
      {"x", KeyX},
      {"y", KeyY},
      {"z", KeyZ},
      {"left bracket", KeyLeftBracket},
      {"backslash", KeyBackslash},
      {"right bracket", KeyRightBracket},
      {"grave accent", KeyGraveAccent},
      {"world 1", KeyWorld1},
      {"world 2", KeyWorld2},
      {"escape", KeyEscape},
      {"enter", KeyEnter},
      {"tab", KeyTab},
      {"backspace", KeyBackspace},
      {"insert", KeyInsert},
      {"delete", KeyDelete},
      {"right", KeyRight},
      {"left", KeyLeft},
      {"down", KeyDown},
      {"up", KeyUp},
      {"page up", KeyPageUp},
      {"page down", KeyPageDown},
      {"home", KeyHome},
      {"end", KeyEnd},
      {"caps lock", KeyCapsLock},
      {"scroll lock", KeyScrollLock},
      {"num lock", KeyNumLock},
      {"print screen", KeyPrintScreen},
      {"pause", KeyPause},
      {"f1", KeyF1},
      {"f2", KeyF2},
      {"f3", KeyF3},
      {"f4", KeyF4},
      {"f5", KeyF5},
      {"f6", KeyF6},
      {"f7", KeyF7},
      {"f8", KeyF8},
      {"f9", KeyF9},
      {"f10", KeyF10},
      {"f11", KeyF11},
      {"f12", KeyF12},
      {"f13", KeyF13},
      {"f14", KeyF14},
      {"f15", KeyF15},
      {"f16", KeyF16},
      {"f17", KeyF17},
      {"f18", KeyF18},
      {"f19", KeyF19},
      {"f20", KeyF20},
      {"f21", KeyF21},
      {"f22", KeyF22},
      {"f23", KeyF23},
      {"f24", KeyF24},
      {"f25", KeyF25},
      {"kp 0", KeyKp0},
      {"kp 1", KeyKp1},
      {"kp 2", KeyKp2},
      {"kp 3", KeyKp3},
      {"kp 4", KeyKp4},
      {"kp 5", KeyKp5},
      {"kp 6", KeyKp6},
      {"kp 7", KeyKp7},
      {"kp 8", KeyKp8},
      {"kp 9", KeyKp9},
      {"kp decimal", KeyKpDecimal},
      {"kp divide", KeyKpDivide},
      {"kp multiply", KeyKpMultiply},
      {"kp subtract", KeyKpSubtract},
      {"kp add", KeyKpAdd},
      {"kp enter", KeyKpEnter},
      {"kp equal", KeyKpEqual},
      {"left shift", KeyLeftShift},
      {"left control", KeyLeftControl},
      {"left alt", KeyLeftAlt},
      {"left super", KeyLeftSuper},
      {"right shift", KeyRightShift},
      {"right control", KeyRightControl},
      {"right alt", KeyRightAlt},
      {"right super", KeyRightSuper},
      {"menu", KeyMenu},
  };

  [[nodiscard]] static inline GFXAPI::KeyCode GetKeyCode(const std::string& key)
  {
    auto mapped = KeyboardKeys.find(key);
    Expects(mapped != KeyboardKeys.end());
    return mapped->second;
  }
} // namespace SimpleGE::Graphic