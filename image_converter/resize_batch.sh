#!/bin/bash



################################################################################
# defines
################################################################################



################################################################################
# functions
################################################################################
source ./common_functions.sh



################################################################################
# main
################################################################################
source ./command_line_manager.sh


#call imagemagick
for IMAGE_NAME in $(find $RAW_DIRECTORY -maxdepth 1 -type f | rev | cut -d '/' -f 1 | rev); do

    echo -e "[i] process image $IMAGE_NAME..."

    echo -e "   * resize 20%..."
    convert -resize 20% $RAW_DIRECTORY/$IMAGE_NAME $OUT_DIRECTORY/resized_20percent_$IMAGE_NAME
    echo -e "   * resize 40%..."
    convert -resize 40% $RAW_DIRECTORY/$IMAGE_NAME $OUT_DIRECTORY/resized_40percent_$IMAGE_NAME
    echo -e "   * resize 60%..."
    convert -resize 60% $RAW_DIRECTORY/$IMAGE_NAME $OUT_DIRECTORY/resized_60percent_$IMAGE_NAME
    echo -e "   * resize 80%..."
    convert -resize 80% $RAW_DIRECTORY/$IMAGE_NAME $OUT_DIRECTORY/resized_80percent_$IMAGE_NAME

done




