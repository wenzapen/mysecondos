#include "kybrd.h"
#include <hal.h>

//! keyboard encoder
#define KYBRD_ENC_INPUT_BUF  0x60
#define KYBRD_ENC_CMD_REG    0x60

#define 	KYBRD_ENC_CMD_SET_LED				0xED
#define 	KYBRD_ENC_CMD_ECHO				0xEE
#define 	KYBRD_ENC_CMD_SCAN_CODE_SET			0xF0
#define 	KYBRD_ENC_CMD_ID				0xF2
#define 	KYBRD_ENC_CMD_AUTODELAY				0xF3
#define 	KYBRD_ENC_CMD_ENABLE				0xF4
#define 	KYBRD_ENC_CMD_RESETWAIT				0xF5
#define 	KYBRD_ENC_CMD_RESETSCAN				0xF6
#define 	KYBRD_ENC_CMD_ALL_AUTO				0xF7
#define 	KYBRD_ENC_CMD_ALL_MAKEBREAK			0xF8
#define 	KYBRD_ENC_CMD_ALL_MAKEONLY			0xF9
#define 	KYBRD_ENC_CMD_ALL_MAKEBREAK_AUTO		0xFA
#define 	KYBRD_ENC_CMD_SINGLE_AUTOREPEAT			0xFB
#define 	KYBRD_ENC_CMD_SINGLE_MAKEBREAK			0xFC
#define 	KYBRD_ENC_CMD_SINGLE_BREAKONLY			0xFD
#define 	KYBRD_ENC_CMD_RESEND				0xFE
#define 	KYBRD_ENC_CMD_RESET				0xFF

//! keyboard controller
#define KYBRD_CTRL_STATS_REG  0x64
#define KYBRD_CTRL_CMD_REG    0x64

#define 	KYBRD_CTRL_STATS_MASK_OUT_BUF		1		//00000001
#define 	KYBRD_CTRL_STATS_MASK_IN_BUF		2		//00000010
#define 	KYBRD_CTRL_STATS_MASK_SYSTEM		4		//00000100
#define 	KYBRD_CTRL_STATS_MASK_CMD_DATA		8		//00001000
#define 	KYBRD_CTRL_STATS_MASK_LOCKED		0x10	//00010000
#define 	KYBRD_CTRL_STATS_MASK_AUX_BUF		0x20	//00100000
#define 	KYBRD_CTRL_STATS_MASK_TIMEOUT		0x40	//01000000
#define 	KYBRD_CTRL_STATS_MASK_PARITY		0x80	//10000000

#define 	KYBRD_CTRL_CMD_READ			0x20
#define 	KYBRD_CTRL_CMD_WRITE			0x60
#define 	KYBRD_CTRL_CMD_SELF_TEST		0xAA
#define 	KYBRD_CTRL_CMD_INTERFACE_TEST		0xAB
#define 	KYBRD_CTRL_CMD_DISABLE			0xAD
#define 	KYBRD_CTRL_CMD_ENABLE			0xAE
#define 	KYBRD_CTRL_CMD_READ_IN_PORT		0xC0
#define 	KYBRD_CTRL_CMD_READ_OUT_PORT		0xD0
#define 	KYBRD_CTRL_CMD_WRITE_OUT_PORT		0xD1
#define 	KYBRD_CTRL_CMD_READ_TEST_INPUTS		0xE0
#define 	KYBRD_CTRL_CMD_SYSTEM_RESET		0xFE
#define 	KYBRD_CTRL_CMD_MOUSE_DISABLE		0xA7
#define 	KYBRD_CTRL_CMD_MOUSE_ENABLE		0xA8
#define 	KYBRD_CTRL_CMD_MOUSE_PORT_TEST		0xA9
#define 	KYBRD_CTRL_CMD_MOUSE_WRITE		0xD4

//! scan error code
#define 	KYBRD_ERR_BUF_OVERRUN			0
#define 	KYBRD_ERR_ID_RET			0x83AB
#define 	KYBRD_ERR_BAT				0xAA	// L. shift key make code
#define 	KYBRD_ERR_ECHO_RET			0xEE
#define 	KYBRD_ERR_ACK				0xFA
#define 	KYBRD_ERR_BAT_FAILED			0xFC
#define 	KYBRD_ERR_DIAG_FAILED			0xFD
#define 	KYBRD_ERR_RESEND_CMD			0xFE
#define 	KYBRD_ERR_KEY				0xFF


