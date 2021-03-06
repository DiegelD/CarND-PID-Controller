# CarND-Controls-PID

> PID Controller for Self-Driving Car ND

[![Udacity - Self-Driving Car NanoDegree](https://s3.amazonaws.com/udacity-sdc/github/shield-carnd.svg)](http://www.udacity.com/drive)

![PID Controller](https://user-images.githubusercontent.com/4352286/37220909-147b8132-2396-11e8-9e0b-544d357cdb75.png)

## Project Basics

The goal of this project is to implement in C++ a PID controller to control the steering angle for driving a car around a virtual track using the [Udacity simulator](https://github.com/udacity/self-driving-car-sim/releases), and in addition tuning each PID gain in order to calculate a steering angle that keeps the car on the track.

The simulator provides the cross-track error (CTE), which expresses the error between the desired and the current path. The PID (proportional-integral-differential) controller takes this error as input and controls the steering commands to drive the car around the simulator track.

**Project Steps**
- 1) Implement a PID Controller for steering the car
- 2) Optimize the PID parameter for a stable ride around the track
- 3) Fine tune the parameters using a optimization algorithm (Backpropagandation)

**Discussion Steps**
- General brief introduction to PID-Controller
- Fine tune parameters using Backpropagandation of the Error
- Conclusion 
- Further autotune information

---
## 1) Brief introduction PID

PIDs are widely used in the industry for controlling plants that need a continuous modulated control. They are using the mechanism of continues feedback control. So, an error can be calculated as the differences between the desired control point and the current control point. This is used as input for the PID-Controller that applies a correction to this error with its proportional, integral and derivate parts. Seen in the code and image below. 

The steering parameter gets updated as followed, where Kp, Ki, Kd are the Controller Parameters:

`steering_angle = Kp * p_err  + Ki * i_err + Kd * d_err`


<figure>
 <img src="./img/Controller_BP.jpg" width="830" alt="data amout plot" />
 <figcaption>
 <p></p> 
 <p style="text-align: center;"> Fig. 1.1: PID-Controller with Backpropagandation</p> 
 </figcaption>
</figure>
 <p></p>
 
To control the plant, a car in our case, the controller uses following three parameters:

- P-Term: Is proportional to the current difference between desired and current control error.  For example, the further the car is a way from the desired path the bigger the control output will be. However, a P-Controller by itself will always remain an error in the system. Hence it needs an error to get an active control output!

- I-Term: Accumulates the past errors and integrates them over the time. For example, if using a P-Controller with the car a residual error will remain and this error accumulated increases the I -value that ceased to grow if no error is left. This is good to compensate offset errors, that can remain the mechanic of the steering actuator. 

- D-Term: Best estimate of the future error, based on the rate of change. The higher the rapid error change rate of the car is, the greater the controller reaction to it will be.

## 2) PID-Parameter tuning
For tuning a PID-Controller are several state of the art [ approaches ]( https://en.wikipedia.org/wiki/PID_controller ) available.  In this project is a "quick & dirty" manual tuning [approch](https://robotics.stackexchange.com/questions/167/what-are-good-strategies-for-tuning-pid-loops ) used.

Following Parameters are manually identified:
```
Kp = -0.1
Ki = -0.005
kd = -1.5
```

## 3) Fine tuning the parameters using a Backpropagandation as optimization technique

Inspired by this [project](https://github.com/antevis/CarND_T2_P4_PID) the idea rose to modify and improve the Backpropagandation approach. From previous Neuronal Network projects in my repository I got already familiar with Backpropagandation and I also wanted to see how it behavior with a controller. Furthermore because other approaches with a local hill climber, didn???t seen much promising since the deliver not the same optimal values [twiddle1]( https://medium.com/intro-to-artificial-intelligence/pid-controller-udacitys-self-driving-car-nanodegree-c4fd15bdc981) [twiddle2]( https://github.com/gdangelo/CarND-PID-Control-Project). 

### The optimization algorithms:

```
/**
 * @param Kx: Kp, Ki or Kd
 * @param dx: partial derivative for Kp, Ki or Kd respectively
 * @param dE: total delta error over the whole epoch (previous epoch - current epoch)
 */
void PID::adjust(double &Kx, double dx, double dE) {
    double partialDKx = Kx * dx * dE * learnRate_;
    Kx -= partialDKx;
}
```

The optimization update is done after an Epoch of training. The Epochs as chosen in respect of the program runs and are tuned with the parameter 'EpochLength_ =375' . Here is a conflict between specialization and generalization of the update. Means the longer the length the more of the track is considered in the "training". To track the results and the method error is calculated as Root Means Sequard Error (RMSE) over each Epoch and gets accumulated over the hole track. With the parameter 'Runs_in_Track = 1500' the length of the track is saved in the software. *important* Epoch Length have to fit into Runs_in_Track without modulo. 

### Results
The results show an overall improvement of the RMSE. Anyhow the Error is strongly oscillating. A first investigation have given the clarity that 'Runs_in_Track = 1500??? a not exactly covering one track round, so that the track parts are always changing and with this are not 1:1 comparable. Furthermore, a steady increase of the Kd & Kp values can be seen. The changes in the Ki values are so marginal that they can???t be seen in this plot. 

<figure>
 <img src="./img/Outcome.jpg" width="830" alt="data amout plot" />
 <figcaption>
 <p></p> 
 <p style="text-align: center;"> Fig. 3.1: Results</p> 
 </figcaption>
</figure>
 <p></p>
 
 ### Conclusion 
 The attempt looks promising. However, several improvements have to be made before this algorithm can be further developed
 1) Setting the right Track runs
 2) Integrating a sign error into the algorithms. Till know it can actually just go into on direction because just the root mean square errros a taken into count. One approach count be to lower the 'EpochLength_ ' so that a more specify learning could be done in combination with changing the 'p/id_error_epoch' from RMSE to a direct CTE margin and lowering the learning rate ' LearnRate_ = 1e-2', due to the fact that the learning will happened anyhow more frequent.
3) Checking out if the optimization reaches a saturation by track runs x >> 16 (current runs)
 
 Code from the parameter adjustments that should be changed 
 
  ```
 void PID::UpdateEpochError()
{
  d_error_epoch = CurrentEpochError - p_error_epoch;
  p_error_epoch = CurrentEpochError;
  i_error_epoch += CurrentEpochError;

  rmse_round_accumulator += CurrentEpochError;
}
 ```
 ```
void PID::BackPropagation()
{
  double deltaError = PreviousEpochError_ - CurrentEpochError;
  PreviousEpochError_ = CurrentEpochError;

  adjust(Kp, p_error_epoch, deltaError);
  adjust(Ki, i_error_epoch , deltaError);
  adjust(Kd, d_error_epoch, deltaError);
}
````

### Further Information for tuning PID-Controller
Some additional information about possibilities to tune Controller.

#### Machine Leaning-based(Lecture)
[Tuning a PID-Controller with Genetic Algorithms](https://www.youtube.com/watch?v=S5C_z1nVaSg) by Steve Brunton.
Abstract: This lecture shows how to use genetic algorithms to tune the parameters of a PID controller.  Tuning a PID controller with genetic algorithms is not generally recommended, but is used to demonstrate evolutionary control algorithms. 

#### Automatic Controller Tuning(PhD Thesis)
[Automatic Controller Tuning using Relay-based Model Identification](https://portal.research.lu.se/ws/files/33100749/ThesisJosefinBerner.pdf) by Josefin Berner.
Abstract: Proportional-integral-derivative (PID) controllers are very common in the process industry. In a regular factory there may be hundreds or thousands of them in use. Each of these controllers needs to be tuned, and even though the PID controller is simple, tuning the controllers still requires several hours of work and adequate knowledge in order to achieve a desired performance. Because of that, many of the operating PID controllers today are poorly tuned or even running in manual mode. Methods for tuning the controllers in an automated fashion are therefore highly beneficial, and the relay autotuner, that was introduced on the market in the 1980???s, has been listed as one of the great success stories of control.
The technology development since the 1980???s, both concerning PID control and available computing power, gives opportunities for improvements of the autotuner. In this thesis three new autotuners are presented. They are all based on asymmetric relay feedback tests, providing process excitation at the frequency intervals relevant for PID control. One of the proposed autotuners is similar to the classic relay autotuner, but provides low-order models from which the controllers are tuned by simple formulas. The second autotuner uses the data from a very short relay test as input to an optimization method. This method provides more accurate model estimations, but to the cost of more computing. The controller is then tuned by another optimization method, using the estimated model as input. The principle of the third autotuner is similar to the second one, but it is used to tune multivariable PID controllers for interacting processes. In this case a relay feedback experiment is performed on all loops simultaneously, and the data is used to identify the process transfer function matrix. All of the proposed autotuners strive to be user-friendly and practically applicable.
Evaluation of the three autotuning strategies are done both through simulation examples and on experimental processes. The developed autotuners are also com- pared to commercially available ones, and the study shows that an upgrade of the industry standard to the newly available autotuners will yield a significant performance improvement.

#### Reinforcement Learning-based (Paper)
[Reinforcement Learning and Deep Learning based Lateral Control for Autonomous](https://arxiv.org/abs/1810.12778) Driving by D. Li, et. al.
Abstract: This paper investigates the vision-based autonomous driving with deep learning and reinforcement learning methods. Different from the end-to-end learning method, our method breaks the vision-based lateral control system down into a perception module and a control module. The perception module which is based on a multi-task learning neural network first takes a driver-view image as its input and predicts the track features. The control module which is based on reinforcement learning then makes a control decision based on these features. In order to improve the data efficiency, we propose visual TORCS (VTORCS), a deep reinforcement learning environment which is based on the open racing car simulator (TORCS). By means of the provided functions, one can train an agent with the input of an image or various physical sensor measurement, or evaluate the perception algorithm on this simulator. The trained reinforcement learning controller outperforms the linear quadratic regulator (LQR) controller and model predictive control (MPC) controller on different tracks. The experiments demonstrate that the perception module shows promising performance and the controller is capable of controlling the vehicle drive well along the track center with visual input.

---


## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1(mac, linux), 3.81(Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
  
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

Fellow students have put together a guide to Windows set-up for the project [here](https://s3-us-west-1.amazonaws.com/udacity-selfdrivingcar/files/Kidnapped_Vehicle_Windows_Setup.pdf) if the environment you have set up for the Sensor Fusion projects does not work for this project. There's also an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3).

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid -0.11 -0.0005 -1.80`  

