#include <iostream>
#include <random>
#include "GPS.h"

using std::normal_distribution;

void printSamples(GPS gps);

int main()
{
	GPS gps = GPS(4983, 5029, 1.201);

	printSamples(gps);

	return 0;
}


void printSamples(GPS gps)
{
	std::default_random_engine gen;
	double std_x = 2;
	double std_y = 2;
	double std_theta = 0.05;

	normal_distribution<double> gauss_x(gps.getX(), std_x);
	normal_distribution<double> gauss_y(gps.getY(), std_y);
	normal_distribution<double> gauss_theta(gps.getTheta(), std_theta);

	for (int i = 0; i < 3; ++i)
	{
		double sample_x = gauss_x(gen);
		double sample_y = gauss_y(gen);
		double sample_theta = gauss_theta(gen);

		std::cout << "Sample " << i + 1 << " " << sample_x << " " << sample_y << " " << sample_theta << std::endl;
	}
}