#pragma once

#include "utils/utils.hpp"
#include "vec.hpp"

namespace clgl
{
enum class EventType
{
    Console                = WINDOW_BUFFER_SIZE_EVENT,
    Key                    = KEY_EVENT,
    Mouse                  = MOUSE_EVENT,
};

enum class KeyCode
{
    LeftMouseButton        = VK_LBUTTON,
    RightMouseButton       = VK_RBUTTON,
    ControlBreakProcessing = VK_CANCEL,
    MiddleMouseButton      = VK_MBUTTON,
    ForwardMouseButton     = VK_XBUTTON1,
    BackwardMouseButton    = VK_XBUTTON2,
    BackSpace              = VK_BACK,
    Tab                    = VK_TAB,
    Clear                  = VK_CLEAR,
    Enter                  = VK_RETURN,
    Shift                  = VK_SHIFT,
    Ctrl                   = VK_CONTROL,
    Alt                    = VK_MENU,
    Pause                  = VK_PAUSE,
    CapsLock               = VK_CAPITAL,
    ImeKanaMode            = VK_KANA,
    ImeHangulMode          = VK_HANGUL,
    ImeOn                  = VK_IME_ON,
    ImeJunjaMode           = VK_JUNJA,
    ImeFinal               = VK_FINAL,
    ImeHanja               = VK_HANJA,
    ImeKanji               = VK_KANJI,
    ImeOff                 = VK_IME_OFF,
    Escape                 = VK_ESCAPE,
    ImeConvert             = VK_CONVERT,
    ImeNonConvert          = VK_NONCONVERT,
    ImeAccept              = VK_ACCEPT,
    ImeModeChange          = VK_MODECHANGE,
    Spacebar               = VK_SPACE,
    PageUp                 = VK_PRIOR,
    PageDown               = VK_NEXT,
    End                    = VK_END,
    Home                   = VK_HOME,
    Left                   = VK_LEFT,
    Up                     = VK_UP,
    Right                  = VK_RIGHT,
    Down                   = VK_DOWN,
    Select                 = VK_SELECT,
    Print                  = VK_PRINT,
    Execute                = VK_EXECUTE,
    PrintScreen            = VK_SNAPSHOT,
    Insert                 = VK_INSERT,
    Delete                 = VK_DELETE,
    Help                   = VK_HELP,
    Zero                   = 0x30,
    One                    = 0x31,
    Two                    = 0x32,
    Three                  = 0x33,
    Four                   = 0x34,
    Five                   = 0x35,
    Six                    = 0x36,
    Seven                  = 0x37,
    Eight                  = 0x38,
    Nine                   = 0x39,
    A                      = 0x41,
    B                      = 0x42,
    C                      = 0x43,
    D                      = 0x44,
    E                      = 0x45,
    F                      = 0x46,
    G                      = 0x47,
    H                      = 0x48,
    I                      = 0x49,
    J                      = 0x4A,
    K                      = 0x4B,
    L                      = 0x4C,
    M                      = 0x4D,
    N                      = 0x4E,
    O                      = 0x4F,
    P                      = 0x50,
    Q                      = 0x51,
    R                      = 0x52,
    S                      = 0x53,
    T                      = 0x54,
    U                      = 0x55,
    V                      = 0x56,
    W                      = 0x57,
    X                      = 0x58,
    Y                      = 0x59,
    Z                      = 0x5A,
    LeftWindowsKey         = VK_LWIN,
    RightWindowsKey        = VK_RWIN,
    Applications           = VK_APPS,
    Sleep                  = VK_SLEEP,
    Numpad0                = VK_NUMPAD0,
    Numpad1                = VK_NUMPAD1,
    Numpad2                = VK_NUMPAD2,
    Numpad3                = VK_NUMPAD3,
    Numpad4                = VK_NUMPAD4,
    Numpad5                = VK_NUMPAD5,
    Numpad6                = VK_NUMPAD6,
    Numpad7                = VK_NUMPAD7,
    Numpad8                = VK_NUMPAD8,
    Numpad9                = VK_NUMPAD9,
    Multiply               = VK_MULTIPLY,
    Add                    = VK_ADD,
    Separator              = VK_SEPARATOR,
    Subtract               = VK_SUBTRACT,
    Decimal                = VK_DECIMAL,
    Divide                 = VK_DIVIDE,
    F1                     = VK_F1,
    F2                     = VK_F2,
    F3                     = VK_F3,
    F4                     = VK_F4,
    F5                     = VK_F5,
    F6                     = VK_F6,
    F7                     = VK_F7,
    F8                     = VK_F8,
    F9                     = VK_F9,
    F10                    = VK_F10,
    F11                    = VK_F11,
    F12                    = VK_F12,
    F13                    = VK_F13,
    F14                    = VK_F14,
    F15                    = VK_F15,
    F16                    = VK_F16,
    F17                    = VK_F17,
    F18                    = VK_F18,
    F19                    = VK_F19,
    F20                    = VK_F20,
    F21                    = VK_F21,
    F22                    = VK_F22,
    F23                    = VK_F23,
    F24                    = VK_F24,
    NumLock                = VK_NUMLOCK,
    ScrollLock             = VK_SCROLL,
    LeftShift              = VK_LSHIFT,
    RightShift             = VK_RSHIFT,
    LeftControl            = VK_LCONTROL,
    RightControl           = VK_RCONTROL,
    LeftAlt                = VK_LMENU,
    RightAlt               = VK_RMENU,
    BrowserBack            = VK_BROWSER_BACK,
    BrowserForward         = VK_BROWSER_FORWARD,
    BrowserRefresh         = VK_BROWSER_REFRESH,
    BrowserStop            = VK_BROWSER_STOP,
    BrowserSearch          = VK_BROWSER_SEARCH,
    BrowserFavorites       = VK_BROWSER_FAVORITES,
    BrowserHome            = VK_BROWSER_HOME,
    VolumeMute             = VK_VOLUME_MUTE,
    VolumeDown             = VK_VOLUME_DOWN,
    VolumeUp               = VK_VOLUME_UP,
    NextTrack              = VK_MEDIA_NEXT_TRACK,
    PreviousTrack          = VK_MEDIA_PREV_TRACK,
    Stop                   = VK_MEDIA_STOP,
    PlayPause              = VK_MEDIA_PLAY_PAUSE,
    StartMail              = VK_LAUNCH_MAIL,
    SelectMedia            = VK_LAUNCH_MEDIA_SELECT,
    StartApplication1      = VK_LAUNCH_APP1,
    StartApplication2      = VK_LAUNCH_APP2,
    Semicolon              = VK_OEM_1,
    Plus                   = VK_OEM_PLUS,
    Comma                  = VK_OEM_COMMA,
    Minus                  = VK_OEM_MINUS,
    Period                 = VK_OEM_PERIOD,
    ForwardSlash           = VK_OEM_2,
    Backquote              = VK_OEM_3,
    SquareBracketOpen      = VK_OEM_4,
    BackwardSlash          = VK_OEM_5,
    SquareBracketClose     = VK_OEM_6,
    Apostrophe             = VK_OEM_7,
    ImeProcess             = VK_PROCESSKEY,
    Packet                 = VK_PACKET,
    Attn                   = VK_ATTN,
    CrSel                  = VK_CRSEL,
    ExSel                  = VK_EXSEL,
    ErEOF                  = VK_EREOF,
    Play                   = VK_PLAY,
    Zoom                   = VK_ZOOM,
    PA1                    = VK_PA1,
    OEMClear               = VK_OEM_CLEAR,
    _KeyCodeCount
};

enum class ControlKey
{
    Capslock               = CAPSLOCK_ON,
    EnhancedKey            = ENHANCED_KEY,
    LeftAlt                = LEFT_ALT_PRESSED,
    LeftCtrl               = LEFT_CTRL_PRESSED,
    Numlock                = NUMLOCK_ON,
    RightAlt               = RIGHT_ALT_PRESSED,
    RightCtrl              = RIGHT_CTRL_PRESSED,
    ScrollLock             = SCROLLLOCK_ON,
    Shift                  = SHIFT_PRESSED,
};

enum class MouseButton
{
    Left                   = FROM_LEFT_1ST_BUTTON_PRESSED,
    Right                  = FROM_LEFT_2ND_BUTTON_PRESSED,
    Middle                 = FROM_LEFT_3RD_BUTTON_PRESSED,
};

class Input;

class KeyEvent
{
public:

    KeyCode keycode;

private:

    bool m_down;
    U16  m_control_key_state;

public:

    bool pressed()  const;
    bool released() const;

    bool check_control_key(ControlKey control_key) const;

friend class Input;
};

class MouseEvent
{
public:

    enum Flag
    {
        Moved            = MOUSE_MOVED,
        DoubleClick      = DOUBLE_CLICK,
        VerticalScroll   = MOUSE_WHEELED,
    };

public:

    Vec2I position;

private:

    U8    m_mouse_state;
    U16   m_control_key_state;
    U8    m_flags;

public:

    bool  pressed(MouseButton button) const;
    bool  check_control_key(ControlKey control_key) const;
    bool  check_flag(Flag flag) const;

friend class Input;
};

class ConsoleEvent
{
public:

    Vec2U new_size;

friend class Input;
};

struct Event
{
    EventType type;

    union {
        KeyEvent keyEvent;
        MouseEvent mouseEvent;
        ConsoleEvent consoleEvent;
    } event;
};
}