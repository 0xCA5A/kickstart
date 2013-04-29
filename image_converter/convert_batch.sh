#!/bin/bash



################################################################################
# defines
################################################################################
SEPIA_IMAGE_PREFIX="sepia"
SEPIA_IMAGE_OUTPUT_DIR_NAME="sepia"

GRAY_IMAGE_PREFIX="gray"
GRAY_IMAGE_OUTPUT_DIR_NAME="gray"

MONOCHROME_IMAGE_PREFIX="monochrome"
MONOCHROME_IMAGE_OUTPUT_DIR_NAME="monochrome"



################################################################################
# functions
################################################################################
source ./common_functions.sh



################################################################################
# main
################################################################################
source ./command_line_manager.sh

#clean output directory
#todo: put the cleanup into a function
echo -e "[i] clean sepia output directory $OUT_DIRECTORY/$SEPIA_IMAGE_OUTPUT_DIR_NAME..."
if [ -d $OUT_DIRECTORY/$SEPIA_IMAGE_OUTPUT_DIR_NAME ]; then
    rm -f $OUT_DIRECTORY/$SEPIA_IMAGE_OUTPUT_DIR_NAME/*
else
    mkdir $OUT_DIRECTORY/$SEPIA_IMAGE_OUTPUT_DIR_NAME
fi

echo -e "[i] clean gray output directory $OUT_DIRECTORY/$GRAY_IMAGE_OUTPUT_DIR_NAME..."
if [ -d $OUT_DIRECTORY/$GRAY_IMAGE_OUTPUT_DIR_NAME ]; then
    rm -f $OUT_DIRECTORY/$GRAY_IMAGE_OUTPUT_DIR_NAME/*
else
    mkdir $OUT_DIRECTORY/$GRAY_IMAGE_OUTPUT_DIR_NAME
fi

echo -e "[i] clean monochrome output directory $OUT_DIRECTORY/$MONOCHROME_IMAGE_OUTPUT_DIR_NAME..."
if [ -d $OUT_DIRECTORY/$MONOCHROME_IMAGE_OUTPUT_DIR_NAME ]; then
    rm -f $OUT_DIRECTORY/$MONOCHROME_IMAGE_OUTPUT_DIR_NAME/*
else
    mkdir $OUT_DIRECTORY/$MONOCHROME_IMAGE_OUTPUT_DIR_NAME
fi


#call imagemagick
for IMAGE_NAME in $(find $RAW_DIRECTORY -maxdepth 1 -type f | rev | cut -d '/' -f 1 | rev); do

    echo -e "[i] process image $IMAGE_NAME..."

    #do sepia
    echo -e "   * sepia..."
    convert $RAW_DIRECTORY/$IMAGE_NAME -set colorspace RGB -sepia-tone 60% $OUT_DIRECTORY/$SEPIA_IMAGE_OUTPUT_DIR_NAME/${SEPIA_IMAGE_PREFIX}_60_$IMAGE_NAME
    convert $RAW_DIRECTORY/$IMAGE_NAME -set colorspace RGB -sepia-tone 80% $OUT_DIRECTORY/$SEPIA_IMAGE_OUTPUT_DIR_NAME/${SEPIA_IMAGE_PREFIX}_80_$IMAGE_NAME
    convert $RAW_DIRECTORY/$IMAGE_NAME -set colorspace RGB -sepia-tone 100% $OUT_DIRECTORY/$SEPIA_IMAGE_OUTPUT_DIR_NAME/${SEPIA_IMAGE_PREFIX}_100_$IMAGE_NAME

    #do grey
    echo -e "   * gray..."
    convert $RAW_DIRECTORY/$IMAGE_NAME -set colorspace Gray $OUT_DIRECTORY/$GRAY_IMAGE_OUTPUT_DIR_NAME/${GRAY_IMAGE_PREFIX}_$IMAGE_NAME

    #do monochrome
    echo -e "   * monochrome..."
    convert $RAW_DIRECTORY/$IMAGE_NAME -monochrome $OUT_DIRECTORY/$MONOCHROME_IMAGE_OUTPUT_DIR_NAME/${MONOCHROME_IMAGE_PREFIX}_$IMAGE_NAME

done




