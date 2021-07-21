#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


class CupChanger{

protected:
	ros::NodeHandle nh;
	image_transport::ImageTransport it;
	image_transport::Subscriber image_sub;
	image_transport::Publisher redPub;
	image_transport::Publisher bluePub;
	image_transport::Publisher greenPub;
	image_transport::Publisher allPub;
	
public:

CupChanger() : it(nh) {
	image_sub = it.subscribe("/kinect2/hd/image_color",1,&CupChanger::cupCb, this);
	bluePub = it.advertise("/color_filter/blue_cup",1);
	greenPub = it.advertise("/color_filter/green_cup",1);
	redPub = it.advertise("/color_filter/red_cup",1);
	allPub = it.advertise("/color_filter/all_cups",1);
}


void cupCb(const sensor_msgs::ImageConstPtr& msg) {


	cv_bridge::CvImagePtr cv_ptr;
	try {
		cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
	}
	catch (cv_bridge::Exception& e) {
		ROS_ERROR("cv_bridge exception: %s", e.what());
		return;
	}
	cv::Mat originalImage = cv_ptr->image;
	int rows = originalImage.rows;
	int cols = originalImage.cols;
	//gets image, and formats it as a matrix of triplets (b,g,r)

	std::vector<cv::Mat> chans;
	cv::split(originalImage,chans);
	cv::Mat blueImage = chans.at(0);
	cv::Mat greenImage = chans.at(1);
	cv::Mat redImage = chans.at(2);
	//3 matricies, one of blue only, one of red only, and one of green only


    //add code here




	cv::Mat onlyBlueCup;
	cv::Mat onlyGreenCup;
	cv::Mat onlyRedCup;
	cv::Mat allCups;

	cv_bridge::CvImage blueSend(std_msgs::Header(), "bgr8", onlyBlueCup);
	cv_bridge::CvImage greenSend(std_msgs::Header(), "bgr8", onlyGreenCup);
	cv_bridge::CvImage redSend(std_msgs::Header(), "bgr8", onlyRedCup);
	cv_bridge::CvImage allSend(std_msgs::Header(), "bgr8", allCups);
	bluePub.publish(blueSend.toImageMsg());
	greenPub.publish(greenSend.toImageMsg());
	redPub.publish(redSend.toImageMsg());	
	allPub.publish(allSend.toImageMsg());
	}
};
	
int main(int argc, char **argv) {
	ros::init(argc, argv, "cupChanger");
	CupChanger cupChanger;
	ros::spin();
	return 0;
}