//! current scan code
static char _scancode;

//! lock keys
static int _numlock, _scrolllock, _capslock;

//! shift, alt and ctrl keys current state
static int _shift, _alt, _ctrl;

//! set if keyboard erro
static int _kkybrd_error = 0;

//! set if the basic assurance test(BAT) failed
static int _kkybrd_bat_res = 0;

//! set if diagnostics failed
static int _kkybrd_diag_res = 0;

//! set if system should resend last command
static int _kkybrd_resend_res = 0;

//! set if keyboard is disabled
static int _kkybrd_disable = 0;

//! original xt scan code set. Array index=make code
//! change what keys the scan code correspond to if your scan code set is different
static int _kkybrd_scancode_std [] = {

	//! key			scancode
	KEY_UNKNOWN,	//0
	KEY_ESCAPE,		//1
	KEY_1,			//2
	KEY_2,			//3
	KEY_3,			//4
	KEY_4,			//5
	KEY_5,			//6
	KEY_6,			//7
	KEY_7,			//8
	KEY_8,			//9
	KEY_9,			//0xa
	KEY_0,			//0xb
	KEY_MINUS,		//0xc
	KEY_EQUAL,		//0xd
	KEY_BACKSPACE,	//0xe
	KEY_TAB,		//0xf
	KEY_Q,			//0x10
	KEY_W,			//0x11
	KEY_E,			//0x12
	KEY_R,			//0x13
	KEY_T,			//0x14
	KEY_Y,			//0x15
	KEY_U,			//0x16
	KEY_I,			//0x17
	KEY_O,			//0x18
	KEY_P,			//0x19
	KEY_LEFTBRACKET,//0x1a
	KEY_RIGHTBRACKET,//0x1b
	KEY_RETURN,		//0x1c
	KEY_LCTRL,		//0x1d
	KEY_A,			//0x1e
	KEY_S,			//0x1f
	KEY_D,			//0x20
	KEY_F,			//0x21
	KEY_G,			//0x22
	KEY_H,			//0x23
	KEY_J,			//0x24
	KEY_K,			//0x25
	KEY_L,			//0x26
	KEY_SEMICOLON,	//0x27
	KEY_QUOTE,		//0x28
	KEY_GRAVE,		//0x29
	KEY_LSHIFT,		//0x2a
	KEY_BACKSLASH,	//0x2b
	KEY_Z,			//0x2c
	KEY_X,			//0x2d
	KEY_C,			//0x2e
	KEY_V,			//0x2f
	KEY_B,			//0x30
	KEY_N,			//0x31
	KEY_M,			//0x32
	KEY_COMMA,		//0x33
	KEY_DOT,		//0x34
	KEY_SLASH,		//0x35
	KEY_RSHIFT,		//0x36
	KEY_KP_ASTERISK,//0x37
	KEY_RALT,		//0x38
	KEY_SPACE,		//0x39
	KEY_CAPSLOCK,	//0x3a
	KEY_F1,			//0x3b
	KEY_F2,			//0x3c
	KEY_F3,			//0x3d
	KEY_F4,			//0x3e
	KEY_F5,			//0x3f
	KEY_F6,			//0x40
	KEY_F7,			//0x41
	KEY_F8,			//0x42
	KEY_F9,			//0x43
	KEY_F10,		//0x44
	KEY_KP_NUMLOCK,	//0x45
	KEY_SCROLLLOCK,	//0x46
	KEY_HOME,		//0x47
	KEY_KP_8,		//0x48	//keypad up arrow
	KEY_PAGEUP,		//0x49
	KEY_KP_2,		//0x50	//keypad down arrow
	KEY_KP_3,		//0x51	//keypad page down
	KEY_KP_0,		//0x52	//keypad insert key
	KEY_KP_DECIMAL,	//0x53	//keypad delete key
	KEY_UNKNOWN,	//0x54
	KEY_UNKNOWN,	//0x55
	KEY_UNKNOWN,	//0x56
	KEY_F11,		//0x57
	KEY_F12			//0x58
};

//! invalid scan code. Used to indicate the last scan code is not to be reused
const int INVALID_SCANCODE = 0;

uint8_t kybrd_ctrl_read_status();
void    kybrd_ctrl_send_cmd(uint8_t);
uint8_t kybrd_enc_read_buf();
void    kybrd_enc_send_cmd(uint8_t);


uint8_t kybrd_ctrl_read_status() {
    return inportb(KYBRD_CTRL_STATS_REG);
}

