#include QMK_KEYBOARD_H

#define _MAIN 0
#define _FN 1

#define KC_X0 LT(_FN, KC_ESC)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_MAIN] = LAYOUT_ortho_2x4(
     KC_APPLICATION, KC_HOME, KC_PAGE_UP,   C(KC_INS),
     MO(_FN),        KC_END,  KC_PAGE_DOWN, S(KC_INS)
  ),

  [_FN] = LAYOUT_ortho_2x4(
     KC_F23,  KC_F24,     KC_AUDIO_MUTE, KC_AUDIO_VOL_UP,
     _______, C(KC_F10),  KC_MPLY,       KC_AUDIO_VOL_DOWN
  )
};

/*
const uint16_t PROGMEM test_combo[] = {KC_HOME, KC_END, COMBO_END};
combo_t key_combos[COMBO_COUNT] = {COMBO_ACTION(test_combo)};

void process_combo_event(uint16_t combo_index, bool pressed) {
	backlight_toggle();
}*/

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case _FN:
            backlight_set(1);
            break;
        default: //  for any other layers, or the default layer
            backlight_set(0);
            break;
    }
    return state;
}


#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
}

bool oled_task_user(void) {
  // Host Keyboard Layer Status
  oled_write_ln_P(PSTR("ANAVI Macro Pad 8"), false);
  oled_write_P(PSTR("Active layer: "), false);

  switch (get_highest_layer(layer_state)) {
    case _MAIN:
      oled_write_ln_P(PSTR("Main"), false);
      break;
    case _FN:
      oled_write_ln_P(PSTR("FN"), false);
      break;
    default:
      // Or use the write_ln shortcut over adding '\n' to the end of your string
      oled_write_ln_P(PSTR("N/A"), false);
  }

  // Host Keyboard LED Status
  led_t led_state = host_keyboard_led_state();
  oled_write_P(PSTR("Num Lock: "), false);
  oled_write_ln_P(led_state.num_lock ? PSTR("On") : PSTR("Off"), false);
  oled_write_P(PSTR("Caps Lock: "), false);
  oled_write_ln_P(led_state.caps_lock ? PSTR("On") : PSTR("Off"), false);
  oled_write_P(PSTR("Scroll Lock: "), false);
  oled_write_ln_P(led_state.scroll_lock ? PSTR("On") : PSTR("Off"), false);
  oled_write_P(PSTR("Backlit: "), false);
  oled_write_ln_P(is_backlight_enabled() ? PSTR("On") : PSTR("Off"), false);
#ifdef RGBLIGHT_ENABLE
  static char rgbStatusLine1[26] = {0};
  snprintf(rgbStatusLine1, sizeof(rgbStatusLine1), "RGB Mode: %d", rgblight_get_mode());
  oled_write_ln(rgbStatusLine1, false);
  static char rgbStatusLine2[26] = {0};
  snprintf(rgbStatusLine2, sizeof(rgbStatusLine2), "h:%d s:%d v:%d", rgblight_get_hue(), rgblight_get_sat(), rgblight_get_val());
  oled_write_ln(rgbStatusLine2, false);
#endif
    return false;
}
#endif
