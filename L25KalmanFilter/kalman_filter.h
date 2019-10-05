#ifndef KALMAN_FILTER_H_
#define KALMAN_FILTER_H_
#include "Eigen/Dense"

using Eigen::MatrixXd;
using Eigen::VectorXd;

class KalmanFilter
{
public:
    KalmanFilter();
    virtual ~KalmanFilter();

    /*
     * Predict the state and the state covariance
     * using the process model
     */
    void Predict();

    /*
     * Updates the state
     * @param z The measurement at k+1
     */
    void Update(const VectorXd &z);

    VectorXd x_; // state vector
    MatrixXd P_; // state covariance matrix
    MatrixXd F_; // state transition matrix
    MatrixXd Q_; // process covariance matrix
    MatrixXd H_; // measurement matrix
    MatrixXd R_; // measurement covariance matrix
};

#endif // KALMAN_FILTER_H_
