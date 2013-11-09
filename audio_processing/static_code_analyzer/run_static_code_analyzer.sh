#!/bin/bash

SOURCE_ROOT=../

CCCC_OUTPUT_DIR=cccc_output
CPPCHECK_OUTPUT_FILE=cppcheck.log


function print_separator
{
    echo -e "################################################################################"
}

function print_banner
{
    __BANNER_STRING=${1}

    echo -e "\n"
    print_separator
    echo -e "# ${__BANNER_STRING}"
    print_separator
}

function print_footer
{
    print_separator
    echo -e "\n"
}


print_banner " run cccc"
if [ ! -z $(which cccc) ]; then
    find ${SOURCE_ROOT} -regex ".*\.[cChH]\(pp\)?" -print | cccc --outdir=${CCCC_OUTPUT_DIR} --lang=c++ -
else
    echo -e "[!] cccc not found on your machine"
fi
print_footer

print_banner "run cppcheck"
if [ ! -z $(which cppcheck) ]; then
    cppcheck -v ../ --enable=all --platform=unix64 2> ${CPPCHECK_OUTPUT_FILE}
else
    echo -e "[!] cppcheck not found on your machine"
fi
print_footer

