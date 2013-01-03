#!/bin/bash


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


#main
get_files

extract

exit

symlink



