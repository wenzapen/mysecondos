#ifndef _KKYBRD_DRIVER_H
#define _KKYBRD_DRIVER_H

#include <stdint.h>

#define         KEY_UNKOWN             0x0
#define  	KEY_SPACE              ' '
#define  	KEY_0                  '0'
#define  	KEY_1                  '1'
#define  	KEY_2                  '2'
#define  	KEY_3                  '3'
#define  	KEY_4                  '4'
#define  	KEY_5                  '5'
#define  	KEY_6                  '6'
#define  	KEY_7                  '7'
#define  	KEY_8                  '8'
#define  	KEY_9                  '9'
  
#define  	KEY_A                  'a'
#define  	KEY_B                  'b'
#define  	KEY_C                  'c'
#define  	KEY_D                  'd'
#define  	KEY_E                  'e'
#define  	KEY_F                  'f'
#define  	KEY_G                  'g'
#define  	KEY_H                  'h'
#define  	KEY_I                  'i'
#define  	KEY_J                  'j'
#define  	KEY_K                  'k'
#define  	KEY_L                  'l'
#define  	KEY_M                  'm'
#define  	KEY_N                  'n'
#define  	KEY_O                  'o'
#define  	KEY_P                  'p'
#define  	KEY_Q                  'q'
#define  	KEY_R                  'r'
#define  	KEY_S                  's'
#define  	KEY_T                  't'
#define  	KEY_U                  'u'
#define  	KEY_V                  'v'
#define  	KEY_W                  'w'
#define  	KEY_X                  'x'
#define  	KEY_Y                  'y'
#define  	KEY_Z                  'z'
  
#define  	KEY_RETURN             '\r'
#define  	KEY_ESCAPE             0x1001
#define  	KEY_BACKSPACE          '\b'
#define  
  // Arrow keys ///////////////////////
  
#define  	KEY_UP                 0x1100
#define  	KEY_DOWN               0x1101
#define  	KEY_LEFT               0x1102
#define  	KEY_RIGHT              0x1103
#define  
  // Function keys ////////////////////
  
#define  	KEY_F1                 0x1201
#define  	KEY_F2                 0x1202
#define  	KEY_F3                 0x1203
#define  	KEY_F4                 0x1204
#define  	KEY_F5                 0x1205
#define  	KEY_F6                 0x1206
#define  	KEY_F7                 0x1207
#define  	KEY_F8                 0x1208
#define  	KEY_F9                 0x1209
#define  	KEY_F10                0x120a
#define  	KEY_F11                0x120b
#define  	KEY_F12                0x120b
#define  	KEY_F13                0x120c
#define  	KEY_F14                0x120d
#define  	KEY_F15                0x120e
  
#define  	KEY_DOT                '.'
#define  	KEY_COMMA              ','
#define  	KEY_COLON              ':'
#define  	KEY_SEMICOLON          ';'
#define  	KEY_SLASH              '/'
#define  	KEY_BACKSLASH          '\\'
#define  	KEY_PLUS               '+'
#define  	KEY_MINUS              '-'
#define  	KEY_ASTERISK           '*'
#define  	KEY_EXCLAMATION        '!'
#define  	KEY_QUESTION           '?'
#define  	KEY_QUOTEDOUBLE        '\'
#define  	KEY_QUOTE              '\'
#define  	KEY_EQUAL              '='
#define  	KEY_HASH               '#'
#define  	KEY_PERCENT            '%'
#define  	KEY_AMPERSAND          '&'
#define  	KEY_UNDERSCORE         '_'
#define  	KEY_LEFTPARENTHESIS    '('
#define  	KEY_RIGHTPARENTHESIS   ')'
#define  	KEY_LEFTBRACKET        '['
#define  	KEY_RIGHTBRACKET       ']'
#define  	KEY_LEFTCURL           '{'
#define  	KEY_RIGHTCURL          '}'
#define  	KEY_DOLLAR             '$'
#define  	KEY_POUND              '£'
#define  	KEY_EURO               '$'
#define  	KEY_LESS               '<'
#define  	KEY_GREATER            '>'
#define  	KEY_BAR                '|'
#define  	KEY_GRAVE              '`'
#define  	KEY_TILDE              '~'
#define  	KEY_AT                 '@'
#define  	KEY_CARRET             '^'
  
  // Numeric keypad /////////////////////
  
#define  	KEY_KP_0               '0'
#define  	KEY_KP_1               '1'
#define  	KEY_KP_2               '2'
#define  	KEY_KP_3               '3'
#define  	KEY_KP_4               '4'
#define  	KEY_KP_5               '5'
#define  	KEY_KP_6               '6'
#define  	KEY_KP_7               '7'
#define  	KEY_KP_8               '8'
#define  	KEY_KP_9               '9'
#define  	KEY_KP_PLUS            '+'
#define  	KEY_KP_MINUS           '-'
#define  	KEY_KP_DECIMAL         '.'
#define  	KEY_KP_DIVIDE          '/'
#define  	KEY_KP_ASTERISK        '*'
#define  	KEY_KP_NUMLOCK         0x300f
#define  	KEY_KP_ENTER           0x3010
  
#define  	KEY_TAB                0x4000
#define  	KEY_CAPSLOCK           0x4001
  
  // Modify keys /////////////////////
  
#define  	KEY_LSHIFT             0x4002
#define  	KEY_LCTRL              0x4003
#define  	KEY_LALT               0x4004
#define  	KEY_LWIN               0x4005
#define  	KEY_RSHIFT             0x4006
#define  	KEY_RCTRL              0x4007
#define  	KEY_RALT               0x4008
#define  	KEY_RWIN               0x4009
  
#define  	KEY_INSERT             0x400a
#define  	KEY_DELETE             0x400b
#define  	KEY_HOME               0x400c
#define  	KEY_END                0x400d
#define  	KEY_PAGEUP             0x400e
#define  	KEY_PAGEDOWN           0x400f
#define  	KEY_SCROLLLOCK         0x4010
#define  	KEY_PAUSE              0x4011


//! return status of a key
//! 0 : false ; otherwise: true

int kkybrd_get_scroll_lock();
int kkybrd_get_numlock();
int kkybrd_get_capslock();
int kkybrd_get_alt();
int kkybrd_get_ctrl();
int kkybrd_get_shift();

//! resend last command
int kkybrd_ignore_resend();
int kkybrd_check_resend();

//! returns status of tests. kkybrd_self_test runs the test
int kkybrd_get_diagnostic_res();
int kkybrd_get_bat_res();
int kkybrd_self_test();

//! returns last scan code, last keystroke
uint8_t kkybrd_get_last_scan();
uint32_t kkybrd_get_last_key();
void kkybrd_discard_last_key();

//! updateds LEDs
void kkybrd_set_leds(int num, int caps, int scroll);

//! converts keycode to ascii character (takes account of caps lock and shift keys)
char kkybrd_key_to_ascii(uint32_t);

//! keyboard enable/disable
void kkybrd_disable();
void kkybrd_enable();
int kkybrd_is_disabled();

//! reset system
void kkybrd_reset_system();

//! install keyboard
void kkybrd_install(int);
void i86_kybrd_irq();

#endif
