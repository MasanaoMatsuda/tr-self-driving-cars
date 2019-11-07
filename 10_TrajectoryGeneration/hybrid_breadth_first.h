#ifndef HYBRID_BREADTH_FIRST_H_
#define HYBRID_BREADTH_FIRST_H_

#include <vector>

using std::vector;

class HBF
{
public:
	HBF();
	virtual ~HBF();

	struct maze_state {
		int g;
		double x;
		double y;
		double theta;
	};

	struct maze_path {
		vector<vector<vector<int>>> closed;
		vector<vector<vector<maze_s>>> came_from;
		maze_s final;
	};

	int theta_to_stack_number(double theta);
	int get_grid_idx(double float_num);
	vector<maze_state> expand(maze_state& state);
	vector<maze_state> reconstruct_path(vector<vector<vector<maze_state>>>& came_from,
		vector<double>& start, HBF::maze_state & final);

	maze_path search(vector<vector<int>>& grid, vector<double>& start,
		vector<int>& goal);

private:
	const int NUM_THETA_CELLS = 90;
	const double SPEED = 1.45;
	const double LENGTH = 0.5;
};

#endif  // HYBRID_BREADTH_FIRST_H_