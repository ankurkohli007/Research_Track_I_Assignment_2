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
//#include <conio.h> //for getch()

using namespace std; //it means we uses the namespace named std(standard), It means we use all the things with in “std” namespace
using namespace ros; //It means we use all the things with in “ros” namespace

//Declaring ServiceClient to the custom service as global object to get the response for updating the velocity of the autopilot robot.
ros::ServiceClient client1;

//Declaring Publisher to the custom topic
ros::Publisher pub; //definition of publisher as global (pub as global because it also uses in the CallBack (subscriber)); also publish the velocity to the robot

bool halt = false;

//Declaring the function to read the input char by user

char GetChar()
{
	char ch;
	cout<<"*****MENU FOR ROBOT CONTROL*****"<<endl;
	cout<<"\n";
	cout<<"For reset robot's position enter r/R"<<endl;
	cout<<"\n";
	cout<<"To decrease robot's velocity enter j/J"<<endl;
	cout<<"\n";
	cout<<"To increase robot's velocity enter l/L"<<endl;
	cout<<"\n";
	cout<<"To quit enter q/Q "<<endl;
	cout<<"\n";
	cin >> ch;
	return ch;
	
	/*char ch;
	cout<<"*****MENU FOR ROBOT CONTROL*****"<<endl;
	cout<<"For reset robot's position enter r/R"<<endl;
	cout<<"To decrease robot's velocity enter ↓ "<<endl;
	cout<<"To increase robot's velocity enter ↑"<<endl;
	cin >> ch;
	return ch;*/
	
}// end of GetChar()

//Function needed for the UserInterface_node to always be ready to ask the user for increasing or decreasing the robot's speed.
void interfacing()
{

	//Creating velocity object which will send the updated data to the controller
	
	second_assignment::Velocity vel_srv;

	//Reading the input entered by the user

	char inp = GetChar();

	//Sending the request as the user's input
	
	vel_srv.request.input = inp;

	//If user give input as 'q' or 'Q' than it will quit the program and manage the errors

	if(inp == 'q' || inp == 'Q') 
	{
	
	halt = true;
	
	}
	
	if(inp != 'q' && inp != 'Q' && inp != 'j' && inp != 'J' && inp != 'l' && inp != 'L' && inp != 'r' && inp != 'R') 
	{
	
	cout<<"Sorry!! WRONG INPUT ENTERED!! Please, try with the new input as specified in the control menu, Thanks!";
	
	}

	////waiting for service existence
	
	client1.waitForExistence();
	
	//now sending request
	
	client1.call(vel_srv);

	//Declaration of custom message to send the response to autopilotrobot.cpp
	
	second_assignment::Vel my_vel;
	
	//Here, assigning the value of the response of the service to the custom message 
	my_vel.a = vel_srv.response.output;
	
	//Now, publishing the value to the correct topic
	pub.publish(my_vel);
}

int main (int argc, char **argv) //initiating main(); here "argc" means argument count, whereas "argv" means argument values; these variables are passed to the main() when it starts executing
{

	// Initializing the node, setting up the NodeHandle for handling the communication with the ROS system.
	
	/* here ros::init() function needs to monitor "argc" & "argv" so that it can perform on any ROS arguments & name remapping that were provided at the command line. For programmatic remappings we can use a different version of init() which takes remappings directly, but for most command-line programs, passing "argc" & "argv" is the easiest way to do it.  The third argument to init() is the name of the node. You must call one of the versions of ros::init() before using any other part of the ROS system. */

	ros::init(argc, argv, "user_interface"); //initialized the node, setup the NodeHandle for handling the communication with the ROS system 
	
	ros::NodeHandle nh; //here NodeHandle is the main access point to communications with the ROS system. The first NodeHandle constructed will fully initialize this node. Lastly, NodeHandle destructed will close down the node

	// Initialization of the the custom topic's publisher of the message Vel

	pub = nh.advertise<second_assignment::Vel>("/accval", 10); 

	//Initialization of the client to the custom service with his attributes

	client1 = nh.serviceClient<second_assignment::Velocity>("/accelarate");

	// Loop of the function that asks the user what to do

	while(ros::ok() && !halt)
	{
		interfacing(); //calling interfacing()
		
		ros::spinOnce(); //ros::spinOnce() will call all the callbacks waiting to be called at that point in time (periodically), spinOnce() meant for single-threaded applications, not optimized for being called from multiple threads at once, & spinOnce() handles the events and returns immediately
	}

	return 0;
}
