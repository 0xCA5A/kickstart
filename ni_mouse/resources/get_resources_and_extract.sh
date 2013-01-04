#!/bin/bash

function get_files {

    echo -e "get necessary binaries..."


    if [ ! -f nite-bin-linux-x64-v1.5.2.21.tar.bz2 ];
        then echo -e " * get nite-bin-linux-x64-v1.5.2.21.tar.bz2...\n"
        wget http://75.98.78.94/downloads/nite-bin-linux-x64-v1.5.2.21.tar.bz2
    else
        echo -e " * file nite-bin-linux-x64-v1.5.2.21.tar.bz2 exists, wont get..."    
    fi

    if [ ! -f sensor-bin-linux-x64-v5.1.0.41.tar.bz2 ];
        then echo -e " * get sensor-bin-linux-x64-v5.1.0.41.tar.bz2...\n"
        wget http://75.98.78.94/downloads/sensor-bin-linux-x64-v5.1.0.41.tar.bz2
    else
        echo -e " * file sensor-bin-linux-x64-v5.1.0.41.tar.bz2 exists, wont get..."    
    fi

    if [ ! -f openni-bin-dev-linux-x64-v1.5.2.23.tar.bz2 ];
        then echo -e " * get openni-bin-dev-linux-x64-v1.5.2.23.tar.bz2...\n"
        wget http://75.98.78.94/downloads/openni-bin-dev-linux-x64-v1.5.2.23.tar.bz2
    else
        echo -e " * file openni-bin-dev-linux-x64-v1.5.2.23.tar.bz2 exists, wont get..."    
    fi

    echo -e " * get SensorKinect (git)...\n"
    git clone https://github.com/avin2/SensorKinect

    echo -e " * update to git revision faf4994fceba82e6fbd3dad16f79e4399be0c184"
    cd SensorKinect
    git checkout faf4994fceba82e6fbd3dad16f79e4399be0c184
    cd -

}


function extract {

    echo -e "extract files... "
    for f in $(ls *.bz2); do echo -e " * process $f"; tar -xf $f; done 

}


function symlink {

    echo -e "create symlinks..."
    if [ -h NITE ]; then rm NITE; fi
    ln -s NITE-Bin-Dev-Linux-x64-v1.5.2.21 NITE
    ls -l NITE

    if [ -h OpenNI ]; then rm OpenNI; fi
    ln -s OpenNI-Bin-Dev-Linux-x64-v1.5.2.23 OpenNI
    ls -l OpenNI

    if [ -h Sensor ]; then rm Sensor; fi
    ln -s Sensor-Bin-Linux-x64-v5.1.0.41 Sensor
    ls -l Sensor

}


function install_openni {

    echo -e "install openni..."
    cd OpenNI
    chmod +x install.sh
    sudo ./install.sh
    cd -

}


function install_sensor {

    echo -e "install sensor..."
    cd Sensor
    chmod +x install.sh
    sudo ./install.sh
    cd -

}

function install_nite {

    echo -e "install nite..."
    cd NITE
    chmod +x install.sh
    sudo ./install.sh
    cd -

}

function install_sensorkinect {

    echo -e "install sensor kinect..."
    cd SensorKinect/Platform/Linux/CreateRedist/
    chmod +x RedistMaker
    ./RedistMaker
    cd ../Redist/Sensor-Bin-Linux-x64-v5.1.0.25/
    chmod +x install.sh
    sudo ./install.sh

}

#main
get_files
extract
symlink
install_openni
install_sensor
install_sensorkinect



