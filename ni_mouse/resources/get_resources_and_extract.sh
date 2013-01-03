#!/bin/bash

<<<<<<< HEAD
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

=======

# read here
# http://www.codeproject.com/Articles/148251/How-to-Successfully-Install-Kinect-on-Windows-Open



NITE_BIN_URL="http://www.openni.org/wp-content/uploads/NiTE2.0.zip"
OPENNI_GIT_URL="https://github.com/OpenNI/OpenNI2.git"
SENSOR_GIT_URL="https://github.com/avin2/SensorKinect.git"





function get_bin_archive
{
    BIN_ARCHIVE_NAME=$(echo $1 | rev | cut -d '/' -f 1 | rev)

    echo -e "[i] get bin $BIN_ARCHIVE_NAME"

    if [ ! -f $BIN_ARCHIVE_NAME ]; then
        echo -e " * get $BIN_ARCHIVE_NAME ($1)...\n"
        wget $1
    else
        echo -e " * file $BIN_ARCHIVE_NAME exists, wont get..."
    fi
}


function get_git_project
{
    GIT_PROJECT_NAME=$(echo $1 | rev | cut -d '/' -f 1 | rev | cut -d '.' -f 1)

    echo -e "[i] get git project $GIT_PROJECT_NAME"

    if [ ! -d $GIT_PROJECT_NAME ]; then
        echo -e " * get $GIT_PROJECT_NAME via git...\n"
        git clone $1
    else
        echo -e " * project $GIT_PROJECT_NAME already cloned, try to pull..."
        cd $GIT_PROJECT_NAME; git pull; cd - &>/dev/null
    fi

}


function get_files {

    echo -e "[i] get necessary binaries..."

    get_bin_archive $NITE_BIN_URL
    get_git_project $SENSOR_GIT_URL
    get_git_project $OPENNI_GIT_URL
}


# function extract_bin
# {
#     echo -e "[i] extract nite bin"
# 
#     unzip $NITE_ARCHIVE_NAME
# 
# }


function extract {

    echo -e "[i] extract nite archive... "
#     for f in $(ls *.bz2); do echo -e " * process $f"; tar -xf $f; done


    if [ ! -d NiTE2.0 ]; then
        unzip NiTE2.0.zip
    fi

    if [ ! -d NiTE-2.0.0 ]; then
        tar -xf NiTE2.0/NiTE-Linux-x64-2.0.0.tar.bz2
    fi
>>>>>>> cd21a8fc6751710794e99845045c4ba454484065
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


<<<<<<< HEAD
function install_openni {

    echo -e "install openni..."
    cd OpenNI
    sudo ./install.sh
    cd -

}


function install_sensor {

    echo -e "install sensor..."
    cd Sensor
    sudo ./install.sh
    cd -

}

function install_nite {

    echo -e "install nite..."
    cd NITE
    sudo ./install.sh
    cd -

}

function install_sensorkinect {

    echo -e "install sensor kinect..."
    cd SensorKinect/Platform/Linux/CreateRedist/
    sudo chmod +x RedistMaker
    sudo ./RedistMaker
    cd ../Redist/Sensor-Bin-Linux-x64-v5.1.0.25/
    sudo chmod +x install.sh
    sudo ./install.sh

}

#main
get_files
extract
symlink
install_openni
install_sensor
install_sensorkinect
=======
#main
get_files

extract

exit

symlink

>>>>>>> cd21a8fc6751710794e99845045c4ba454484065


