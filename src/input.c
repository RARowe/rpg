// Helpers
static inline bool input_is(int src, int mask) {
    return src & mask;
}
//
//inline bool
//input_mouse_is(const Input* i, int mask) {
//    return i->mouseState & mask;
//}
//
//inline bool
//input_is_down(const Input* i, GameInput input) {
//    return i->game[input] & INPUT_STATE_DOWN;
//}
//
//inline bool
//input_game_input_is_pressed(const Input* i, GameInput input) {
//    return i->game[input] & INPUT_STATE_PRESSED;
//}
//
//inline bool
//input_key_is_pressed(const Input* i, Key key) {
//    return i->keys[key] & INPUT_STATE_PRESSED;
//}
//
static void input_handle_key_event(Input* i, SDL_Event* event) {
	SDL_Keycode key = event->key.keysym.sym;
	bool keyDown = event->type == SDL_KEYDOWN;

	switch (key) {
		case SDLK_UP:
			if (keyDown) {
				i->pressed |= GAME_INPUT_UP;
				i->down |= GAME_INPUT_UP;
			} else {
				i->released |= GAME_INPUT_UP;
			}
			break;
		case SDLK_DOWN:
			if (keyDown) {
				i->pressed |= GAME_INPUT_DOWN;
				i->down |= GAME_INPUT_DOWN;
			} else {
				i->released |= GAME_INPUT_DOWN;
			}
			break;
		case SDLK_LEFT:
			if (keyDown) {
				i->pressed |= GAME_INPUT_LEFT;
				i->down |= GAME_INPUT_LEFT;
			} else {
				i->released |= GAME_INPUT_LEFT;
			}
			break;
		case SDLK_RIGHT:
			if (keyDown) {
				i->pressed |= GAME_INPUT_RIGHT;
				i->down |= GAME_INPUT_RIGHT;
			} else {
				i->released |= GAME_INPUT_RIGHT;
			}
			break;
		case SDLK_ESCAPE:
			if (keyDown) {
				i->pressed |= GAME_INPUT_ESC;
				i->down |= GAME_INPUT_ESC;
			} else {
				i->released |= GAME_INPUT_ESC;
			}
			break;
		case SDLK_RGUI:
		case SDLK_LGUI:
			if (keyDown) {
				i->pressed |= GAME_INPUT_ESC;
				i->down |= GAME_INPUT_ESC;
			} else {
				i->released |= GAME_INPUT_ESC;
			}
			break;
		case SDLK_RCTRL:
		case SDLK_LCTRL:
			if (keyDown) {
				i->pressed |= GAME_INPUT_CTRL;
				i->down |= GAME_INPUT_CTRL;
			} else {
				i->released |= GAME_INPUT_CTRL;
			}
			break;
		default:
			if (key >= '\r' && key <= 'z') {
				i->keys[(int)key] = 1;
				i->hasLastPressedKey = true;
				i->lastPressedKey = key;
			} 
			//i->game[GAME_INPUT_SELECT] = i->keys[SDLKf];
			//i->game[GAME_INPUT_BACK] = i->keys[SDLK_d];
			break;
	}
}

static int input_process(Input* i) {
	SDL_Event event;

	/* Reset input */
	i->mouseMoving = false;
	i->hasLastPressedKey = false;

	i->pressed = 0;
	i->released = 0;

	if (input_is(i->mouseState, INPUT_STATE_PRESSED)) {
		i->mouseState = INPUT_STATE_DOWN;
	} else if (input_is(i->mouseState, INPUT_STATE_RELEASED)) {
		i->mouseState = INPUT_STATE_UP;
	}

	for (int j = 0; j < 128; j++) {
		if (input_is(i->keys[j], INPUT_STATE_PRESSED)) {
			i->keys[j] = INPUT_STATE_DOWN;
		} else if (input_is(i->keys[j], INPUT_STATE_RELEASED)) {
			i->keys[j] = INPUT_STATE_UP;
		}
	}

	/* Poll new input */
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				return -1;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				input_handle_key_event(i, &event);
				break;
			case SDL_MOUSEBUTTONDOWN:
				i->mouseState = INPUT_STATE_PRESSED | INPUT_STATE_DOWN;
				break;
			case SDL_MOUSEBUTTONUP:
				i->mouseState = INPUT_STATE_RELEASED | INPUT_STATE_UP;
				break;
			case SDL_MOUSEMOTION:
				i->mouseMoving = true;
				i->mouseX = event.motion.x;
				i->mouseY = event.motion.y;
				break;
		}
	}

	return 0;
}
//
//
