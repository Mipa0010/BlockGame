#pragma once
#include <SDL\SDL_render.h>

#include "Tile.h"

class TrashGame;

// A wrapper around SDL renderering for this specific project
class Renderer
{
public:
	enum class RenderFlag { Fill = 0, NoFill = 1 };

	Renderer(SDL_Window* window);
	Renderer(SDL_Renderer* renderer);

	Renderer(const Renderer& other) = delete;
	Renderer& operator=(const Renderer& other) = delete;

	void SetDrawColor(const Color& color);
	void SetDrawColor(unsigned char r, unsigned char g, unsigned char b);
	void SetClearColor(const Color& color);
	void SetClearColor(unsigned char r, unsigned char g, unsigned char b);
	
	void DrawRect(const Rectangle& rectangle, RenderFlag flag = RenderFlag::Fill);
	void DrawTile(const Tile& tile, const Rectangle& viewport, RenderFlag flag = RenderFlag::Fill);
	void DrawTile(const Tile& tile, const Rectangle& viewport, const Color& color, RenderFlag flag = RenderFlag::Fill);
	void Clear();
	void Swap();

private:
	SDL_Rect RectangleToSDLRectangle(const Rectangle& rectangle) const;
	SDL_Rect CalculateSDLRectForTile(const Tile& tile, const Rectangle& rectangle) const;
	void DrawWithFlag(const SDL_Rect& rect, RenderFlag flag);
private:
	Color m_default_clear_color = Color(0, 0, 0);
	SDL_Renderer* m_sdl_renderer = nullptr;
private:
	friend TrashGame;
};