//! keyboard controller has 2 buffer, 1 for input and 1 for output
void    kybrd_ctrl_send_cmd(uint8_t cmd) {
    //! loop until in buffer is empty(1 is full)	
    while(kybrd_ctrl_read_status() & KYBRD_CTRL_STATS_MASK_IN_BUF) ;
    outportb(KYBRD_CTRL_CMD_REG, cmd);
}

//! read keyboard encode buffer
uint8_t kybrd_enc_read_buf() {
    //! loop until out buffer is full(0 is empty)
//    while(!(kybrd_ctrl_read_status() & KYBRD_CTRL_STATS_MASK_OUT_BUF)) ;
    return inportb(KYBRD_ENC_INPUT_BUF);
}

//! keyboard controller has 2 buffer, 1 for input and 1 for output
//! send cmd to keyboard encode via port 60
void    kybrd_enc_send_cmd(uint8_t cmd) {
    //! loop until in buffer is empty(1 is full)	
    while(kybrd_ctrl_read_status() & KYBRD_CTRL_STATS_MASK_IN_BUF) ;
    outportb(KYBRD_ENC_CMD_REG, cmd);
}

//! keyboard interrupt handler

void i86_kybrd_irq() {
    static int _extended = 0;
    int code = 0;
    //! read scan code only if the keyboard controller output buffer is full
    if( kybrd_ctrl_read_status() & KYBRD_CTRL_STATS_MASK_OUT_BUF) {
	code = kybrd_enc_read_buf();
	//! set it and return if this is an extended code
	if( code==0xE0 || code==0xE1) {
	    _extended = 1;
	} else {
	    //! test if this is a break code(original XT scan code specific
	    _extended = 0;
	    if(code & 0x80) {
		code -= 0x80;
		int key = _kkybrd_scancode_std[code];
		//! test if a special key has been released and set it
		switch(key) {
		    case KEY_LCTRL:
		    case KEY_RCTRL:
			_ctrl = 0;
			break;
		    case KEY_LSHIFT:
		    case KEY_RSHIFT:
			_shift = 0;
			break;
		    case KEY_LALT:
		    case KEY_RALT:
			_alt = 0;
			break;
		}
	    }  else {    // this is a make code
		_scancode = code;
		int key = _kkybrd_scancode_std[code];
		//! test if use is holding down a special key and set it
		switch(key) {
		    case KEY_LCTRL:
		    case KEY_RCTRL:
			_ctrl = 1;
			break;
		    case KEY_LSHIFT:
		    case KEY_RSHIFT:
			_shift = 1;
			break;
		    case KEY_LALT:
		    case KEY_RALT:
			_alt = 1;
			break;
		    case KEY_CAPSLOCK:
			_capslock = _capslock ? 0 : 1;
			kkybrd_set_leds(_numlock, _capslock, _scrolllock);
			break;
		    case KEY_KP_NUMLOCK:
			_numlock = _numlock ? 0 : 1;
			kkybrd_set_leds(_numlock, _capslock, _scrolllock);
			break;
		    case KEY_SCROLLLOCK:
			_scrolllock = _scrolllock ? 0 : 1;
			kkybrd_set_leds(_numlock, _capslock, _scrolllock);
			break;
		}
	    }

	}
	//! watch for error
	switch(code) {
	    case KYBRD_ERR_BAT_FAILED:
		_kkybrd_bat_res = 0;
		break;
	    case KYBRD_ERR_DIAG_FAILED:
		_kkybrd_diag_res = 0;
		break;
	    case KYBRD_ERR_RESEND_CMD:
		_kkybrd_resend_res = true;
		break;
	}
    }
}

int kkybrd_get_scroll_lock() {
    return _scrolllock;
}

int kkybrd_get_numlock() {
    return _numlock;
}

int kkybrd_get_capslock() {
    return _capslock;
}

int kkybrd_get_ctrl() {
    return _ctrl;
}

int kkybrd_get_shift() {
    return _shift;
}

int kkybrd_get_alt() {
    return _alt;
}

//! tells driver to ignore last resend request
void kkybrd_ignore_resend() {
    _kkybrd_resend_res = 0;
}

//! return if system should redo last commands
int kkybrd_check_resend() {
    return _kkybrd_resend_res;
}

//! return diagnostics test result
int kkybrd_get_diagnostic_res() {
    return _kkybrd_diag_res;
}

//! return BAT test result
int kkybrd_get_bat_res() {
    return _kkybrd_bat_res;
}

