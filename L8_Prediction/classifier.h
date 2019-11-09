#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

#include "./../Eigen/Dense"
#include <string>
#include <vector>


using std::string;
using std::vector;
using Eigen::ArrayXd;

class GNB 
{
	ArrayXd left_means;
	ArrayXd keep_means;
	ArrayXd right_means;

	ArrayXd left_stds;
	ArrayXd keep_stds;
	ArrayXd right_stds;

	int left_prior;
	int keep_prior;
	int right_prior;

public:
	GNB();
	virtual ~GNB();
	void train(const vector<vector<double>>& data, const vector<string>& labels);
	string predict(const vector<double>& sample);
	vector<string> possible_labels = { "left", "keep", "right" };
};

#endif // !CLASSIFIER_H_
