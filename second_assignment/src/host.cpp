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
// Initializing an object Empty; declaring the server which will be needed to call the reset function.
std_srvs::Empty reset_server;

float speed = 0.0;
	
//Declaring bool function of the server i.e. monitor(), which will be used as a function dedicated to change velocity of the autopilotrobot side by side interacting with user_interface_node.

bool monitor(second_assignment::Velocity::Request &req, second_assignment::Velocity::Response &res)
{
       cout << req.input << endl;
	// If user give input service as 'j', it means velocity of the autopilot robot should increases 5.0 factor which will be added to the velocity global variable (vloct)
	
	if(req.input == 'j' || req.input == 'J')
	{
		speed = speed + 5.0;
		
	}

	// If user give input service as 'l', it means velocity of the autopilot robot should increases 5.0 factor which will be added to the velocity global variable (vloct)
	
	if(req.input == 'l' || req.input == 'L')
	{
		speed = speed - 5.0;
		
	}

	// If user give input service as 'r' or 'R', the position of the autopilotrobot will be reset to the initial position.

	if(req.input == 'r' || req.input == 'R')
	{
		ros::service::call("/reset_positions", reset_server);
	}

	
	// We put in the response the value.

	res.output = speed;   
	return true;
	
	
	/*switch(req.input)
	{
	
		case 72:
		
	// If user give input service as '↑', it will read as 72 ASCII value which means velocity of the autopilot robot should increases 1.0 factor which will be added to the velocity global variable (vloct)
			speed = speed + 5.0;
			
		
		break;
	
		case 80:
	
	// If user give input service as '↓', it will read as 80 ASCII value which means velocity of the autopilot robot should decreases 1.0 factor which will be subtracted to the velocity global variable (vloct)
			speed = speed - 5.0;
			
		
		break;
	
		case 'r':
		case 'R':
		
		// If user give input service as 'r' or 'R', the position of the autopilotrobot will be reset to the initial position.
			ros::service::call("/reset_positions", reset_server);
		
		break;
	}
	
	// Used g as input in order not to modify the value of the velocity.

	
	// Stored in the response value.

	res.output = speed;   
	return true;*/	
} //end of monitor()

int main(int argc, char **argv)//initiating main(); here "argc" means argument count, whereas "argv" means argument values; these variables are passed to the main() when it starts executing
{

	// Initializing the node, setting up the NodeHandle for handling the communication with the ROS system.
	
	/* here ros::init() function needs to monitor "argc" & "argv" so that it can perform on any ROS arguments & name remapping that were provided at the command line. For programmatic remappings we can use a different version of init() which takes remappings directly, but for most command-line programs, passing "argc" & "argv" is the easiest way to do it.  The third argument to init() is the name of the node. You must call one of the versions of ros::init() before using any other part of the ROS system. */

	ros::init(argc, argv, "server"); //initialized the node, setup the NodeHandle for handling the communication with the ROS system
	ros::NodeHandle nh; //here NodeHandle is the main access point to communications with the ROS system. The first NodeHandle constructed will fully initialize this node. Lastly, NodeHandle destructed will close down the node.

	// Declaring the service/speedup

	ros::ServiceServer service =  nh.advertiseService("/accelarate", monitor); //declares subscriber; created subscriber "speedup" using the "topic"; The topic name is "speedup".
    
    // Function to loop the callback function i.e. monitor().

	// Spinning the callback function.
	
	ros::spin();//a function for calling a callback function, waiting for a message to be received, executing a callback function when it is received, & blocks until ROS invokes a shutdown
	return 0;
} //end od main()
