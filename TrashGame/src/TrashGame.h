#pragma once
#include <SDL\SDL.h>
#include "Renderer.h"
#include "TileHandler.h"

struct Settings
{
	Settings()
	{
		window_width = 400;
		window_height = 600;

		view_width = window_width - 100;
		view_height = window_height - 100;
		view_padding_left = (window_width - view_width) / 2;
		view_padding_top = (window_height - view_height) / 2;

		max_rows = 12;
		max_columns = 6;
		row_count = 1;
		initial_row_count = 1;
		tile_speed = 25.0f;

		tile_width = view_width / max_columns;
		tile_height = view_height / max_rows;

		lose_line_y = view_padding_top + 100.0f;
	}

	int window_width;
	int window_height;
	
	int view_width;
	int view_height;
	int view_padding_left;
	int view_padding_top;

	int max_rows;
	int max_columns;
	int row_count;
	int initial_row_count;
	float tile_speed;

	int tile_width;
	int tile_height;
	// Distance from the top of the view
	int lose_line_y;
};

class TrashGame
{
public:
	TrashGame(SDL_Window*&& window, SDL_Renderer*&& renderer);
	~TrashGame();
	void Run();

private:
	void CleanUp();
	void Update();
	void Render();
	bool IsRectangleInView(const Rectangle& rectangle) const;
	bool PointIsInView(const int x, const int y) const;
	bool PointInRect(const int x, const int y, const Rectangle& r) const;
private:
	SDL_Window* m_window;
	Renderer m_renderer;
	TileHandler m_tile_handler;
	Settings m_settings;

	int m_window_width;
	int m_window_height;

	Rectangle m_view_rectangle;
	Rectangle m_lose_line;
};

TrashGame* CreateGame();
