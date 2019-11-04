#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "classifier.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::vector;

vector<vector<double>> load_state(string file_name);
vector<string> load_label(string file_name);

int main()
{
	vector<vector<double>> x_train = load_state("./train_states.txt");
	vector<vector<double>> x_text = load_state("./test_states.txt");
	vector<string> y_train = load_label("./train_labels.txt");
	vector<string> y_test = load_label("./test_labels.txt");
	cout << "X_train number of elements: " << x_train.size() << endl;
	cout << "X_traing element size: " << x_train.at(0).size() << endl;
	cout << "Y_train number of elements: " << y_train.size() << endl;

	GNB gnb = GNB();
	gnb.train(x_train, y_train);

	cout << "x_test number of elements: " << x_test.size() << endl;
	cout << "x_test element size: " << x_test.at(0).size() << endl;
	cout << "y_test number of elements: " << y_test.size() << endl;

	int score = 0;
	for (int i = 0; i < x_text.size(); ++i)
	{
		vector<double> coords = x_test.at(i);
		string predicted = gnb.predict(coords);
		if (predicted.compare(y_test.at(i)) == 0)
			score += 1;
	}

	float fraction_correct = float(score) / y_test.size();
	cout << "You got " << (100 * fraction_correct) << " correct" << endl;

	return 0;
}