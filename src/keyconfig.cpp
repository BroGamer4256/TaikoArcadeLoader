#include "keyconfig.h"
#include "helpers.h"
#include "poll.h"

Keybindings EXIT          = {.keycodes = {VK_ESCAPE}};
Keybindings TEST          = {.keycodes = {VK_F1}};
Keybindings SERVICE       = {.keycodes = {VK_F2}};
Keybindings DEBUG_UP      = {.keycodes = {VK_UP}};
Keybindings DEBUG_DOWN    = {.keycodes = {VK_DOWN}};
Keybindings DEBUG_ENTER   = {.keycodes = {VK_RETURN}};
Keybindings COIN_ADD      = {.keycodes = {VK_RETURN}, .buttons = {SDL_CONTROLLER_BUTTON_START}};
Keybindings CARD_INSERT_1 = {};
Keybindings CARD_INSERT_2 = {};
Keybindings QR_CARD_READ  = {};
Keybindings QR_DATA_READ  = {.keycodes = {'Q'}};
Keybindings P1_LEFT_BLUE  = {.keycodes = {'D'}, .axis = {SDL_AXIS_LTRIGGER_DOWN}};
Keybindings P1_LEFT_RED   = {.keycodes = {'F'}, .buttons = {SDL_CONTROLLER_BUTTON_LEFTSTICK}};
Keybindings P1_RIGHT_RED  = {.keycodes = {'J'}, .buttons = {SDL_CONTROLLER_BUTTON_RIGHTSTICK}};
Keybindings P1_RIGHT_BLUE = {.keycodes = {'K'}, .axis = {SDL_AXIS_RTRIGGER_DOWN}};
Keybindings P2_LEFT_BLUE  = {};
Keybindings P2_LEFT_RED   = {};
Keybindings P2_RIGHT_RED  = {};
Keybindings P2_RIGHT_BLUE = {};

namespace KeyConfig {

void
Init() {
	auto configPath      = std::filesystem::current_path () / "keyconfig.toml";
	toml_table_t *config = openConfig (configPath);
	if (config) {
		SetConfigValue (config, "EXIT", &EXIT);

		SetConfigValue (config, "TEST", &TEST);
		SetConfigValue (config, "SERVICE", &SERVICE);
		SetConfigValue (config, "DEBUG_UP", &DEBUG_UP);
		SetConfigValue (config, "DEBUG_DOWN", &DEBUG_DOWN);
		SetConfigValue (config, "DEBUG_ENTER", &DEBUG_ENTER);

		SetConfigValue (config, "COIN_ADD", &COIN_ADD);
		SetConfigValue (config, "CARD_INSERT_1", &CARD_INSERT_1);
		SetConfigValue (config, "CARD_INSERT_2", &CARD_INSERT_2);
		SetConfigValue (config, "QR_CARD_READ", &QR_CARD_READ);
		SetConfigValue (config, "QR_DATA_READ", &QR_DATA_READ);

		SetConfigValue (config, "P1_LEFT_BLUE", &P1_LEFT_BLUE);
		SetConfigValue (config, "P1_LEFT_RED", &P1_LEFT_RED);
		SetConfigValue (config, "P1_RIGHT_RED", &P1_RIGHT_RED);
		SetConfigValue (config, "P1_RIGHT_BLUE", &P1_RIGHT_BLUE);
		SetConfigValue (config, "P2_LEFT_BLUE", &P2_LEFT_BLUE);
		SetConfigValue (config, "P2_LEFT_RED", &P2_LEFT_RED);
		SetConfigValue (config, "P2_RIGHT_RED", &P2_RIGHT_RED);
		SetConfigValue (config, "P2_RIGHT_BLUE", &P2_RIGHT_BLUE);

		toml_free (config);
	}
}
}