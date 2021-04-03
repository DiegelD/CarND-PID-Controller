# CarND-Controls-PID

> PID Controller for Self-Driving Car ND

[![Udacity - Self-Driving Car NanoDegree](https://s3.amazonaws.com/udacity-sdc/github/shield-carnd.svg)](http://www.udacity.com/drive)

![PID Controller](https://user-images.githubusercontent.com/4352286/37220909-147b8132-2396-11e8-9e0b-544d357cdb75.png)

## Project Basics

The goal of this project is to implement in C++ a PID controller to control the steering angle for driving a car around a virtual track using the [Udacity simulator](https://github.com/udacity/self-driving-car-sim/releases), as well as tuning each PID gain in order to calculate a steering angle that keeps the car on the track.

The simulator provides cross-track error (CTE), that is displaying the error between the desired and the current path, via websocket. The PID (proportional-integral-differential) controller takes this error as input input and controles the steering commands to drive the car around the simulator track.

**Project Steps**
- 1) Implement a PID Controller for steering the car
- 2) Optimize the PID parameter for a stable ride adround the track
- 3)  Fine tune the parmeters using a optimization

**Discussion Steps**
- General brief introduction to PID-Controlers
- Fine tune parameters using Backpropagandation of the Error
- Conclusion 

---
## 1) Brief intoduction PID

PIDs are widly used in the industry for controling plants that needs a continious modulted control. They are using the mechanism of contious feedback control for this. So a error can be calculated as the differencs between between the desired control point and the current control point. This is used as input for the PID-Control that applies a correction to this with its porportional, integral and derivate parts. Seen in the Imge below. 

<figure>
 <img scr="./img/Controller_BP.jpg" width="830" alt="data amout plot" />
 <figcaption>
 <p></p> 
 <p style="text-align: center;"> Fig. 1.1: PID-Controller with Backpropagandation</p> 
 </figcaption>
</figure>
 <p></p>
 
To cotrol the plant, car in our case, the controller uses following three parameters:

- P-Term: Is proportional to the current difference between desired and current control error value. For exampel the further the car is a way from the desired path the bigger the control output will be. However a P-Controller by itselfe will always a error in the system remain. Hence it need a error to get an active control output!

- I-Term: Accumulates the past errors and intregrates them over the time. For example is using a P-Controler with the car a residual error will remain and with leds to a increacing I -value that ceased to grow if no error is left. This is good to compensate Offset error, that can remain the the meachanic of the steering for example. 

- D-Term: Best estimate of the future error, based on the rate of change. The hugher the rapid error change rate of the car is, the greater the controler reaction to it will be.

## 2) PID-Parameter tuning
For tuning a PID-Controler are several state of the art [ approaches ]( https://en.wikipedia.org/wiki/PID_controller ) availabe.  In this project is a "qick & dirty" manual tuning [approch](https://robotics.stackexchange.com/questions/167/what-are-good-strategies-for-tuning-pid-loops ) used.

Folowing Parameters are identifyed  :
```
Kp = -0.1
Ki = -0.005
kd = -1.5
```

## 3) Fine tuning the parameters using a optimization technique

Inspired by this [project](https://github.com/antevis/CarND_T2_P4_PID) the idea rose to modify and improve the Backpropagandation approach. From previos Neuronal Network project i already was more or les familar with this Backpropagandation and also wanted to see how it behavios with a controler. Also because other approcahse with a local hill climber, didnt seen much prominisng to earn new paraemters and to modify the aprroch came fromn the backround that also in the inspired project no big improve ments could be made. 

### The optimization algorthmus:

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

The optimization update is done after an Epoch of training. The Eopochs as choosen in respect of the programm runs and are tuned with the parameter 'EpochLength_ =375' . Here is a conflict between specialization and genreralzization of the update. Means the longer the length the more of the track is considered in the "training". To track the results and the mathod Error is calucalted as Root Means Sequar Error over each Epoch and gets accumulated over the hole track. The Programm runs to rach a full track run is saved here 'Runs_in_Track = 1500'. *Importend* EpochLength have to fit into Runs_in_Track without modulo. 

In the end it updates the steering:

`steering_angle = Kp * p_err  + Ki * i_err + Kd * d_err`

### Results
The results show an over all improvement of the RMSE. Anyhow the Error is strongly ozilating. Probably because the 'Runs_in_Track = 1500'  a not excatly covering one trakc round, so that the track parts are always changing and with it are not 1:1 compareable. Furthermore a steady increase of the Kd & Kp valuse can bee seen. The changes in the Ki values are so marignal that they cant be seen in this plot. 

<figure>
 <img scr="./img/Outcome.jpg" width="830" alt="data amout plot" />
 <figcaption>
 <p></p> 
 <p style="text-align: center;"> Fig. 3.1: Results</p> 
 </figcaption>
</figure>
 <p></p>
 
 ### Conlusion 
 The attamp looks promising. However several impovementrs have to be made before this algrithm can be further developed
 1) Setting the right Track runs
 2) Integreating a sign error into the algorthimus. Till know it can actually just go into on direction because just the root mean square errros a taken into count. One approch count be to lower the 'EpochLength_ ' so that a more specifiy learning could be done in combination with changing the 'p/id_error_epoch' from RMSE to a direct CTE margin and lowering the learning rate ' LearnRate_ = 1e-2', due to the fact that the learning will happend anyhow more frequent.
 
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
```

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
4. Run it: `./pid`. 

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

