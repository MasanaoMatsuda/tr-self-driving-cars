#ifndef STATISTICS_H
#define STATISTICS_H
#define _USE_MATH_DEFINES

#include <math.h>
#include <vector>

static class Statistics
{
public:
	float norm_pdf(float x, float mu, float std);
	std::vector<float> norm_vector(std::vector<float> input_vec);
};

#endif //STATISTICS_H
