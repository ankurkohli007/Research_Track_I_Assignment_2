#include "ros/ros.h" //this header will includes all the headers which are necessary to use in the most common public pieces of the ROS system. 
#include "geometry_msgs/Twist.h" //header added which is the related to the velocity message
#include "turtlesim/Spawn.h"
#include "sensor_msgs/LaserScan.h" //this header will include the Laser Scan necessary headers for the system
#include "second_assignment/Velocity.h" //header for the service message
#include "second_assignment/Vel.h" //header for the custom message
#include "std_srvs/Empty.h" //header for service such as reset position
#include "stdio.h" //header file which includes necessary information to include the input/output related functions in our program
#include <sstream> //this header file which provides templates & types that enable interoperation between stream buffers and string objects
#include <iostream> //this header file declares a set of functions for standard Input/Output




using namespace std; //it means we uses the namespace named std(standard), It means we use all the things with in “std” namespace
using namespace ros; //It means we use all the things with in “ros” namespace





// Initiaslization of the publisher needed to send the velocity values through the
// /cmd_vel topic to the simulation node.
ros::Publisher pub; //definition of publisher as global (pub as global because it also uses in the CallBack (subscriber)); also publish the velocity to the robot



// Twist message object for assigning the robot's speed.
geometry_msgs::Twist my_vel; //defining the type of message (standard message)

// Initialization of the coefficient added to the linear velocity param in order to encrise the total speed's value.
float velocity = 0.0;

// Initialization of the distance threashold variables for the managing of the robot's speed.
float distance_th = 1.5;

// Initialization of the integer variable count needed for makeing the robot decellerating a bit when exiting turns.
int counter = 0;

// Initializatioon of the multiplier for regulating the anguar velocity.
float angular_multiplier = 0.25;

// Initializatioon of the multiplier for regulating the linear velocity when getting out of turns.
float linear_multiplier = 0.5;

// Function to retrive the lower value among a given span of the laser scan set.
float minimum (float laser_array[720], int init = 0, int capacity_laser = 720)
{


	float value = 40.0;
	int j = 0;
	for(j= init; j < init+capacity_laser; j++)
	{
	
		if (value >= laser_array[j])
		{
			value = laser_array[j];
		}
	}
	return value;
}


// autopilotCallBack needed by the node to costantly retrieve data about the laser-scans.
// Subsctiption to the /base_scan  topic which contains the updated data about what the laser-scanners see.
void autopilotCallBack(const sensor_msgs::LaserScan::ConstPtr& msg) //CallBack() is defined in which sensor_msgs::LaserScan is the specific message that going to be see; and also subscriber for a robot 
{
 
        // Assigning all the retrived values to a different array in order to exploit the collected data.
		float laser[721];
		int i = 0;
		for(i = 0; i < 721; i++)
		{
		laser[i]=msg->ranges[i];
		}
		
        // If the closest distance detected by the lasers'set in fornt of the robot is higher than 1.5 units the robot will
        // travel straight.
		if (minimum(laser, 310, 100) > 1.5)
		{
		
			// When the count gets to zero the normal velocity will take place again.
			if ((counter--) <= 0)
			{
			
								
				cout<<"Linear Velocity: "<<1.0+velocity;
				cout<<"Angular Velocity: "<<0.0;
				
				my_vel.linear.x = 1.0+velocity;
				my_vel.angular.z = 0.0;
			}
			
			else
			{
				cout<<"Linear Velocity"<<1.0+(velocity*linear_multiplier);
				cout<<"Angular Velocity: "<<0.0;
				
				
				my_vel.linear.x = 1.0+(velocity*linear_multiplier);
				my_vel.angular.z = 0.0;
			
			}
			
		} 
		
    // If the closest distance detected by the set of lasers in fornt of the
    // robot is lower than 1.5 units:
		else
		{
            
            // the robot will check the sides and it will turn towards the opposit
            // direction of the closest wall.
            // Following this rule, the robot will avoid hitting the wall.
			
		// If the closest wall detected is on the RIGHT SIDE.
			if (minimum(laser,0,100) < minimum(laser,620,100))
			{
				cout<<"Linear Velocity"<<0.2;
				cout<<"Angular Velocity: "<<1+(velocity*angular_multiplier);
				
				my_vel.linear.x = 0.2;
				my_vel.angular.z = 1+(velocity*angular_multiplier); 
			}
			
		// If the closest wall detected is on the LEFT SIDE.
			else 
			{
				cout<<"Linear Velocity"<<0.2;
				cout<<"Angular Velocity: "<<(-(1+(velocity*angular_multiplier)));
				
				my_vel.linear.x = 0.2;
				my_vel.angular.z = -(1+(velocity*angular_multiplier)); 
			}
			
			// Setting the count to 5 in order to set a slower velovity when the robot comes out of turns.
			counter = 5;			
		}
				 
		 //int x1 = 300, x2 = 360, x3 = 180, x4 = 420, x5 = 0, x6 = 540; //variables declared which are used to set the initial value (x1, x2, x3, x4, x5, x6) of each range  
/*int x1 = 200, x2 = 260, x3 = 80, x4 = 320, x5 = 0, x6 = 440; 
double nrst_o1, nrst_o2, nrst_o3, nrst_o4, nrst_o5, nrst_o6; //variables declared to find the nearest obstacle (nrst_o) in the range of robot path  

/*nrst_o1 = msg->ranges[0];
nrst_o2 = msg->ranges[60];
nrst_o3 = msg->ranges[120];
nrst_o4 = msg->ranges[180];
nrst_o5 = msg->ranges[240];
nrst_o6 = msg->ranges[300];

nrst_o1 = msg->ranges[200];
nrst_o2 = msg->ranges[260];
nrst_o3 = msg->ranges[80];
nrst_o4 = msg->ranges[320];
nrst_o5 = msg->ranges[0];
nrst_o6 = msg->ranges[440];

for(x1 = 200; x1 < 260; x1++) //for loop initiated to check the nearest obstacle in the range of robot path //for RIGHT
{
	if(msg->ranges[x1] < nrst_o1) //comparing the initial value with the nearest obstacle value
	{    
            nrst_o1 = msg->ranges[x1];
        }
        
        //left[x1]; 
} //end of for loop

for(x2 = 260; x2 < 320; x2++) //for loop again initiated to check again the nearest obstacle in the range of robot path //for FRONT
{
	if(msg->ranges[x2] < nrst_o2) //again comparing the initial value with the nearest obstacle value
	{
	    nrst_o2 = msg->ranges[x2];
        }
        
       // centre[x2];
} //end of for loop

for(x3 = 80; x3 < 200; x3++) //for loop again initiated for the third to check again the nearest obstacle in the range of robot path //for LEFT
{
	if(msg->ranges[x3] < nrst_o3) //again comparing the third initial value with the nearest obstacle value
        {

            nrst_o3 = msg->ranges[x3];
        }
        
        //right[x3]; 
} //end of for loop


// considering only first three ranges 


for(x4 = 320; x4 < 440; x4++) //for loop again initiated to check another the nearest obstacle in the range of robot path
{
	if(msg->ranges[x4] < nrst_o4) //comparing the another initial value with the nearest obstacle value
        {

            nrst_o4 = msg->ranges[x4];

        } 
} //end of for loop

for(x5 =0; x5 < 80; x5++) //for loop again initiated to check another the nearest obstacle in the range of robot path
{
	if(msg->ranges[x5] < nrst_o5) //comparing the another initial value with the nearest obstacle value
        {
            nrst_o5 = msg->ranges[x5];
        } 

}*/ //end of for loop
		 
		 
		// Pubblishing the velocity values
		pub.publish(my_vel);


}

