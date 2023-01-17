#include <iostream>
#include <stdlib.h>

using namespace std;

void start_video_acquisition_module() {
    // execute the video acquisition module
    system("./video_acquisition_module");
}

void start_video_processing_module() {
    // execute the video processing module
    system("./video_processing_module");
}

void start_distributed_processing_module() {
    // execute the distributed processing module
    system("./distributed_processing_module");
}

void start_network_communication_module() {
    // execute the network communication module
    system("./network_communication_module");
}

void start_database_module() {
    // execute the database module
    system("./database_module");
}

void start_front_end_module() {
    // execute the front-end module
    system("./front_end_module");
}

int main() {
    start_video_acquisition_module();
    start_video_processing_module();
    start_distributed_processing_module();
    start_network_communication_module();
    start_database_module();
    start_front_end_module();
    return 0;
