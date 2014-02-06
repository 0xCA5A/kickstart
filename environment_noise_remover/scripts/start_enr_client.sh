#!/bin/bash


################################################################################
# defines
################################################################################
################################################################################


################################################################################
# main
################################################################################
if [ $(ps aux | grep enr_jack_client | grep -v grep | wc -l) -ge 1 ]; then
    echo -e "[i] remove already started enr_jack_client..."
    killall enr_jack_client
fi

echo -e "[i] start enr_jack_client as deamon..."
../src/enr_jack_client &

sleep 1
echo -e "[i] current jack connections"
jack_lsp -c


echo -e "[i] done"
