#include "udp_bridge/udp_bridge.h"
#include "std_msgs/String.h"
#include "std_msgs/Bool.h"
#include "geographic_msgs/GeoPointStamped.h"
#include "asv_msgs/VehicleStatus.h"
#include <regex>
#include "boost/date_time/posix_time/posix_time.hpp"


std::map<udp_bridge::Channel,std::string> udp_bridge::UDPROSNode::topic_map;

int main(int argc, char **argv)
{
    ros::init(argc, argv, "zmg_bridge_agent");
    
    std::string host = "localhost";
    if (argc > 1)
        host = argv[1];
  
    
    boost::posix_time::ptime now = ros::WallTime::now().toBoost();
    std::string iso_now = std::regex_replace(boost::posix_time::to_iso_extended_string(now),std::regex(":"),"-");
    
    std::string log_filename = "nodes/udp_bridge_vehicle-"+iso_now+".bag";
    
    int send_port = 4200;
    int receive_port = 4201;
    
    udp_bridge::UDPROSNode n(host,send_port,receive_port,log_filename);

    n.addSender<geographic_msgs::GeoPointStamped, udp_bridge::position>("/position");
    n.addSender<std_msgs::String, udp_bridge::appcast>("/moos/appcast");
    n.addSender<geographic_msgs::GeoPoint,udp_bridge::origin>("/origin");
    n.addSender<asv_msgs::VehicleStatus,udp_bridge::vehicle_status>("/vehicle_status");
    
    n.addReceiver<std_msgs::Bool,udp_bridge::active>("/active");
    n.addReceiver<std_msgs::String,udp_bridge::helm_mode>("/helm_mode");
    n.addReceiver<std_msgs::String,udp_bridge::wpt_updates>("/moos/wpt_updates");
    n.addReceiver<std_msgs::String,udp_bridge::loiter_updates>("/moos/loiter_updates");

    n.spin();
    
    return 0;
}

