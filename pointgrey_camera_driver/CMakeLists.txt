cmake_minimum_required(VERSION 2.8.3)
project(pointgrey_camera_driver)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

find_package(catkin REQUIRED COMPONENTS
  roscpp roslint nodelet sensor_msgs wfov_camera_msgs
  image_exposure_msgs camera_info_manager image_transport
  dynamic_reconfigure diagnostic_updater camera_base)

# If the user has manually installed flycapture, use the system path version. If not,
# download it. We can't resolve this dependency using the usual rosdep means because
# the Point Grey EULA prohibits redistributing the headers or the packages which
# contains them. Fortunately, we can download the archive directly from their
# website during this build process.
find_library(POINTGREY_LIB flycapture)
if(NOT POINTGREY_LIB)
  message(STATUS "libflycapture not found in system library path")
  include(cmake/DownloadFlyCap.cmake)
  download_flycap(POINTGREY_LIB POINTGREY_INCLUDE_DIR)
  message(STATUS "libflycapture library: ${POINTGREY_LIB}")
  message(STATUS "libflycapture include: ${POINTGREY_INCLUDE_DIR}")
  include_directories(${POINTGREY_INCLUDE_DIR})
endif()


generate_dynamic_reconfigure_options(
  cfg/PointGrey.cfg
)

catkin_package(
  CATKIN_DEPENDS roscpp nodelet camera_base
  LIBRARIES ${PROJECT_NAME} ${POINTGREY_LIB}
  INCLUDE_DIRS include ${POINTGREY_INCLUDE_DIR}
)


include_directories(include ${catkin_INCLUDE_DIRS} ${POINTGREY_INCLUDE_DIR})

add_library(${PROJECT_NAME}
  src/PointGreyCamera.cpp
  src/PointGreyCamera_ros.cpp
)
target_link_libraries(${PROJECT_NAME}
  ${POINTGREY_LIB} ${catkin_LIBRARIES}
)
add_dependencies( ${PROJECT_NAME}
  ${catkin_EXPORTED_TARGETS}
  ${PROJECT_NAME}_gencfg
  ${PROJECT_NAME}_gencpp
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
)

add_library(PointGreyCameraNodelet src/nodelet.cpp)
target_link_libraries(PointGreyCameraNodelet ${PROJECT_NAME} ${catkin_LIBRARIES})

add_library(PointGreyStereoCameraNodelet src/stereo_nodelet.cpp)
target_link_libraries(PointGreyStereoCameraNodelet ${PROJECT_NAME} ${catkin_LIBRARIES})

add_executable(pointgrey_camera_node src/node.cpp)
target_link_libraries(pointgrey_camera_node ${PROJECT_NAME} ${catkin_LIBRARIES})
set_target_properties(pointgrey_camera_node
                      PROPERTIES OUTPUT_NAME camera_node PREFIX "")

add_executable(pointgrey_stereo_node src/stereo_node.cpp)
target_link_libraries(pointgrey_stereo_node ${PROJECT_NAME} ${catkin_LIBRARIES})
set_target_properties(pointgrey_stereo_node
                      PROPERTIES OUTPUT_NAME stereo_node PREFIX "")

add_executable(pointgrey_list_cameras src/list_cameras.cpp)
target_link_libraries(pointgrey_list_cameras ${PROJECT_NAME} ${catkin_LIBRARIES})
set_target_properties(pointgrey_list_cameras
                      PROPERTIES OUTPUT_NAME list_cameras PREFIX "")

install(TARGETS
  ${PROJECT_NAME}
  PointGreyCameraNodelet
  PointGreyStereoCameraNodelet
  pointgrey_camera_node
  pointgrey_stereo_node
  pointgrey_list_cameras
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

# Redistributing the flycapture .so file is permitted by the SDK EULA:
# http://www.ptgrey.com/support/kb/data/PGR-FlyCap-SDK-LA.pdf
install(FILES ${POINTGREY_LIB} DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION})

install(FILES nodelet_plugins.xml DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION} )

install(DIRECTORY launch DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})

install(DIRECTORY include/${PROJECT_NAME}/
 DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
 FILES_MATCHING PATTERN "*.h"
 PATTERN "*.*~" EXCLUDE
)

roslint_cpp()
