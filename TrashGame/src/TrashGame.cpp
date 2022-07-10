#include "TrashGame.h"
#include <vector>
#include <array>
#include <list>

#include "TileHandler.h"
#include "Time.h"

TrashGame::TrashGame(SDL_Window*&& window, SDL_Renderer*&& renderer)
	: m_window(window),  m_renderer(renderer)
{
	// TrashGame::m_renderer has ownership of the SDL_Renderer
	renderer = nullptr;

	// TrashGame::m_window has ownership of the window
	window = nullptr;

	SDL_SetWindowSize(m_window, m_settings.window_width, m_settings.window_height);
	SDL_SetWindowPosition(m_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	
	m_view_rectangle = Rectangle(
		static_cast<float>(m_settings.view_padding_left), 
		static_cast<float>(m_settings.view_padding_top),
		m_settings.view_width,
		m_settings.view_height,
		Color(0, 0, 255)
	);


	m_lose_line = Rectangle(m_view_rectangle.x, m_settings.lose_line_y, m_view_rectangle.width, 10, Color(255, 0, 0));


	m_renderer.SetClearColor(Color(255, 255, 180));

	m_tile_handler = TileHandler(m_settings.tile_width, m_settings.tile_height, m_settings.max_columns, m_view_rectangle);

	srand(static_cast<unsigned int>(time(nullptr)));
}

TrashGame::~TrashGame()
{
	CleanUp();
}

void TrashGame::Run()
{
	m_tile_handler.GenerateRows(m_settings.initial_row_count);
	m_tile_handler.RowBasedUpdate();

	// For frame time calculations
	Time time;
	
	// in seconds
	float timer_max = 6;
	float timer = timer_max;

	bool paused = false;
	bool open = true;
	while (open)
	{
		// Timers
		time.Update();

		// Input handling
		bool mouse_down = false;
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				open = false;
				break;
			case SDL_MOUSEWHEEL:
				if(e.wheel.y > 0)
				{
					if (timer_max == 0.5)
					{
						timer_max = 1;
					}
					else
					{
						timer_max += 1 * e.wheel.y;
					}
				}
				else
				{
					timer_max += 1 * e.wheel.y;
					if (timer_max <= 0)
						timer_max = 0.5;
				}

				timer = timer_max;
				std::cout << timer << "/" << timer_max << std::endl;
				break;
			case SDL_MOUSEBUTTONDOWN:
				mouse_down = true;
				break;
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_p)
					paused = !paused;
				if (e.key.keysym.sym != SDLK_SPACE)
					mouse_down = true;
				else
					m_tile_handler.m_selected.clear();
				break;
			}
		}

		if (!open)
		{
			break;
		}

		int mouse_x, mouse_y;
		SDL_GetMouseState(&mouse_x, &mouse_y);

		// Game logic

		// Handle input;
		if (mouse_down && PointIsInView(mouse_x, mouse_y))
		{
			m_tile_handler.HandleClick(mouse_x, mouse_y);
		}

		if(!paused)
		{
			timer -= time.delta_time;

			if (timer <= 0)
			{
				m_tile_handler.GenerateRows(1);
				m_tile_handler.RowBasedUpdate();
				timer = timer_max;
			}
		}

		m_tile_handler.UpdateFalling(time.delta_time);

		Render();
	}
}

void TrashGame::CleanUp()
{
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer.m_sdl_renderer);
	SDL_Quit();
}

void TrashGame::Update()
{

}

void TrashGame::Render()
{
	// Rendering logic
//Background 
	m_renderer.Clear();

	// Defeat line
	m_renderer.DrawRect(m_lose_line);

	// Tiles
	for (auto& column : m_tile_handler.GetTiles())
	{
		for (auto& tile : column)
		{
			if (IsRectangleInView(tile))
			{
				m_renderer.DrawTile(tile, m_view_rectangle);
			}
		}
	}

	// Give selected tiles a border
	for (TileLocation& location : m_tile_handler.SelectedTiles())
	{
		m_renderer.DrawTile(*(location.tile_it), m_view_rectangle, Color(255, 174, 25), Renderer::RenderFlag::NoFill);
	}

	// Game view border
	m_renderer.DrawRect(m_view_rectangle, Renderer::RenderFlag::NoFill);

	m_renderer.Swap();
}

// Only handles Y-Axis for now
bool TrashGame::IsRectangleInView(const Rectangle& rect) const
{
	return (rect.GetY() + rect.height > m_view_rectangle.GetY() && rect.GetY() < m_view_rectangle.GetY() + m_view_rectangle.height);
}

bool TrashGame::PointIsInView(const int x, const int y) const
{
	const Rectangle& viewport = m_view_rectangle;

	return(x > viewport.GetX() && x < viewport.GetX() + viewport.width && y > viewport.GetY() && y < viewport.GetY() + viewport.height);
}

bool TrashGame::PointInRect(const int x, const int y, const Rectangle& r) const
{
	return (x > r.GetX() && x < r.GetX() + r.width && y > r.GetY() && y < r.GetY() + r.height);
}

TrashGame* CreateGame()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		throw std::runtime_error("Failed to initialize SDL");
	}

	SDL_Window* window = SDL_CreateWindow("Trash game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 600, SDL_WINDOW_ALLOW_HIGHDPI);

	if (!window)
	{
		SDL_Quit();
		throw std::runtime_error("Failed to open window");
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!renderer)
	{
		SDL_DestroyWindow(window);
		SDL_Quit();
		throw std::runtime_error("Failed to create renderer");
	}

	TrashGame* game = new TrashGame(std::move(window), std::move(renderer));
	
	return (game);
}