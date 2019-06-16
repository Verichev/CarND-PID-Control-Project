#include "PID.h"

/**
 * TODO: Complete the PID class. You may add any additional desired functions.
 */

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_) {
  Kp = Kp_;
  Ki = Ki_;
  Kd = Kd_;
}

void PID::UpdateError(double cte) {
  squared_sum_cte += cte * cte;
  d_error = cte - p_error;
  p_error = cte;
  i_error += cte;  
}

double PID::TotalError() {
  return -Kp * p_error - Kd * d_error - Ki * i_error;;
}

double PID::AccumError() {
  return squared_sum_cte;
}
