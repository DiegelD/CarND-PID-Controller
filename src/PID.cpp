#include "PID.h"
#include <iostream>
#include <math.h> 

using namespace std;
/**
 * TODO: Complete the PID class. You may add any additional desired functions.
 */

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_) {
  /**
   * TODO: Initialize PID coefficients (and errors, if needed)
   */
  this->Kp = Kp_;
  this->Ki = Ki_;
  this->Kd = Kd_;

  this->p_error = 0;
  this->i_error = 0;
  this->d_error = 0;

  d_error_cumulative = 0;
  p_error_cumulative = 0;
  i_error_cumulative = 0;

      // Counter Varaiables
      run_counter = 0;
  round_counter = 0;
  rmse_round_accumulator = 0 ;

  // Training and evaluation variables
  EpochCumulativeError_ = 0;
  PreviousEpochError_ = 0;
  CurrentEpochError = 0;
  Training_to_do = true;

  

}

void PID::UpdateError(double cte) {
  /**
   * TODO: Update PID errors based on cte.
   */
  d_error = cte - p_error;
  p_error = cte;
  i_error += cte;
  EpochCumulativeError_ += pow(cte, 2);
  
  d_error_cumulative += d_error;
  p_error_cumulative += p_error;
  i_error_cumulative += i_error;
}

double PID::TotalError() {
  /**
   * TODO: Calculate and return the total error
   */

  return Kp*p_error + Ki*i_error + Kd*d_error;  // TODO: Add your total error calc here!
}

void PID::UpdateEpochError()
{
  d_error_epoch = CurrentEpochError - p_error_epoch;
  p_error_epoch = CurrentEpochError;
  i_error_epoch += CurrentEpochError;

  rmse_round_accumulator += CurrentEpochError;
}

void PID::ResetEpochError()
{
  EpochCumulativeError_ = 0;
  d_error_cumulative = 0;
  p_error_cumulative = 0;
  i_error_cumulative = 0;
}

void PID::Great_Reset()
{
  rmse_round_accumulator = 0;
  round_counter = 0;
}

void PID::Evaluate()
{
  // Stop training once criteria has been met.
  if (Training_to_do)
  {
    CurrentEpochError = sqrt(EpochCumulativeError_ / EpochLength_); // RMSE of the Epoch
    Training_to_do = CurrentEpochError > ErrorThreshold_;
  }
}

void PID::printMessage(std::string message, double value)
{
  std::cout << message << value << std::endl;
}

/**
 * @param Kx: Kp, Ki or Kd
 * @param dx: partial derivative for Kp, Ki or Kd respectively
 * @param dE: total delta error over the whole epoch (previous epoch - current epoch)
 */
void PID::adjust(double &Kx, double dx, double dE)
{
  double partialDKx = Kx * dx * dE * LearnRate_;
  Kx -= partialDKx;
}

double PID::getKp() { return Kp; }
double PID::getKi() { return Ki; }
double PID::getKd() { return Kd; }
double PID::getRMSERA(){return rmse_round_accumulator;}

void PID::BackPropagation()
{
  double deltaError = PreviousEpochError_ - CurrentEpochError;
  PreviousEpochError_ = CurrentEpochError;

  adjust(Kp, p_error_epoch, deltaError);
  adjust(Ki, i_error_epoch , deltaError);
  adjust(Kd, d_error_epoch, deltaError);
}