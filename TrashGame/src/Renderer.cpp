#include "Renderer.h"
#include <cassert>

Renderer::Renderer(SDL_Window* window)
{
	assert(window);
	m_sdl_renderer = SDL_CreateRenderer(
		window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
}

Renderer::Renderer(SDL_Renderer* renderer)
{
	assert(renderer);
	m_sdl_renderer = renderer;
}

void Renderer::SetDrawColor(const Color& color)
{
	SDL_SetRenderDrawColor(m_sdl_renderer, color.r, color.g, color.b, 255);
}

void Renderer::SetDrawColor(unsigned char r, unsigned char g, unsigned char b)
{
	SDL_SetRenderDrawColor(m_sdl_renderer, r, g, b, 255);
}

void Renderer::SetClearColor(const Color& color)
{
	m_default_clear_color = color;
}

void Renderer::SetClearColor(unsigned char r, unsigned char g, unsigned char b)
{
	m_default_clear_color = { r, g, b };
}

void Renderer::Clear()
{
	SetDrawColor(m_default_clear_color);
	SDL_RenderClear(m_sdl_renderer);
}

void Renderer::DrawRect(const Rectangle& rectangle, RenderFlag flag)
{
	SDL_Rect tmp = RectangleToSDLRectangle(rectangle);
	SetDrawColor(rectangle.color);
	DrawWithFlag(tmp, flag);
}
void Renderer::DrawTile(const Tile& tile, const Rectangle& viewport, const Color& color, RenderFlag flag)
{
	SDL_Rect tmp = CalculateSDLRectForTile(tile, viewport);
	SetDrawColor(color);
	DrawWithFlag(tmp, flag);
}

void Renderer::DrawTile(const Tile& tile, const Rectangle& viewport, RenderFlag flag)
{
	SDL_Rect tmp = CalculateSDLRectForTile(tile, viewport);
	SetDrawColor(tile.color);
	DrawWithFlag(tmp, flag);
}

SDL_Rect Renderer::RectangleToSDLRectangle(const Rectangle& rectangle) const
{
	return SDL_Rect(rectangle.GetX(), rectangle.GetY(), rectangle.width, rectangle.height);
}

SDL_Rect Renderer::CalculateSDLRectForTile(const Tile& tile, const Rectangle& viewport) const
{
	int height;
	int y_position;

	// Rectangles top is below the top border
	if (tile.GetY() > viewport.GetY())
	{
		// The rectangle appears from the bottom
		y_position = tile.GetY();
		height = std::min(viewport.GetY() + viewport.height - tile.GetY(), tile.height);
	}
	else
	{
		// The rectangle disappears at the top
		y_position = viewport.GetY();
		height = tile.height - (viewport.GetY() - tile.GetY());
	}

	return SDL_Rect(tile.GetX(), y_position, tile.width, height);
}

void Renderer::DrawWithFlag(const SDL_Rect& rect, RenderFlag flag)
{
	// Possibly avoid using try-catch in "performance critical" ares of the application
	try
	{
		if (flag == RenderFlag::Fill)
		{
			SDL_RenderFillRect(m_sdl_renderer, &rect);
		}
		else if (flag == RenderFlag::NoFill)
		{
			SDL_RenderDrawRect(m_sdl_renderer, &rect);
		}
		else
		{
			throw std::runtime_error("Unrecognized RenderFlag used");
		}
	}
	catch (const std::exception& exception)
	{
		std::cout << "Error: " << exception.what() << std::endl;
	}
}


void Renderer::Swap()
{
	SDL_RenderPresent(m_sdl_renderer);
}