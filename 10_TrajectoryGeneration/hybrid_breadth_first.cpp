#include <math.h>
#include <iostream>
#include <vector>
#include "hybrid_breadth_first.h"

using maze_grid = vector<vector<int>>;

HBF::HBF() {}

HBF::~HBF() {}

int HBF::theta_to_stack_number(double theta) {
	// Takes an angle (in radians) and returns which "stack" in the 3D 
	//   configuration space this angle corresponds to. Angles near 0 go in the 
	//   lower stacks while angles near 2 * pi go in the higher stacks.
	double normed_theta = fmod(theta, (2 * M_PI));
	int stack_number = (int)(round(normed_theta * NUM_THETA_CELLS / (2 * M_PI))) % NUM_THETA_CELLS;
	return stack_number;
}

int HBF::get_grid_idx(double float_num)
{
	return int(floor(float_num));
}

vector<HBF::maze_state> HBF::expand(HBF::maze_state& state)
{
	vector<HBF::maze_state> next_states;

	int next_g = state.g + 1;
	for (double delta_i = -35; delta_i < 40; delta_i += 5)
	{
		double delta = M_PI / 180.0 * delta_i;
		double omega = SPEED / LENGTH * tan(delta);
		double next_theta = state.theta + omega;
		if (next_theta < 0)
			next_theta += 2 * M_PI;
		double next_x = state.x + SPEED * cos(theta);
		double next_y = state.y + SPEED * sin(theta);

		HBF::maze_state next_state;
		next_state.g = next_g;
		next_state.x = next_x;
		next_state.y = next_y;
		next_state.theta = next_theta;
		next_states.push_back(next_state);
	}

	return next_states;
}

vector<HBF::maze_state> HBF::reconstruct_path(
	vector<vector<vector<HBF::maze_state>>>& came_from,
	vector<double>& start, 
	HBF::maze_state & final)
{
	vector<maze_state> path = { final };
	
	int stack = theta_to_stack_number(final.theta);
	maze_state current = came_from[stack][get_grid_idx(final.x)][get_grid_idx(final.y)];
	stack = theta_to_stack_number(current.theta);

	double x = current.x;
	double y = current.y;

	while (x != start[0] || y != start[1])
	{
		path.push_back(current);
		current = came_from[stack][get_grid_idx(x)][get_grid_idx(y)];
		x = current.x;
		y = current.y;
		stack = theta_to_stack_number(current.theta);
	}

	return path;
}


HBF::maze_path HBF::search(vector<vector<int>>& grid, vector<double>& start, vector<int>& goal)
{
	vector<vector<vector<int>>> closed(
		NUM_THETA_CELLS, vector<vector<int>>(grid[0].size(), vector<int>(grid.size())));

	vector<vector<vector<maze_state>>> came_from(
		NUM_THETA_CELLS, vector<vector<maze_state>>(grid[0].size(), vector<maze_state>(grid.size())));

	int stack = theta_to_stack_number(start[2]);

	maze_state state;
	state.g = 0;
	state.x = start[0];
	state.y = start[1];
	state.theta = start[2];

	closed[stack][get_grid_idx(state.x)][get_grid_idx(state.y)] = 1;
	came_from[stack][get_grid_idx(state.x)][get_grid_idx(state.y)] = state;
	
	int total_closed = 1;
	vector<maze_state> opened = { state };
	bool finished = false;
	while (!opened.empty())
	{
		maze_state current = opened[0];
		opened.erase(opened.begin());

		int x = current.x;
		int y = current.y;

		if (get_grid_idx(x) == goal[0] && get_grid_idx(y) == goal[1])
		{
			std::cout << "found path to goal in " << total_closed << " expansions" << std::endl;
			maze_path path;
			path.came_from = came_from;
			path.closed = closed;
			path.final = current;

			return path;
		}

		vector<maze_state> next_state = expand(current);

		for (int i = 0; i < next_state.size(); ++i)
		{
			int next_g = next_state[i].g;
			double next_x = next_state[i].x;
			double next_y = next_state[i].y;
			double next_theta = next_state[i].theta;

			if ((next_x < 0 || next_x >= grid.size()) || (next_y < 0 || next_y >= grid[0].size()))
				continue;

			int next_stack = theta_to_stack_number(next_theta);

			if (closed[next_stack][get_grid_idx(next_x)][get_grid_idx(next_y)] == 0
				&& grid[get_grid_idx(next_x)][get_grid_idx(next_y)])
			{
				opened.push_back(next_state[i]);
				closed[next_stack][get_grid_idx(next_x)][get_grid_idx(next_y)] = 1;
				came_from[next_stack][get_grid_idx(next_x)][get_grid_idx(next_y)] = current;
				++total_closed;
			}
		}
	}

	std::cout << "no valid path." << std::endl;
	HBF::maze_path path;
	path.came_from = came_from;
	path.closed = closed;
	path.final = state;

	return path;
}
