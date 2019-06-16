# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

---

[![Watch the result video](https://github.com/Verichev/CarND-PID-Control-Project/blob/master/pid_controller.mp4)]


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

## Reflection

PID - controller is responsable for smooth, reactive and safe contoller, that consists of three components:
P - proportional, the component that is proportional to the current CTE. Helps to return back on track.
I - integral, the component that accumulates the CTE error, and incorporates it in the controller, to mitigate the systimatic error, that is included in vehicle itself.
D - derivative, the component, proportional to derivative of CTE, helps to smooth the path, avoid oscillation.
All this components have it's coefficients, that can be adjusted. That is the main goal of this project.

The c++ programm that sends to simulator respective to CTE steering and throttle values reworked to incorporate the PID parameters, that are passed as arguments to the programm.

Also along with implementing PID components in steering value in PID class, I also accumulate all path error, and after the programm finished return it to stdout. 

The application has maximum of cycles that can be used to evaluate the accumulated error. If the vehicle gets stuck in the way. The programm adds the remained cte to the accumulated error, restarts the simulator and close the socket. This is needed to run the programm many times from external process with output of accumulated error.

In root projects used python script with twidder algorithm, which is using different trying to find optimal PID paramters, evaluating the accumulated error through our built C++ programm. To save time it's better to evaluate the error starting from small quantity of cycles, than increasing it, while program is finding pretty good parameters for the current path.

## Results
The results are not so determinant as for more simple simulators, they are dependent on the speed of the vehicle, the time of socket response and the phisics of implementation of the simulator itself, so twiddle cannot find the absolute best solution, but there is a correlation between the optimal parameters and minimal accumulated error. I guess in big project makes sense to to use data provided by twiddle and use regression technics to find the best paramters based on provided data. 
params:  [2.4733010137467275, -0.7290000000000001, 13.417216854917202]
err:  707883.0
params:  [2.4733010137467275, 0.0, 14.308216854917202]
err:  612.704
params:  [2.4733010137467275, 0.0, 12.526216854917202]
err:  472.658
params:  [3.4534010137467277, 0.0, 13.417216854917202]
err:  641155.0
params:  [1.4932010137467273, 0.0, 13.417216854917202]
err:  551.43

For our case, I picked the paramters with minimal evaluated error.

err:  441.756
params:  [2.473301013746727, 0.0, 12.317216854917202]
