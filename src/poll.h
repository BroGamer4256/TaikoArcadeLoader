#pragma once
#include <SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <toml.h>
#include <windows.h>

enum SDLAxis {
	SDL_AXIS_NULL,
	SDL_AXIS_LEFT_LEFT,
	SDL_AXIS_LEFT_RIGHT,
	SDL_AXIS_LEFT_UP,
	SDL_AXIS_LEFT_DOWN,
	SDL_AXIS_RIGHT_LEFT,
	SDL_AXIS_RIGHT_RIGHT,
	SDL_AXIS_RIGHT_UP,
	SDL_AXIS_RIGHT_DOWN,
	SDL_AXIS_LTRIGGER_DOWN,
	SDL_AXIS_RTRIGGER_DOWN,
	SDL_AXIS_MAX
};

struct SDLAxisState {
	unsigned int LeftLeft : 1;
	unsigned int LeftRight : 1;
	unsigned int LeftUp : 1;
	unsigned int LeftDown : 1;
	unsigned int RightLeft : 1;
	unsigned int RightRight : 1;
	unsigned int RightUp : 1;
	unsigned int RightDown : 1;
	unsigned int LTriggerDown : 1;
	unsigned int RTriggerDown : 1;
};

enum Scroll { MOUSE_SCROLL_INVALID, MOUSE_SCROLL_UP, MOUSE_SCROLL_DOWN };

struct Keybindings {
	uint8_t keycodes[255];
	SDL_GameControllerButton buttons[255];
	enum SDLAxis axis[255];
	enum Scroll scroll[2];
};

typedef struct Keybindings Keybindings;

enum EnumType { none, keycode, button, axis, scroll };

struct ConfigValue {
	enum EnumType type;
	union {
		uint8_t keycode;
		SDL_GameControllerButton button;
		enum SDLAxis axis;
		enum Scroll scroll;
	};
};

struct InternalButtonState {
	unsigned int Released : 1;
	unsigned int Down : 1;
	unsigned int Tapped : 1;
};

#ifdef __cplusplus
extern "C" {
#endif
bool InitializePoll (void *DivaWindowHandle);
void UpdatePoll (void *DivaWindowHandle);
void DisposePoll ();

struct ConfigValue StringToConfigEnum (char *value);
void SetConfigValue (toml_table_t *table, char *key, struct Keybindings *keybind);
struct InternalButtonState GetInternalButtonState (struct Keybindings bindings);
void SetRumble (int left, int right);

bool KeyboardIsDown (uint8_t keycode);
bool KeyboardIsUp (uint8_t keycode);
bool KeyboardIsTapped (uint8_t keycode);
bool KeyboardIsReleased (uint8_t keycode);
bool KeyboardWasDown (uint8_t keycode);
bool KeyboardWasUp (uint8_t keycode);
POINT GetMousePosition ();
POINT GetLastMousePosition ();
POINT GetMouseRelativePosition ();
POINT GetLastMouseRelativePosition ();
void SetMousePosition (POINT newPosition);
bool GetMouseScrollUp ();
bool GetMouseScrollDown ();
bool GetWasMouseScrollUp ();
bool GetWasMouseScrollDown ();
bool GetMouseScrollIsReleased (enum Scroll scroll);
bool GetMouseScrollIsDown (enum Scroll scroll);
bool GetMouseScrollIsTapped (enum Scroll scroll);
bool ControllerButtonIsDown (SDL_GameControllerButton button);
bool ControllerButtonIsUp (SDL_GameControllerButton button);
bool ControllerButtonWasDown (SDL_GameControllerButton button);
bool ControllerButtonWasUp (SDL_GameControllerButton button);
bool ControllerButtonIsTapped (SDL_GameControllerButton button);
bool ControllerButtonIsReleased (SDL_GameControllerButton button);
bool ControllerAxisIsDown (enum SDLAxis axis);
bool ControllerAxisIsUp (enum SDLAxis axis);
bool ControllerAxisWasDown (enum SDLAxis axis);
bool ControllerAxisWasUp (enum SDLAxis axis);
bool ControllerAxisIsTapped (enum SDLAxis axis);
bool ControllerAxisIsReleased (enum SDLAxis axis);
bool IsButtonTapped (struct Keybindings bindings);
bool IsButtonReleased (struct Keybindings bindings);
bool IsButtonDown (struct Keybindings bindings);
#ifdef __cplusplus
}
#endif
