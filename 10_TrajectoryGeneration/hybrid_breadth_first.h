#ifndef HYBRID_BREADTH_FIRST_H_
#define HYBRID_BREADTH_FIRST_H_

#include <vector>

using std::vector;

using came_from_type = vector<vector<vector<HBF::maze_state>>>;
using closed_type = vector<vector<vector<int>>>;
using grid_type = vector<vector<int>>;

class HBF
{
public:
	HBF();
	virtual ~HBF();

	struct maze_state {
		int f;
		int g;
		double x;
		double y;
		double theta;
	};

	struct maze_path {
		closed_type closed;
		came_from_type came_from;
		maze_state final;
	};

	vector<maze_state> reconstruct_path(const came_from_type& came_from, 
										const vector<double>& start, 
										const HBF::maze_state & final);
	maze_path search(const grid_type& grid, 
					 const vector<double>& start, 
					 const vector<int>& goal);

private:
	int theta_to_stack_number(double theta);
	int idx(double float_num);
	vector<maze_state> expand(maze_state& state);
	HBF::maze_state HBF::get_shortest(const vector<maze_state>& opened);

private:
	const int NUM_THETA_CELLS = 90;
	const double SPEED = 1.45;
	const double LENGTH = 0.5;
};

#endif  // HYBRID_BREADTH_FIRST_H_