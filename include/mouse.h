#pragma once

#include <iostream>
#include <SDL2pp/SDL2pp.hh>
#include "common_utils.h"
#include "client_camera.h"


class MouseHandler {
	private:
		SDL_Event * mouse_event;
		Camera & camera;
		size_t tile_dim;
		Position mpos_on_click;
		bool clicked;
	public:
		MouseHandler(size_t tile_dim,Camera & camera);
		void getEvent(SDL_Event * mouse_event);
		Position currentPosition();
		Position clickedPosition();
		bool leftClick(); // Check for left click
		bool rightClick(); // Check for right click
		void click(); // Mark as clicked and update click position
		void unclick(); // Mark as un-clicked and update click position
		Area getSelection(Position init_pos, Position final_pos); // 		Get area from two positions
};

