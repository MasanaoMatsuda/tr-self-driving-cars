#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "road.h"
#include "vehicle.h"

using std::map;
using std::string;
using std::vector;

Road::Road(int speed_limit, double traffic_density, vector<int>& lane_speeds)
	: num_lanes(lane_speeds.size)
	, lane_speeds(lane_speeds)
	, speed_limit(speed_limit)
	, density(traffic_density)
	, camera_center(update_width / 2)
{ }

Road::~Road() { }

Vehicle Road::get_ego() { return vehicles.find(ego_key) }