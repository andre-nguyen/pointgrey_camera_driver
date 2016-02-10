//
// Created by andre on 09/02/16.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "CannotResolve"
#ifndef POINTGREY_CAMERA_DRIVER_POINTGREYCAMERA_ROS_H_H
#define POINTGREY_CAMERA_DRIVER_POINTGREYCAMERA_ROS_H_H

#include "PointGreyCamera.h"

#include <string>

#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <cv_bridge/cv_bridge.h>
#include <camera_base/camera_ros_base.h>

class PointGreyCameraRos : public camera_base::CameraRosBase {
public:
    explicit PointGreyCameraRos(const ros::NodeHandle& pnh,
                                const std::string& prefix = std::string());

    PointGreyCamera& camera();

    bool RequestSingle();

    bool Grab(  const sensor_msgs::ImagePtr& image_msg,
                const sensor_msgs::CameraInfoPtr& cinfo_msg = nullptr) override;

    void PublishImageMetadata(const ros::Time& time);

    void Stop();
    void Start();


private:
    PointGreyCamera pgc_;
    ros::NodeHandle pnh_;
};

#endif //POINTGREY_CAMERA_DRIVER_POINTGREYCAMERA_ROS_H_H

#pragma clang diagnostic pop