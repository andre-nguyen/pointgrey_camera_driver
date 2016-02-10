//
// Created by andre on 09/02/16.
//

#include <pointgrey_camera_driver/PointGreyCamera_ros.h>

PointGreyCameraRos::PointGreyCameraRos(const ros::NodeHandle &pnh,
                                       const std::string &prefix)
    : CameraRosBase(pnh, prefix), pnh_(pnh)
{
    
    pgc_.setDesiredCamera(identifier());
}