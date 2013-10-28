#!/bin/bash
# script to convert a mp3 file to 16khz wav


echo "[i] hello from mp3 to 16khz wave converter script"


if [ $# -ne 1 ]; then
    echo -e "[!] give me a mp3 file to convert"
    exit
fi

if [ ! -f $1 ]; then
    echo -e "[!] $1 is not a valid file"
    exit
fi

__INPUT_FILE_NAME=$1
__NUDE_FILE_NAME=$(basename ${__INPUT_FILE_NAME} .mp3)


if [ "x$(which mpg123)" == "x" ]; then
    echo "[!] mpg123 required, not found on this system"
    exit
fi

if [ "x$(which sox)" == "x" ]; then
    echo "[!] sox required, not found on this system"
    exit
fi


echo -e "[i] convert mp3 to mono wave using mpg123..."
mpg123 --mono -w ${__NUDE_FILE_NAME} ${__INPUT_FILE_NAME}

echo -e "[i] convert wave samplerate to 16khz using sox..."
sox ${__NUDE_FILE_NAME} -b 16 ${__NUDE_FILE_NAME}_16khz.wav channels 1 rate 16000

rm ${__NUDE_FILE_NAME}

echo -e "[i] result:"
file ${__NUDE_FILE_NAME}_16khz.wav
