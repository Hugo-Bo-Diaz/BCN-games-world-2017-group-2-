#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Window.h"
#include "SDL/include/SDL.h"

#define MAX_KEYS 300

j1Input::j1Input() : j1Module()
{
	name.create("input");

	keyboard = new j1KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(j1KeyState) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(j1KeyState) * NUM_MOUSE_BUTTONS);
}

// Destructor
j1Input::~j1Input()
{}

// Called before render is available
bool j1Input::Awake(pugi::xml_node* config)
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
	{
		LOG("SDL_GAMEPAD could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		if (SDL_NumJoysticks() > 0)
		{
			Controller = SDL_GameControllerOpen(0);
			if (Controller == nullptr)
			{
				LOG("Controller couldn't be initialized SDL_Error: %s\n", SDL_GetError());
			}
			else
			{
				controller_connected = true;
			}
		}
	};
	return ret;
}

// Called before the first frame
bool j1Input::Start()
{
	SDL_StopTextInput();
	return true;
}

// Called each loop iteration
bool j1Input::PreUpdate()
{
	static SDL_Event event;

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (mouse_buttons[i] == KEY_DOWN)
			mouse_buttons[i] = KEY_REPEAT;

		if (mouse_buttons[i] == KEY_UP)
			mouse_buttons[i] = KEY_IDLE;
	}

	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			windowEvents[WE_QUIT] = true;
			break;

		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
				//case SDL_WINDOWEVENT_LEAVE:
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_FOCUS_LOST:
				windowEvents[WE_HIDE] = true;
				break;

				//case SDL_WINDOWEVENT_ENTER:
			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_RESTORED:
				windowEvents[WE_SHOW] = true;
				break;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			mouse_buttons[event.button.button - 1] = KEY_DOWN;
			//LOG("Mouse button %d down", event.button.button-1);
			break;

		case SDL_MOUSEBUTTONUP:
			mouse_buttons[event.button.button - 1] = KEY_UP;
			//LOG("Mouse button %d up", event.button.button-1);
			break;

		case SDL_MOUSEMOTION:
			int scale = App->win->GetScale();
			mouse_motion_x = event.motion.xrel / scale;
			mouse_motion_y = event.motion.yrel / scale;
			mouse_x = event.motion.x / scale;
			mouse_y = event.motion.y / scale;
			//LOG("Mouse motion x %d y %d", mouse_motion_x, mouse_motion_y);
			break;
		}
	}


	controller_1.left_joystick.x = ((float)SDL_GameControllerGetAxis(Controller, SDL_CONTROLLER_AXIS_LEFTX) / 32767.0f);
	controller_1.left_joystick.y = ((float)SDL_GameControllerGetAxis(Controller, SDL_CONTROLLER_AXIS_LEFTY) / 32767.0f);
	controller_1.right_joystick.x = ((float)SDL_GameControllerGetAxis(Controller, SDL_CONTROLLER_AXIS_RIGHTX) / 32767.0f);
	controller_1.right_joystick.y = ((float)SDL_GameControllerGetAxis(Controller, SDL_CONTROLLER_AXIS_RIGHTY) / 32767.0f);
	controller_1.change_character = ((bool)SDL_GameControllerGetButton(Controller, SDL_CONTROLLER_BUTTON_X));	
	controller_1.jump = ((bool)SDL_GameControllerGetButton(Controller, SDL_CONTROLLER_BUTTON_A));	
	controller_1.anchor_big = ((bool)SDL_GameControllerGetButton(Controller, SDL_CONTROLLER_BUTTON_A));
	controller_1.grab_big = ((bool)SDL_GameControllerGetButton(Controller, SDL_CONTROLLER_BUTTON_B));
	controller_1.slam_big = ((bool)SDL_GameControllerGetButton(Controller, SDL_CONTROLLER_BUTTON_BACK));
	controller_1.whip_small = ((bool)SDL_GameControllerGetButton(Controller, SDL_CONTROLLER_BUTTON_START));
	controller_1.slide_small = ((bool)SDL_GameControllerGetButton(Controller, SDL_CONTROLLER_BUTTON_START));

	return true;
}

// ---------
// Called before quitting
bool j1Input::CleanUp()
{
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
	return true;
}

bool j1Input::GetWindowEvent(j1EventWindow ev)
{
	return windowEvents[ev];
}


void j1Input::GetMousePosition(int& x, int& y)
{
	x = mouse_x;
	y = mouse_y;
}

void j1Input::GetMouseMotion(int& x, int& y)
{
	x = mouse_motion_x;
	y = mouse_motion_y;
}