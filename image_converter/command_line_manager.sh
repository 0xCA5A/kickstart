
unset RAW_DIRECTORY
unset OUT_DIRECTORY

#check commend line arguments
if [ $# -eq 1 ] || [ $# -eq 2 ]; then
    echo -e "[i] nr of command line arguments ok..."
else
    echo -e "[!] give me at least a source directory, two arguments max..."
    print_howto
    exit
fi

echo -e "[i] set raw data directory..."
if [ -d $1 ]; then
    echo -e "   * take $1 as raw directory"
    RAW_DIRECTORY=$1
else
    echo -e "   * $RAW_DIRECTORY is not a valid directory..."
    exit
fi

echo -e "[i] set output directory..."
OUT_DIRECTORY=./
if [ $# -eq 2 ]; then
    if [ -d $2 ]; then
        OUT_DIRECTORY=$2
    else
        echo -e "[!] output directory $2 is not valid..."
        exit
    fi

fi
echo -e "   * take $OUT_DIRECTORY as output directory"
