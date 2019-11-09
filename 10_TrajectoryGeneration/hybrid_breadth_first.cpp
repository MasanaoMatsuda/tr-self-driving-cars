#include "hybrid_breadth_first.h"
#include <math.h>
#include <iostream>
#include <vector>
#include <algorithm>

HBF::HBF() {}

HBF::~HBF() {}

vector<HBF::maze_state> HBF::reconstruct_path(const came_from_type &came_from, 
											  const vector<double> &start,
											  const HBF::maze_state &final)
{
	vector<maze_state> path = { final };

	int stack = theta_to_stack_number(final.theta);
	maze_state previous = came_from[stack][idx(final.x)][idx(final.y)];
	while (previous.x != start[0] || previous.y != start[1])
	{
		path.push_back(previous);
		stack = theta_to_stack_number(previous.theta);
		previous = came_from[stack][idx(previous.x)][idx(previous.y)];
	}

	return path;
}


HBF::maze_path HBF::search(const grid_type &grid,
						   const vector<double> &start,
						   const vector<int> &goal)
{
	closed_type closed(NUM_THETA_CELLS,
					   grid_type(grid.size(), 
					   vector<int>(grid[0].size())));
	came_from_type came_from(NUM_THETA_CELLS, 
							 vector<vector<maze_state>>(grid.size(), 
							 vector<maze_state>(grid[0].size())));
	
	maze_state state;
	state.g = 0;
	state.x = start[0];
	state.y = start[1];
	state.theta = start[2];
	state.f = g + heuristic(state.x, state.y, goal);

	int stack = theta_to_stack_number(start[2]);
	closed[stack][idx(state.x)][idx(state.y)] = 1;
	came_from[stack][idx(state.x)][idx(state.y)] = state;
	int total_closed = 1;

	vector<maze_state> opened = { state };
	while (!opened.empty())
	{
		sort(opened.begin(), opened.end(), compare_maze_state);
		maze_state current = opened.at(0)		
		opened.erase(current);

		if (idx(current.x) == goal[0] && idx(current.y) == goal[1])
		{
			std::cout << "Found path to goal in " << total_closed << " expansions." << std::endl;
			
			maze_path path;
			path.came_from = came_from;
			path.closed = closed;
			path.final = current;
			
			return path;
		}

		vector<maze_state> next_states = expand(current);
		for (maze_state state: next_states)
		{
			if (state.x < 0 || state.x >= grid.size() || state.y < 0 || state.y >= grid[0].size())
				continue;
			
			int next_stack = theta_to_stack_number(state.theta);
			
			if (closed[next_stack][idx(state.x)][idx(state.y)] != 0 || grid[idx(state.x)][idx(state.y)] != 0)
				continue;
			
			opened.push_back(state);
			closed[next_stack][idx(state.x)][idx(state.y)] = 1;
			came_from[next_stack][idx(state.x)][idx(state.y)] = current;
			++gotal_closed;
		}
	}

	std::cout << "no valid path." << std::endl;

	HBF::maze_path path;
	path.came_from = came_from;
	path.closed = closed;
	path.final = state;

	return path;
}

int HBF::theta_to_stack_number(double theta) {
	double normed_theta = fmod(theta + 2 * M_PI, (2 * M_PI));
	int stack_number = (int)round(normed_theta * NUM_THETA_CELLS / (2 * M_PI)) % NUM_THETA_CELLS;
	return stack_number;
}

int HBF::idx(double float_num)
{
	return int(floor(float_num));
}

vector<HBF::maze_state> HBF::expand(HBF::maze_state &state)
{
	vector<HBF::maze_state> next_states;

	double delta_rad, omega, next_theta;
	for (double delta = -35; delta < 40; delta+=5)
	{
		delta_rad = M_PI / 180.0 * delta;
		omega = SPEED / LENGTH * tan(delta_rad);

		next_theta = state.theta + omega;
		if (next_theta < 0)
			next_theta += 2 * M_PI;

		HBF::maze_state next_state;
		next_state.g = state.g + 1;
		next_state.x = state.x + SPEED * cos(state.theta);
		next_state.y = state.y + SPEED * sin(state.theta);
		next_state.theta = next_theta;

		next_states.push_back(next_state);
	}

	return next_states;
}


bool HBF::compare_maze_state(const maze_state &lhs, const maze_state &rhs)
{
	return lhs.f < rhs.f;
}

double HBF::heuristic(double x, double y, vector<int> &goal)
{
	return fabs(y - goal[0]) + fabs(x - goal[1]);
}

//HBF::maze_state HBF::get_shortest(const vector<maze_state>& opened)
//{
//	int min_f = 100000;
//	maze_state min_f_state;
//	for (maze_state state: opened)
//	{
//		int f = state.g + heuristics[idx(state.x)][idx(state.y)];
//		if (f < min_f)
//		{
//			min_f = f;
//			min_f_state = state;
//		}
//	}
//
//	return min_f_state;
//}