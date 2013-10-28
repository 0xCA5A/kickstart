#!/bin/bash
# script to convert stereo RIFF wave files to left and right channel mono files, converted to a specific sampling rate

function extract_and_convert_audio_wave_channels
{
    if [ $# -ne 2 ]; then
        echo "[!] function $FUNCNAME expects a filename and a sampling rate in hz as argument"
        exit
    fi

    RIFF_WAVE_INPUT_FILE_EXTENSION=$(echo $1 | rev | cut -d '.' -f 1 | rev)
    RIFF_WAVE_INPUT_FILE_NAME=$(basename ${1} .${RIFF_WAVE_INPUT_FILE_EXTENSION})
    SAMPLE_RATE=$2

    echo "[i] convert file $1"

    RIFF_WAVE_OUTPUT_FILE=${RIFF_WAVE_INPUT_FILE_NAME}_mono_left_${SAMPLE_RATE}_samplingrate.${RIFF_WAVE_INPUT_FILE_EXTENSION}
    echo " * extract and convert left channel to file ${RIFF_WAVE_OUTPUT_FILE}, sampling rate ${SAMPLE_RATE}..."
    sox $1 -c 1 -r ${SAMPLE_RATE} ${RIFF_WAVE_OUTPUT_FILE} remix 1

    RIFF_WAVE_OUTPUT_FILE=${RIFF_WAVE_INPUT_FILE_NAME}_mono_right_${SAMPLE_RATE}_samplingrate.${RIFF_WAVE_INPUT_FILE_EXTENSION}
    echo " * extract and convert right channel to file ${RIFF_WAVE_OUTPUT_FILE}, sampling rate ${SAMPLE_RATE}..."
    sox $1 -c 1 -r ${SAMPLE_RATE} ${RIFF_WAVE_OUTPUT_FILE} remix 2

    unset RIFF_WAVE_OUTPUT_FILE
    unset RIFF_WAVE_INPUT_FILE_EXTENSION
    unset RIFF_WAVE_INPUT_FILE_NAME
    unset SAMPLE_RATE
}



echo "[i] hello from converter script"

if [ "x$(which sox)" == "x" ]; then
    echo "[!] sox required, not found on this system"
    exit
fi

if [ $# -ne 1 ]; then
    echo "[!] i want one argument, one valid wave file"
    exit
fi

if [ ! -f $1 ]; then
    echo "[!] file $1 does not exist"
    exit
fi


echo "[i] arguments ok, convert file $1 using sox"

extract_and_convert_audio_wave_channels $1 32000
extract_and_convert_audio_wave_channels $1 24000
extract_and_convert_audio_wave_channels $1 16000
extract_and_convert_audio_wave_channels $1 8000

