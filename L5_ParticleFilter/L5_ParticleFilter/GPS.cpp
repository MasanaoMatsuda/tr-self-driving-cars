#include "GPS.h"

GPS::GPS(double x, double y, double theta)
	:x(x), y(y), theta(theta)
{

};


GPS::~GPS()
{
	
};

double GPS::getX() { return x; };
double GPS::getY() { return y; };
double GPS::getTheta() { return theta; };