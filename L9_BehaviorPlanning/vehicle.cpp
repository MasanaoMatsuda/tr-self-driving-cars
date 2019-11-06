#include "vehicle.h"
#include <algorithm>


Vehicle::Vehicle();

Vehicle::Vehicle(int lane, float s, float v, float a, string state = "CS")
	: lane(lane)
	, s(s)
	, v(v)
	, a(a)
	, current_state(state)
	, max_acceleration(-1)
{ }

Vehicle::~Vehicle();

vector<Vehicle> Vehicle::choose_next_state(map<int, vector<Vehicle>>& predictions)
{
	possible_states = successor_states();

	map<string, int> costs;
	for (auto state : possible_states)
	{
		generage_trajectory(state, costs);
	}
}

vector<string> Vehicle::successor_states()
{
	vector<string> possible_states;
	possible_states.push_back("KL");

	if (current_state.compare("KL") == 0)
	{
		possible_states.push_back("PLCL");
		possible_states.push_back("PLCR");
	}
	else if (current_state.compare("PLCL") == 0)
	{
		if (lane != lanes_available - 1)
		{
			possible_states.push_back("PLCL");
			possible_states.push_back("LCL");
		}
	}
	else if (current_state.compare("PLCR") == 0)
	{
		if (lane != 0)
		{
			possible_states.push_back("PLCR");
			possible_states.push_back("LCR");
		}
	}

	return states;
}

vector<Vehicle> Vehicle::generage_trajectory(strnig state, map<int, Vehicle >> &predictions)
{
	vector<Vehicle> trajectory;
	if (state.compare("CS") == 0)
		trajectory = constant_speed_trajectory();
	else if (state.compare("KL") == 0)
		trajectory = keep_lane_trajectory(predictions);
	else if (state.compare("LCL") == 0 || state.compare("LCR") == 0)
		trajectory = lane_change_trajectory(state, predictions);
}

vector<float> Vehicle::get_kinematics(map<int, vector<Vehicle>>& predictions, int lane)
{
	float max_velocity_accel_limit = max_acceleration + v;
	float new_velocity;

	Vehicle vehicle_ahead;
	Vehicle vehicle_behind;
	if (get_vehicle_ahead(predictions, lane, vehicle_ahead))
		if get_vehicle_behind(predictions, lane, vehicle_behind)
			new_velocity = vehicle_ahead.v;
		else
		{
			float max_velocity_in_front =
				(vehicle_ahead.s - s - preffered_buffer) + vehicle_ahead.v - 0.5 * a;
			new_velocity = std::min(
				std::min(max_velocity_in_front, max_velocity_accel_limit),
				target_speed
			);
		}
	else
		new_velocity = std::min(max_velocity_accel_limit, target_speed);

	float new_accel = new_velocity - v; // Equation: (v1 - v0)/t = acceleration
	float new_position = s + new_velocity + new_accel / 2.0;

	return { new_position, new_velocity, new_accel };
}




vector<Vehicle> Vehicle::constant_speed_trajectory()
{
	float next_pos = position_at(1);
	vector<Vehicle> trajectories = {
		Vehicle(lane, s, v, a, state),
		Vehicle(lane, next_pos, v, 0, state)
	};
	return trajectories
}

vector<Vehicle> Vehicle::keep_lane_trajectory(map<int, vector<Vehicle>>& predictions)
{
	vector<Vehicle> trajectory = {
		Vehicle(lane, s, v, a, state)
	};
	vector<float> kinematics = get_kinematics(predictions, lane);
	trajectory.push_back(Vehicle(lane, kinematics[0], kinematics[1], kinematics[2], "KL"));

	return trajectory;
}



float Vehicle::position_at(int t)
{
	return s + v * t + a * t * t / 2.0;
}


bool Vehicle::get_vehicle_behind(map<int, vector<Vehicle>>& predictions, int lane, Vehicle& rVehicle)
{
	int max_s = -1;
	Vehicle temp_vehicle;
	bool found_vehicle = false;
	for (auto iter = predictions.begin(); iter != predictions.end(); ++iter)
	{
		temp_vehicle = iter->second[0];
		if (temp_vehicle.lane == lane && temp_vehicle.s < s && temp_vehicle.s > max_s)
		{
			max_s = temp_vehicle.s;
			rVehicle = temp_vehicle;
			found_vehicle = true;
		}
	}

	return found_vehicle;
}

bool Vehicle::get_vehicle_ahead(map<int, vector<Vehicle>>& predictions, int lane, Vehicle& rVehicle)
{
	int min_s = goal_s;
	Vehicle temp_vehicle;
	bool found_vehicle = false;
	for (auto iter = predictions.begin(); iter != predictions.end(); ++iter)
	{
		temp_vehicle = iter->second[0];
		if (temp_vehicle.lane == lane && temp_vehicle.s > s&& temp_vehicle.s < min_s)
		{
			min_s = temp_vehicle.s;
			rVehicle = temp_vehicle;
			found_vehicle = true
		}
	}
	return found_vehicle;
}