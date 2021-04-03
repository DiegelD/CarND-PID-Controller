#ifndef PID_H
#define PID_H

#include <string>
using namespace std;

class PID {
 public:
   /**
   * Constructor
   */
   PID();

   /**
   * Destructor.
   */
   virtual ~PID();

   /**
   * Initialize PID.
   * @param (Kp_, Ki_, Kd_) The initial PID coefficients
   */
   void Init(double Kp_, double Ki_, double Kd_);

   /**
   * Update the PID error variables given cross track error.
   * @param cte The current cross track error
   */
   void UpdateError(double cte);

   /**
     * Increments epoch error
     * 
     */
   void UpdateEpochError();

   /**
   * Calculate the total PID error.
   * @output The total PID error
   */
   double TotalError();

   /**
     * 
     */
   void Evaluate();

   /**
     * 
     * 
     */
   void BackPropagation();

   /**
     * Rest the Error of the acumulated Epoch Error 
     */
   void ResetEpochError();

   /**
   * Reset nesecary Parameter, hence one track round is driven.
   * 
   */
   void Great_Reset();

   /**
     *  acces to private variables
     */
   double getKp();
   double getKi();
   double getKd();
   double getRMSERA();

   /**
     * training parameters
     */
   int run_counter;
   int round_counter; // runs in one round of the track, needed for regonizing one round
   bool Training_to_do;
   double CurrentEpochError;
   const int EpochLength_ = 375;  // The bigger the Epoch is the biiger the generalization is
   const int Runs_in_Track = 1500; // Programm rund needed for one track round

 private:
  /**
   * PID Errors
   */
  double p_error;
  double i_error;
  double d_error;
  double d_error_cumulative;  // controller error accumulated over epoch
  double p_error_cumulative;
  double i_error_cumulative;
  double p_error_epoch;
  double i_error_epoch;
  double d_error_epoch;
  double rmse_round_accumulator; // accumulates the rmse up till one round in the simulator is reached

  /**
   * PID Coefficients
   */
  double Kp;
  double Ki;
  double Kd;

  /**
   * training parameters
   */
  double EpochCumulativeError_; //Accumulates squares of CTEs to further compute RMSE
  double PreviousEpochError_;
  const double ErrorThreshold_ = 5e-2;
  const double LearnRate_ = 1e-2;

  /**
     * @param Kx: Kp, Ki or Kd
     * @param dx: partial derivative for Kp, Ki or Kd respectively
     * @param dE: total delta error over the whole epoch (prev. epoch - current epoch)
     */
  void adjust(double &Kx, double dx, double dE);

  void printMessage(std::string message, double value);
};

#endif  // PID_H