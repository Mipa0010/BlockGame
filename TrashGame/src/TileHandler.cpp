#include "TileHandler.h"
#include "Rectangle.h"
#include <iterator>

TileHandler::TileHandler()
{
	m_tile_width = 0;
	m_tile_height = 0;
	m_origin = Rectangle();
}

TileHandler::TileHandler(int tile_width, int tile_height, size_t column_max, const Rectangle& rect) :
	m_tiles(column_max), m_selected(), m_needs_update(), m_colors(), m_tile_width(tile_width), m_tile_height(tile_height), m_origin(rect)
{
	AddColors();
}

void TileHandler::GenerateRows(int row_count)
{
	size_t column_max = m_tiles.size();
	for (int column_index = 0; column_index < column_max; ++column_index)
	{
		auto& column = m_tiles[column_index];

		for (int row_index = 0; row_index < row_count; ++row_index)
		{
			int color_index = rand() % m_colors.size();

			// For the x and y coordinates of the new tile
			float x = .0f;
			float y = .0f;

			if (!column.empty())
			{
				// Get an iterator to the current last item
				auto last_item = column.end();
				--last_item;

				// Use the x position from the current last tile
				x = static_cast<float>(last_item->GetX());

				// See if the current last tile is falling
				auto falldata = std::find_if(m_needs_update.begin(), m_needs_update.end(), [&](const FallData& data) {
					if (data.location == TileLocation(&column, last_item))
					{
						return (true);
					}
					else
					{
						return (false);
					}
				});

				// If the current last tile is falling  adjust the spawn location accordingly
				// else spawn the tile right below the existing tile
				if (falldata == m_needs_update.end())
				{
					y = static_cast<float>(last_item->y + m_tile_height);
				}
				else
				{
					y = static_cast<float>(last_item->y + falldata->distance_left + m_tile_height);
				}
			}
			else
			{
				// Spawn new tiles right below the view rectangle
				x = static_cast<float>(m_origin.x + column_index * m_tile_width);
				y = static_cast<float>(m_origin.y + m_origin.height + row_index * m_tile_height);
			}

			// Finally construct the new tile in the current column
			column.emplace_back(
				x,
				y,
				m_tile_width,
				m_tile_height,
				m_colors[color_index]
			);
		}
	}
}

// Is responsible for moving the tiles smoothly based on the delta between frames
void TileHandler::SmoothUpdate(float speed, float delta)
{
	for (auto& list : m_tiles)
	{
		for (auto& tile : list)
		{
			tile.y -= speed * delta;
		}
	}
}

// Moves tiles upwards by m_tile_height
void TileHandler::RowBasedUpdate()
{
	for (auto& list : m_tiles)
	{
		for (auto& tile : list)
		{
			tile.y -= m_tile_height;
		}
	}
}

void TileHandler::UpdateFalling(float delta)
{
	for (auto& fall_data : m_needs_update)
	{
		float step = (0.1f / delta) * fall_data.distance_left * delta;
		// Prevent the step from being too small so that it will eventually
		// deplete the distance_left
		step = std::max(step, 1.0f);

		if (fall_data.distance_left - step > 0)
		{
			// Tile still falling
			fall_data.location.tile_it->y += step;
			fall_data.distance_left -= step;
		}
		else
		{
			// Current step gaps the distance
			step = fall_data.distance_left;
			fall_data.distance_left = 0;
			fall_data.location.tile_it->y = std::round(fall_data.location.tile_it->y + step);
		}
	}

	UpdateFallData();
}

std::list<Tile>& TileHandler::GetColumn(int index)
{
	if (index < 0 || index > m_tiles.size() - 1)
	{
		throw std::runtime_error("GetColumn index out of range");
	}

	return (m_tiles[index]);
}

void TileHandler::HandleClick(int mouse_x, int mouse_y)
{
	// Over which column the click happened?
	int column_index = (mouse_x - m_origin.GetX()) / m_tile_width;
	auto& column = GetColumn(column_index);

	auto tile_it = column.begin();
	for (; tile_it != column.end(); ++tile_it)
	{
		if (tile_it->PointInRect(mouse_x, mouse_y))
		{
			TileLocation new_location(&column, tile_it);

			// If m_selected is empty place selected tile into it
			if (!m_selected.empty())
			{
				// Check that the tile isnt already in m_selected and 
				bool already_selected = false;
				for (TileLocation& current_location : m_selected)
				{
					if (current_location == new_location)
					{
						already_selected = true;
						break;
					}
				}

				if (!already_selected)
				{
					// check that the tiles color matches with the previous selections
					if (tile_it->color == m_selected[0].tile_it->color)
					{
						m_selected.emplace_back(new_location);
					}
					else
					{
						m_selected.clear();
						m_selected.emplace_back(new_location);
					}
				}
			}
			else
			{
				m_selected.emplace_back(new_location);
			}
		}
	}

	UpdateSelection();
}

void TileHandler::AddColors()
{
	m_colors.emplace_back(Color(0, 0, 255));
	m_colors.emplace_back(Color(0, 255, 0));
	m_colors.emplace_back(Color(255, 0, 0));
	m_colors.emplace_back(Color(255, 255, 255));
	m_colors.emplace_back(Color(0, 0, 0));
	m_colors.emplace_back(Color(255, 255, 0));
	m_colors.emplace_back(Color(255, 0, 255));
}

void TileHandler::UpdateSelection()
{
	// Whenever the player matches three tiles of the same color
	// remove the selected three tiles and make the tiles above the selected ones fall down
	// to close any gaps 
	if (m_selected.size() == 3)
	{
		for (auto& selection : m_selected)
		{
			auto& column = selection.origin;
			auto& it_selected = selection.tile_it;

			// If the selected tile isnt the top most tile of the column
			// the tiles above it need to fall down to place
			// 
			if (it_selected != column->begin())
			{
				// Go through the tiles above the current selection
				// and create FallData for each tile
				std::list<Tile>::iterator above = it_selected;

				while (above != column->begin())
				{
					--above;
					// Check if there is falldata for the tile already
					auto test_it = std::find_if(m_needs_update.begin(), m_needs_update.end(), [&](const FallData& data) {
						if (data.location == TileLocation(column, above))
						{
							return true;
						}
						else
						{
							return false;
						}
						});

					// if falldata exist update it else create new instance of falldata
					if (test_it != m_needs_update.end())
					{
						test_it->distance_left += static_cast<float>(m_tile_height);
					}
					else
					{
						m_needs_update.emplace_back(TileLocation(column, above), static_cast<float>(m_tile_height));
					}
						//std::cout << "Fall data created" << std::endl;
				}
			}

			// Check the falldata vector for tiles that are going to be deleted 
			// and delete the appropriate falldata
			std::vector<FallData>::iterator it_ = m_needs_update.begin();
			while (it_ != m_needs_update.end())
			{
				if (it_->location != selection)
				{
					it_++;
				}
				else
				{
					it_ = m_needs_update.erase(it_);
					//std::cout << "Fall data removed" << std::endl;
				}
			}

			// Removes selected tile
			column->erase(it_selected);
		}
		
		m_selected.clear();
	}
}

// Whenever a tile has been moved downwards enough we remove the falldata for that tile
void TileHandler::UpdateFallData()
{
	auto it = m_needs_update.begin();

	while (it != m_needs_update.end())
	{
		if (it->distance_left != 0)
		{
			it++;
		}
		else
		{
			it = m_needs_update.erase(it);
		}
	}
}

void TileHandler::ClearTiles()
{
	m_selected.clear();
	m_needs_update.clear();

	for (auto& col : m_tiles)
	{
		col.clear();
	}
}