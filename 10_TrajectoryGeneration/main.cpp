#include <iostream>
#include <vector>
#include "hybrid_breadth_first.h"

using std::cout;
using std::endl;

const int X = 1;
const int _ = 0;
vector<vector<int>> grid = {
  {_,X,X,_,_,_,_,_,_,_,X,X,_,_,_,_,},
  {_,X,X,_,_,_,_,_,_,X,X,_,_,_,_,_,},
  {_,X,X,_,_,_,_,_,X,X,_,_,_,_,_,_,},
  {_,X,X,_,_,_,_,X,X,_,_,_,X,X,X,_,},
  {_,X,X,_,_,_,X,X,_,_,_,X,X,X,_,_,},
  {_,X,X,_,_,X,X,_,_,_,X,X,X,_,_,_,},
  {_,X,X,_,X,X,_,_,_,X,X,X,_,_,_,_,},
  {_,X,X,X,X,_,_,_,X,X,X,_,_,_,_,_,},
  {_,X,X,X,_,_,_,X,X,X,_,_,_,_,_,_,},
  {_,X,X,_,_,_,X,X,X,_,_,X,X,X,X,X,},
  {_,X,_,_,_,X,X,X,_,_,X,X,X,X,X,X,},
  {_,_,_,_,X,X,X,_,_,X,X,X,X,X,X,X,},
  {_,_,_,X,X,X,_,_,X,X,X,X,X,X,X,X,},
  {_,_,X,X,X,_,_,X,X,X,X,X,X,X,X,X,},
  {_,X,X,X,_,_,_,_,_,_,_,_,_,_,_,_,},
  {X,X,X,_,_,_,_,_,_,_,_,_,_,_,_,_,} };

const vector<double> start = { 0.0, 0.0, 0.0 };
const vector<int> goal = { (int)grid.size() - 1, (int)grid[0].size() - 1 };


int main()
{
	cout << "Finding path through grid:" << endl;

	for (int i = 0; i < grid.size(); ++i)
	{
		cout << grid[i][0];
		for (int j = 0; i < grid[0].size(); ++j)
			cout << "," << grid[i][j];
		cout << endl;
	}

	HBF hbf = HBF();
	HBF::maze_path get_path = hbf.search(grid, start, goal);
	vector<HBF::maze_state> show_path = hbf.reconstruct_path(get_path.came_from, start, get_path.final);
	
	cout << "show path from start to finish" << endl;
	for (int i = show_path.size() - 1; i >= 0; --i)
	{
		HBF::maze_state step = show_path[i];
		cout << "##### step " << step.g << " #####" << endl;
		cout << "x " << step.x << endl;
		cout << "y " << step.y << endl;
		cout << "theta " << step.theta << endl;
	}

	return 0;
}