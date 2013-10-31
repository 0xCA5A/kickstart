#!/bin/bash

echo -e "[i] run cccc"
if [ ! -z $(which cccc) ]; then
    find ../ -regex ".*\.[cChH]\(pp\)?" -print | cccc --outdir=cccc_output --lang=c++ -
else
    echo -e "[!] cccc not found"
fi

echo -e "[i] run cppcheck"
if [ ! -z $(which cppcheck) ]; then
    cppcheck -v ../ --enable=all --platform=unix64 2>cppcheck.log
else
    echo -e "[!] cppcheck not found"
fi

