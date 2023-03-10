#include QMK_KEYBOARD_H

extern keymap_config_t keymap_config;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _BASE 0
#define _FN1 1

bool is_alt_tab_active = false; // ADD this near the begining of keymap.c
bool is_alt_shift_tab_active = false; // ADD this near the begining of keymap.c
uint16_t alt_tab_timer = 0;     // we will be using them soon.

//ALT TAB Encoder Timer
void matrix_scan_user(void) { // The very important timer.
  if (is_alt_tab_active) {
    if (timer_elapsed(alt_tab_timer) > 1000) {
      unregister_code(KC_LALT);
      unregister_code(KC_LSFT);
      is_alt_tab_active = false;
      is_alt_shift_tab_active = false;
    }
  }
};




enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  ALT_TAB,
  ALT_SHIFT_TAB,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_80_with_macro(
    KC_MEDIA_PLAY_PAUSE,          KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,  KC_AUDIO_MUTE,
    LCTL(KC_X),   LCTL(KC_C),   KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_DEL,  KC_BSPC, KC_HOME,
    LCTL(KC_F),   LCTL(KC_V),   KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_END,
    KC_WWW_BACK,   KC_WWW_FORWARD,   KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  KC_PGUP,
    KC_WWW_STOP,   KC_WWW_REFRESH,   KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_UP,   KC_PGDN,
    KC_DEL,   KC_PSCR,  KC_LCTL, KC_LGUI, KC_LALT, MO(1),   KC_SPC,  KC_SPC,           MO(1),   KC_SPC,  KC_RALT, KC_RGUI, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
  ),
  [1] = LAYOUT_80_with_macro(
    _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    RGB_HUI, RGB_HUD, KC_ESC, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______, _______,
    RGB_SAI, RGB_SAD, RGB_TOG, RGB_MOD, _______, KC_UP,   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    RGB_VAI, RGB_VAD, RGB_MODE_PLAIN, _______, KC_LEFT, KC_DOWN, KC_RGHT, _______, _______, _______, _______, _______, _______, _______,          _______, _______,
    _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______
  ),
};

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (get_highest_layer(layer_state|default_layer_state) == 0) {
        if (index == 0) {
            if (clockwise) {
                tap_code(KC_MEDIA_NEXT_TRACK);
            } else {
                tap_code(KC_MEDIA_PREV_TRACK);
            }
        } else if (index == 1) {
            if (clockwise) {
                tap_code(KC_VOLU);
            } else {
                tap_code(KC_VOLD);
            }
        }
    }

    else if (get_highest_layer(layer_state|default_layer_state) == 1) {  /* Layer 1 */
        if (index == 0) {
            if (clockwise) {
                rgblight_increase_hue();
            } else {
                rgblight_decrease_hue();
            }
        } else if (index == 1) {
            if (clockwise) {
                if (!is_alt_tab_active) {
                    is_alt_tab_active = true;
                unregister_code(KC_LSFT);
                register_code(KC_LALT);
                }
                alt_tab_timer = timer_read();
                tap_code(KC_TAB);
            } else {
                if (!is_alt_shift_tab_active) {
                    is_alt_shift_tab_active = true;
                register_code(KC_LALT);
                register_code(KC_LSFT);
                }
                alt_tab_timer = timer_read();
                tap_code(KC_TAB);
            }
        }
    }
    return false;
};