Tips for setting up your environment can be found [here](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/0949fca6-b379-42af-a919-ee50aa304e6a/lessons/f758c44c-5e40-4e01-93b5-1a82aa4e044f/concepts/23d376c7-0195-4276-bdf0-e02f1f3c665d)

## Editor Settings

We've purposefully kept editor configuration files out of this repo in order to
keep it as simple and environment agnostic as possible. However, we recommend
using the following settings:

* indent using spaces
* set tab width to 2 spaces (keeps the matrices in source code aligned)

## Code Style

Please (do your best to) stick to [Google's C++ style guide](https://google.github.io/styleguide/cppguide.html).

## Project Instructions and Rubric

Note: regardless of the changes you make, your project must be buildable using
cmake and make!

More information is only accessible by people who are already enrolled in Term 2
of CarND. If you are enrolled, see [the project page](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/f1820894-8322-4bb3-81aa-b26b3c6dcbaf/lessons/e8235395-22dd-4b87-88e0-d108c5e5bbf4/concepts/6a4d8d42-6a04-4aa6-b284-1697c0fd6562)
for instructions and the project rubric.

## Hints!

* You don't have to follow this directory structure, but if you do, your work
  will span all of the .cpp files here. Keep an eye out for TODOs.

## Call for IDE Profiles Pull Requests

Help your fellow students!

We decided to create Makefiles with cmake to keep this project as platform
agnostic as possible. Similarly, we omitted IDE profiles in order to we ensure
that students don't feel pressured to use one IDE or another.

However! I'd love to help people get up and running with their IDEs of choice.
If you've created a profile for an IDE that you think other students would
appreciate, we'd love to have you add the requisite profile files and
instructions to ide_profiles/. For example if you wanted to add a VS Code
profile, you'd add:

* /ide_profiles/vscode/.vscode
* /ide_profiles/vscode/README.md

The README should explain what the profile does, how to take advantage of it,
and how to install it.

Frankly, I've never been involved in a project with multiple IDE profiles
before. I believe the best way to handle this would be to keep them out of the
repo root to avoid clutter. My expectation is that most profiles will include
instructions to copy files to a new location to get picked up by the IDE, but
that's just a guess.

One last note here: regardless of the IDE used, every submitted project must
still be compilable with cmake and make./

## How to write a README
A well written README file can enhance your project and portfolio.  Develop your abilities to create professional README files by completing [this free course](https://www.udacity.com/course/writing-readmes--ud777).

