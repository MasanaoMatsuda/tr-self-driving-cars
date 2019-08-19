#include "tracking.h"
#include <iostream>

using std::cout;
using std::endl;
using std::vector;

Tracking::Tracking()
{
    is_initialized_ = false;
    previous_timestamp_ = 0;
    kf_.x_ = VectorXd(4);
    kf_.P_ = MatrixXd(4,4);
    kf_.P_ << 1,0,0,0,
              0,1,0,0,
              0,0,1000,0,
              0,0,0,1000;
    kf_.R_ = MatrixXd(2,2);
    kf_.R_ << 0.0225, 0,
              0, 0.0225;
    kf_.H_ = MatrixXd(2,4);
    kf_.H_ << 1,0,0,0,
              0,1,0,0;
    kf_.F_ = MatrixXd(4,4);
    kf_.F_ << 1,0,1,0,
              0,1,0,1,
              0,0,1,0,
              0,0,0,1;
    noise_ax = 5;
    noise_ay = 5;
}

Tracking::~Tracking()
{
}

void Tracking::ProcessMeasurement(const MeasurementPackage &measurement_pack)
{
    if (!is_initialized_)
    {
        kf_.x_ << measurement_pack.raw_measurements_[0],
                  measurement_pack.raw_measurements_[1],
                  0,
                  0;
        previous_timestamp_ = measurement_pack.timestamp_;
        is_initialized_ = true;
        return;
    }

    float dt = (previous_timestamp_ - measurement_pack.timestamp_) / 1000000.0;
    float dt_2 = dt * dt;
    float dt_3 = dt_2 * dt;
    float dt_4 = dt_3 * dt;
    previous_timestamp_ = measurement_pack.timestamp_;

    kf_.F_(0,2) = dt;
    kf_.F_(1,3) = dt;
    kf_.Q_ = MatrixXd(4,4);
    kf_.Q_ << dt_4/4*noise_ax, 0, dt_3/2*noise_ax, 0,
              0, dt_4/4*noise_ay, 0, dt_3/2*noise_ay,
              dt_3/2*noise_ax, 0, dt_2*noise_ax, 0,
              0, dt_3/2*noise_ay, 0, dt_2*noise_ay;

    kf_.Predict();
    kf_.Update(measurement_pack.raw_measurements_);

    cout << "x_ = " << kf_.x_ << endl;
    cout << "P_ = " << kf_.P_ << endl;
}
