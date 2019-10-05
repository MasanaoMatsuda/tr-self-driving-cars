#include <iostream>
#include <vector>
#include "Eigen/Dense"
#include "measurement_package.h"
#include "tracking.h"

using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::istringstream;
using std::vector;

int main()
{
    vector<MeasurementPackage> measurement_pack_list;

    string in_file_name = "./data/obj_pose-laser-radar-synthetic-input.txt";
    ifstream in_file(in_file_name.c_str(), ifstream::in);

    if (!in_file.is_open())
    {
        cout << "Cannot open input file: " << in_file_name << endl;
    }

    string line;
    int i = 0;
    while(getline(in_file, line) && (i <= 3))
    {
        MeasurementPackage meas_package;
        istringstream iss(line);
        string sensor_type;
        iss >> sensor_type;
        int64_t timestamp;
        if (sensor_type.compare("L") == 0)
        {
            meas_package.sensor_type_ = MeasurementPackage::LASER;
            meas_package.raw_measurements_ = VectorXd(2);
            float x;
            float y;
            iss >> x;
            iss >> y;
            meas_package.raw_measurements_ << x,y;
            iss >> timestamp;
            meas_package.timestamp_ = timestamp;
            measurement_pack_list.push_back(meas_package);
        }
        else if (sensor_type.compare("R") == 0)
        {
            // Skip Radar measurements
            continue;
        }
        ++i;
    }

    Tracking tracking;
    size_t N = measurement_pack_list.size();
    for(size_t k = 0; k < N; ++k)
    {
        tracking.ProcessMeasurement(measurement_pack_list[k]);
    }

    if (in_file.is_open())
    {
        in_file.close();
    }

    return 0;
}
