#pragma once
#include <vector>
#include <list>

#include "Tile.h"
#include "Color.h"

struct TileLocation
{
	std::list<Tile>* origin = nullptr;
	std::list<Tile>::iterator tile_it;

	TileLocation(std::list<Tile>* _origin, std::list<Tile>::iterator it) : origin(_origin), tile_it(it) { }
	inline bool operator==(const TileLocation& other) { return (origin == other.origin && tile_it == other.tile_it); }
	inline bool operator!=(const TileLocation& other) { return (origin != other.origin || tile_it != other.tile_it); }
};

struct FallData
{
	FallData(const TileLocation& location_, float y_diff) :
		location(location_), distance_left(y_diff) { }
	FallData(const TileLocation&& location_, float y_diff) : 
		location(location_), distance_left(y_diff) { }

	TileLocation location;
	float distance_left;
};

class TileHandler
{
public:
	TileHandler();
	TileHandler(int tile_width, int tile_height, size_t column_max, const Rectangle& origin);
public:
	void GenerateRows(int row_count);
	void SmoothUpdate(float speed, float delta);
	void RowBasedUpdate();
	std::vector<std::list<Tile>>& GetTiles() { return (m_tiles); }
	std::list<Tile>& GetColumn(int index);
	void HandleClick(int mouse_x, int mouse_y);
	std::vector<TileLocation>& SelectedTiles() { return (m_selected); }
	void UpdateFalling(float delta);

private:
	void AddColors();
	void UpdateSelection();
	void UpdateFallData();
public:
	std::vector<std::list<Tile>> m_tiles;
	std::vector<TileLocation> m_selected;
	std::vector<FallData> m_needs_update;
	std::vector<Color> m_colors;
	int m_tile_width;
	int m_tile_height;
	Rectangle m_origin;
};