#include <ros/ros.h>
#include "std_msgs/Int32.h"
#include "std_msgs/Float32.h"
#include "ardrone_autonomy/Navdata.h"
#include"geometry_msgs/Twist.h" 
#include "std_msgs/Empty.h"

using namespace std;

double altitude;
void heightControl(const ardrone_autonomy::Navdata& msg_in);
float GetErrorLin(int alt,int h);
geometry_msgs::Twist vel;


int main(int argc, char **argv)
{
  ros::init(argc,argv,"takeoff");
  ros::NodeHandle n;
  ros::start();
  
  ros::Subscriber sub = n.subscribe("/ardrone/navdata", 100, heightControl);
  ros::Publisher takeoff1=n.advertise<std_msgs::Empty>("/ardrone/takeoff",10,true);
  ros::Publisher velpub = n.advertise<geometry_msgs::Twist>("cmd_vel",100,true);
  ros::Rate loop_rate(10);  
  double ErrorLin=0;
  double PrvError=0;
  double DifError=0;
  double InError=0;
  std_msgs::Empty emp;
  takeoff1.publish(emp);
  int h = atof(argv[1]);
  int kp = atof(argv[2]);
  int ki = atof(argv[3]);
  int kd = atof(argv[4]);

   while (ros::ok())  {
        
               ros::Time t =ros::Time::now();
                PrvError=ErrorLin;
		ErrorLin=GetErrorLin(altitude,h);
                DifError=ErrorLin-PrvError;
                InError=InError+ErrorLin;
                vel.linear.z=(kp*0.001*ErrorLin)+(ki*0.000001*InError)+(kd*0.0000001*DifError);
                vel.linear.x=0;
		vel.linear.y=0;
                vel.angular.x=0;
		vel.angular.y=0;
		vel.angular.z=0;
                velpub.publish(vel);
               
                printf("%lf ",t.toSec());
 		printf("%lf\n",altitude);
        ros::spinOnce();
         loop_rate.sleep();
        
}       
        
  }



void heightControl(const ardrone_autonomy::Navdata& msg_in)
{
  altitude=msg_in.altd;
  
}
float GetErrorLin(int alt ,int h)
{
    float Etx=-(alt-h);
    return (Etx);
}
