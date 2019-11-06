#ifndef VEHICLE_H_
#define VEHICLE_H_

#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;


class Vehicle
{
public:
	Vehicle();
	Vehicle(int lane, float s, float v, float a, string state = "CS");
	~Vehicle();

	vector<Vehicle> choose_next_state(map<int, vector<Vehicle>>& predictions);
	vector<string> successor_states();
	vector<Vehicle> generage_trajectory(strnig state, map<int, Vehicle >> &predictions);
	vector<float> get_kinematics(map<int, vector<Vehicle>>& predictions, int lane);
	vector<Vehicle> constant_speed_trajectory();
	vector<Vehicle> keep_lane_trajectory(map<int, vector<Vehicle>>& predictions);
	vector<Vehicle> lane_change_trajectory(string state, map<int, vector<Vehicle>>& predictions);
	
	void increment(int dt);
	float position_at(int t);
	bool get_vehicle_behind(map<int, vector<Vehicle>>& predictions, int lane, Vehicle& rVehicle);
	bool get_vehicle_ahead(map<int, vector<Vehicle>>& predictions, int lane, Vehicle& rVehicle);
	vector<Vehicle> generate_predictions(int horizon = 2);
	void realize_next_state(vector<Vehicle>& trajectory);
	void configure(vector<int>& road_data);
	
	struct collider
	{
		bool collision;
		int time;
	};

	map<string, int> lane_direction = { {"PLCL", 1},
										{"LCL", 1},
										{"LCR", -1},
										{"PLCR", -1} };
	
	int L = 1;
	int preffered_buffer = 6;
	int lane, s, goal_lane, goal_s, lanes_available;
	float v, target_speed, a, max_acceleration;
	string current_state;
};

#endif // VEHICLE_H_