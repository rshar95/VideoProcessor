all: video_acquisition_module video_processing_module distributed_processing_module network_communication_module database_module front_end_module

video_acquisition_module: video_acquisition_module.cpp
    g++ -o video_acquisition_module video_acquisition_module.cpp

video_processing_module: video_processing_module.cpp
    g++ -o video_processing_module video_processing_module.cpp

distributed_processing_module: distributed_processing_module.cpp
    mpic++ -o distributed_processing_module distributed_processing_module.cpp

network_communication_module: network_communication_module.cpp
    g++ -o network_communication_module network_communication_module.cpp

database_module: database_module.cpp
    g++ -o database_module database_module.cpp -lmysqlx

front_end_module: front_end_module.cpp
    g++ -o front_end_module front_end_module.cpp

run:
    ./video_acquisition_module
    ./video_processing_module
    mpirun -np 4 ./distributed_processing_module
    ./network_communication_module
    ./database_module
    ./front_end_module

clean:
    rm -f video_acquisition_module video_processing_module distributed_processing_module network_communication_module database_module front_end_module