// CallBack function to update the velocity factor.
void velocityCallBack(const second_assignment::Vel::ConstPtr& msg1)
{
    // Assigning the updated accelleration value to the acc global variable.
	velocity = msg1->a;
    
    // Returnung to the shell the value the newly-updated velocity coefficient.
    ROS_INFO("[%f]", velocity + 1.0);
}


int main (int argc, char **argv) //initiating main(); here "argc" means argument count, whereas "argv" means argument values; these variables are passed to the main() when it starts executing
{
	// Initialize the node, setup the NodeHandle for handling the communication with the ROS system.
	
	/* here ros::init() function needs to monitor "argc" & "argv" so that it can perform on any ROS arguments & name remapping that were provided at the command line. For programmatic remappings we can use a different version of init() which takes remappings directly, but for most command-line programs, passing "argc" & "argv" is the easiest way to do it.  The third argument to init() is the name of the node. You must call one of the versions of ros::init() before using any other part of the ROS system. */ 
	
	ros::init(argc, argv, "robot_controller"); //initialized the node, setup the NodeHandle for handling the communication with the ROS system
	ros::NodeHandle nh; //here NodeHandle is the main access point to communications with the ROS system. The first NodeHandle constructed will fully initialize this node. Lastly, NodeHandle destructed will close down the node.
    
    	// Initialize the two subscribers for both the call backs.
	ros::Subscriber sub = nh.subscribe("/base_scan", 1000, autopilotCallBack); //declares subscriber; created subscriber "base_scan" using the "scanner"; The topic name is "base_scan" & the size of the publisher queue is set to 1000.
	ros::Subscriber sub1 = nh.subscribe("/Vel", 1000, velocityCallBack); //declares subscriber; created subscriber "velocity" using the "topic"; The topic name is "velocity" & the size of the publisher queue is set to 1000.
	
    	// Initialize the publisher needed to publish the new velocity values to update the robot's speed.
	pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1000); //nh = node handles; geometry_msgs = is the type of message specified which is going to be publish; "/cmd_vel" = name of the topic & 1000 is the buffer size; in the main() initialzed the publisher; publisher cannot be initialized outside the main() because in C++ we need first init of ROS
    
    	// Function to loop the callback functions.
	ros::spin(); //a function for calling a callback function, waiting for a message to be received, executing a callback function when it is received, & blocks until ROS invokes a shutdown
	
	return 0;
}
