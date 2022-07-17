[Research Track I](https://corsi.unige.it/en/off.f/2021/ins/51201)<br>
**Programmer:** [Ankur Kohli](https://github.com/ankurkohli007)<br>
[M.Sc Robotics Engineering](https://corsi.unige.it/corsi/10635)<br>
[University of Genoa (uniGe)](https://unige.it/en)<br>
**Supervisor:** [Prof. Carmine Tommaso Recchiuto](https://rubrica.unige.it/personale/UkNDWV1r)

# Assignment 2: Monza Raccing Circuit

## Abstract
This assignment analyzes how to control the robot using [ROS](https://www.ros.org/) (Robot Operating System). The robot is endowed with laser scanners. There were different nodes were design to control the robot. Also, one additional node which interacts with the user to increase/decrease the speeed of the robot and reset the position of the robot. For this, ***cpp*** programing is used. 

## Introduction
In this assignment, a controller that runs on ROS. A simple robot which runs autonomously on a track which is basically the reproduction of the [Monza Racing Circuit](https://www.monzanet.it/en/). The robot runs on a track and alos avoides the collision with the walls. Lastly, user interface node is designed which gives access to the users to operate robot's and also change it's behaviour such as increase/decrease of the speed, resetting the robots position.

## Objective
This assignment objective to create a ROS Package which have the ability of designing a robot equipped with the laser scanners, a lap around a given circuit. Moreover, package conists of the cpp sources needed for the interaction with the robot. The world where the robot has to run autonomously is a simple 3D model of the actual Monza's F1 circuit. ROS is a group os tools and libraries for autonomous, robust, efficient, and secure mobile robot navigation, which uses sensors to sense the surroundings and computer vision techniques as the navigation method.

The robot behaviour has to fullfill the following criteria while running on the circuit:

* Continously moving around the world in the **clockwise direction**
* Speed can **increase** or **decrease** according to user choice
* Position of the robot can be **reset** any time when user wish to do so.

The user can easily control the robot operations by using **Keyboard Keys**:

* By giving an input as `l/L`, this will **ACCELERATE** the robot's speed by **half unit** at every input from the keyboard by user end.
* * By giving an input as `j/J`, this will **DECELERATE** the robot's speed by **half unit** at every input from the keyboard by user end.
* * By giving an input as `r/R`, this will **RESET** the robot's position at every input from the keyboard by user end.
* By giving an input as `q/Q`, thi will **QUIT** the session. 

## World
The circuit as shown in figure below is the **WORLD** where robot runs autonomously and perform it's operations as per user inputs. The robot is represented as a blue point which is equipped with laser scanner. The light blue area is the area scanned by the laser sensors on the robot.

![alt text](Results_Worlds_Images/world.png) 

The ```world``` node publishes messages on the ```/base_scan``` topic. Messages are ```LaserScan``` type, from ```sensor_msg``` package, and they provide the result of a single scan by the robot's laser sensors.

It also subscribes to the topic ```/cmd_vel``` to impose a linear and angular velocity to the robot along the three axis. It receives ```Twist``` type messages from ```geometry_msgs``` package.

## Intalling & Running 

###### Installation

For this assignment, simulation requires [ROS Noetic](https://wiki.ros.org/noetic/Installation). After installing ROS Noetic on your system clone the [Prof. Carmine Tommaso Recchiuto](https://github.com/CarmineD8/second_assignment). this repository contains the main workspace for running the simulation world. The source includes the *CmakeList* for compiling the node and the *World* file for building the scene.

The package developed during the assignmnet are as belows: 

* An *autopilot_node*, this node will initiates the movement of the robot from the initial position in clockwise direction. Also, aims to guide the robot to avoid collision into the walls of the circuit.
* A *host_node*, this node is the Server node which handles service to manage request from the user and give the responce to the user node. Lastly, this node will process the Keyboard input and change the robot's behaviour such as increase/decrease the speed and resets the position of the robot. 
* A *user_interface_node*, This node will takes input from the user and send the request to the server node and waiting for the response from the server node. Finally, this node will give access to the user and user can interacts with the simulation with the Keyboard inputs and controls the robot's operations. 
* ONE *custom service* ```Velocity.srv``` needed by the server node to exchange data between the *request* and the *response*.
* ONE *custom message* ```Vel.msg``` needed to pass the acceleration factor from the service to the controller through the use of a topic.

###### Running the program

* After cloning the repository from the aforementioned *GitHub* link, copy the `second_assignment` folder which is included in the cloned repository and paste in the src folder which is inside local ROS workspace directory or else you can directly clone the aforementioned repository in your src folder of the local ROS workspace. 
* Now, back to your ROS worksapce folder, run the command ```catkin_make```. This command will build all the scripts in the package.
* After building, run rospack profile in order to refresh or update your packages. 
```
rospack profile
```
* Now, follow the steps given below to run the simulation:

**Step 1:** Open a terminal, and run the master node in terminal to run the ROS in your PC by the following command: 
```
roscore&
```
**Step 2:** After execution of master node, in the same terminal run the **world simulation** by the command to launch the robot in the given world. 
```
rosrun stage_ros stageros $(rospack find second_assignment)/world/my_world.world
```
**Step 3:** Now, open the another terminal window and execute the command for `robot controller node`. This node will launch the robot autonomously and avoiding the collision with the wall.
```
rosrun second_assignment autopilot_controller_node
```
**Step 4:** After that, open another terminal window and run the command for `server node`. This node tackles the services to manage request from the user end and give response to the user node.
```
rosrun second_assignment host_node
```
**Step 5:** Finally, for the last time open new terminal window and the execute the command for `user interface node`. This node will receive the inputs from the user end and send request to the `host_node` and waits for the response from the same node.
```
rosrun second_assignment user_interface_node
```
## NODES Descriptions & their logics

###### Simulation Node: StageRos node (stage_ros package)

The stageros node wraps the Stage 2-D multi-robot simulator, via libstage and this node is used for the simulation is the standard node `StageRos`. Stage simulates a world as defined in a .world file. This file tells stage everything about the world, from obstacles (usually represented via a bitmap to be used as a kind of background), to robots and other objects.

The **robot controller node** works with the **stageros node**, which should also be running. The communication with the aforementioned node occurs through **topics**, to which the controller subscribes. In particular:
1 - "/base_scan" from which sensor data is acquired (related to obstacles)
2 - "/cmd_vel" through which the robot can be commanded with ROS geometry_msgs::Twist messages

Also, this node only exposes a subset of Stage's functionality via ROS. Specifically, it finds the Stage models of type laser, camera and position, and maps these models to the ROS topics given below. If at least one laser/camera and position model are not found, stageros exits.

Stageros only exposes models created by a subset of the `.world` file syntax, specifically `laser`, `position` and `camera` models.

* Parameters:
    * world : The Stage .world file to load.
    * -g : If set, this option will run stage as "headless," no GUI will be displayed.
* Subscribers:
    * The node **subscribes** to the `cmd_vel (geometry_msgs/Twist)` topic, to manage the velocity of the robot.
* Publishers: The node **publishes** to the following topics:
    * `odom (nav_msgs/Odometry)`: odometry data of the model. 
    * `base_scan (sensor_msgs/LaserScan)`: __scans from the laser model. We will use this one.
    * `base_pose_ground_truth (nav_msgs/Odometry)`: __the ground truth pose. We will use this one.
    * `image (sensor_msgs/Image)`: visual camera image.
    * `depth (sensor_msgs/Image)`: depth camera image.
    * `camera_info (sensor_msgs/CameraInfo)`: camera calibration info.
* Usage:
```
rosrun stage_ros stageros [-g runs headless] <world> [standard ROS args]
```
***Nodes Topic StageRos RQT Graph*** 
![alt text](Results_Worlds_Images/nodestopicrqtstageros.png)

***Nodes Topic Active StageRos RQT Graph*** 
![alt text](Results_Worlds_Images/nodestopicactiverqtstageros.png.png)

###### Robot Controller Node: autopilot_controller_node

Robot Controller Node will drives the robot autonomously. Also, while moving inside a circuit robot's have the ability to avoid the collision among the walls. t will command the robot to drive at a certain velocity inside the track. In this node, robot will also be able to scan the walls of the circuit and not hit them. The robot subscribes and publishes to the following topics: 

* The node **publishes** to the `cmd_vel geometry_msgs/Twist` Topic to change the *linear* and *angular* velocity values related to the robot.

* The node **subscribes** to the following topics:

    * `base_scan (sensor_msgs/LaserScan)`:  which will provide the node with Data about the enviroment surrouding the robot.
    * `Acc (second_assignment/Acc)`:  Which will tell the node how much to increase the *linear* velocity of the robot on the `cmd_vel` Topic.

Here, some inputs and outputs for the laser scanner.

* **INPUTS**:
    * `float laser_array[720]`: The laser range array. 
    * `int init = 0`: The initial index from which the function will start checking for the minimum value.
    * `int capacity_laser = 720`: This is the laserscapacity, also tells how laser function will proceed.

* **OUTPUTS**:
    *  `float value`: The minimum distance registered tin the assigned area.
    
```cpp
float mini ( float arr[720], int ind=0, int size=720)
{


    float value = 40.0;
    
    for(int j= init; i < init+capacity_laser; j++)
    {
    
        if (value >= laser_array[j])
        {
            value = laser_array[j];
        }
    }
    return value;
}
```
Another function called `autopilotCallBack`, this will execute a cycle that will determine what velocity to publish on the `cmd_vel`  topic. 

![alt text](Results_Worlds_Images/autopilotCallBackFlowChart.png)

Figure above shows the process of `autopilotCallBack` function.

After launching of the **Monza Raccing Circuit** and also after launching of the `robot controller node` i.e. named as `autopilot_controller_node`, he robot will move around the circuit at a constant velocity of 1.0 towards its relative x-axis resulting in a forward motion. This speed may vary depending on the accelleration factor given by the user. his float number is stored in the `acc` variable which is added to the initial linear speed. Through the `user_interface_node` the user can either rase or slow down the speed factor by 0.5 notchs.Once the middle detecting area scans a distance shorter than 1.5 units, the robot will stop driving forward and it'll check on the other detecting regions.

![Alt Text](Results_Worlds_Images/robotturningon curves.gif)
