//! return last scan code
uint8_t kkybrd_get_last_scan() {
    return _scancode;
}

void kkybrd_set_leds(int num, int caps, int scroll) {
    uint8_t data = 0;
    data = scroll ? (data | 0x1) : (data & 0x1);
    data = num ? (data | 0x2) : (data & 0x2);
    data = caps ? (data | 0x4) : (data & 0x4);
    kybrd_enc_send_cmd(KYBRD_ENC_CMD_SET_LED);
    kybrd_enc_send_cmd(data);
}

//! get last key stroke
int kkybrd_get_last_key() {
    return (_scancode != INVALID_SCANCODE) ? (_kkybrd_scancode_std[_scancode]) : (KEY_UNKOWN);
}

void kkybrd_discard_last_key() {
    _scancode =INVALID_SCANCODE;
}

char kkybrd_key_to_ascii(int code) {
    if(!isascii(code)) return 0;
    uint8_t key = (uint8_t)code;
    if(_shift ^ _capslock) 
	if(key>='a' && key<='z')
	    key -= 32;
    if(_shift) {
	if(key>='0' && key<='9') {
	    switch (key) {
	    
	    	case '0':
	    		key = KEY_RIGHTPARENTHESIS;
	    		break;
	    	case '1':
	    		key = KEY_EXCLAMATION;
	    		break;
	    	case '2':
	    		key = KEY_AT;
	    		break;
	    	case '3':
	    		key = KEY_EXCLAMATION;
	    		break;
	    	case '4':
	    		key = KEY_HASH;
	    		break;
	    	case '5':
	    		key = KEY_PERCENT;
	    		break;
	    	case '6':
	    		key = KEY_CARRET;
	    		break;
	    	case '7':
	    		key = KEY_AMPERSAND;
	    		break;
	    	case '8':
	    		key = KEY_ASTERISK;
	    		break;
	    	case '9':
	    		key = KEY_LEFTPARENTHESIS;
	    		break;
	    }

	} else {
	    switch (key) {
	    	case KEY_COMMA:
	    		key = KEY_LESS;
	    		break;
	    
	    	case KEY_DOT:
	    		key = KEY_GREATER;
	    		break;
	    
	    	case KEY_SLASH:
	    		key = KEY_QUESTION;
	    		break;
	    
	    	case KEY_SEMICOLON:
	    		key = KEY_COLON;
	    		break;
	    
	    	case KEY_QUOTE:
	    		key = KEY_QUOTEDOUBLE;
	    		break;
	    
	    	case KEY_LEFTBRACKET :
	    		key = KEY_LEFTCURL;
	    		break;
	    
	    	case KEY_RIGHTBRACKET :
	    		key = KEY_RIGHTCURL;
	    		break;
	    
	    	case KEY_GRAVE:
	    		key = KEY_TILDE;
	    		break;
	    
	    	case KEY_MINUS:
	    		key = KEY_UNDERSCORE;
	    		break;
	    
	    	case KEY_PLUS:
	    		key = KEY_EQUAL;
	    		break;
	    
	    	case KEY_BACKSLASH:
	    		key = KEY_BAR;
	    		break;
	    }

	}


    }
    return key;
}

void kkybrd_disable() {
    kybrd_ctrl_send_cmd(KYBRD_CTRL_CMD_DISABLE);
    _kkybrd_disable = 1;
} 

void kkybrd_enable() {
    kybrd_ctrl_send_cmd(KYBRD_CTRL_CMD_ENABLE);
    _kkybrd_disable = 0;
}

int kkybrd_is_disabled() {
    return _kkybrd_disable;
}

void kkybrd_reset_system() {
    kybrd_ctrl_send_cmd(KYBRD_CTRL_CMD_WRITE_OUT_PORT);
    kybrd_enc_send_cmd(0xfe);
}

int kkybrd_self_test() {
    kybrd_ctrl_send_cmd(KYBRD_CTRL_CMD_SELF_TEST);
    while(!(kybrd_ctrl_read_status() & KYBRD_CTRL_STATS_MASK_OUT_BUF))
	;
    return (kybrd_enc_read_buf() == 0x55) ? 1 : 0;

}

void kkybrd_install() {
    _kkybrd_bat_res = 1;
    _scancode = 0;
    _numlock = _scrolllock ==capslock = 0;
    kkybrd_set_leds(0, 0, 0);
    _shift = _alt = _ctrl = 0;

}
