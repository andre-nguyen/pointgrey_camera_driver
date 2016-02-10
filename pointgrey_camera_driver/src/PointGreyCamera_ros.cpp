//
// Created by andre on 09/02/16.
//

#include <pointgrey_camera_driver/PointGreyCamera_ros.h>

PointGreyCameraRos::PointGreyCameraRos(const ros::NodeHandle &pnh,
                                       const std::string &prefix)
    : CameraRosBase(pnh, prefix), pnh_(pnh)
{
    pgc_.setDesiredCamera(std::stoi(identifier()));

}

PointGreyCamera& PointGreyCameraRos::camera() { return pgc_; }



bool PointGreyCameraRos::Grab(const sensor_msgs::ImagePtr &image_msg,
                              const sensor_msgs::CameraInfoPtr &cinfo_msg) {

    pgc_.grabImage(*image_msg, "test");
    return true; // shit will throw if it fails
}

bool PointGreyCameraRos::RequestSingle() {
    throw std::runtime_error("PointGreyCameraRos::RequestSingle() is unsupported");
    return false;
}

void PublishImageMetadata(const ros::Time& time){
    throw std::runtime_error("PointGreyCameraRos::PublishImageMetadata() is unsupported");
}

void PointGreyCameraRos::Start() { pgc_.start(); }

void PointGreyCameraRos::Stop() { pgc_.stop(); }