class GPS
{
	double x;
	double y;
	double theta;
public:
	GPS(double x, double y, double theta);
	~GPS();
	double getX();
	double getY();
	double getTheta();
};