#ifndef ROAD_H_
#define ROAD_H_

#include <map>
#include <string>
#include <vector>
#include "vehicle.h"


class Road
{
public:
	Road(int speed_limit, double traffic_density, std::vector<int>& lane_speeds);
	virtual ~Road();

	Vehicle get_ego();
	void populate_traffic();
	void advance();
	void display(int timestep);
	void add_ego(int lane_num, int s, std::vector<int>& config_data);
	void cull();

	int update_width = 70;
	int vehicles_added = 0;
	int ego_key = -1;
	int num_lanes, speed_limit, camera_center;
	double density;
	std::map<int, Vehicle> vehicles;
	std::string ego_rep = " *** ";
	std::vector<int> lane_speeds;
};

#endif // ROAD_H_