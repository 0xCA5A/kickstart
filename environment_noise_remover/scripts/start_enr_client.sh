#!/bin/bash


################################################################################
# defines
################################################################################
JACK_AUDIO_DEVICE_INDEX=0
JACK_SAMPLING_RATE=16000
JACK_NPERIODS=2
JACK_PERIOD=160
JACK_CHANNELS=2
JACK_INCHANNELS=${JACK_CHANNELS}
JACK_OUTCHANNELS=${JACK_CHANNELS}
################################################################################


################################################################################
# main
################################################################################
echo -e "[i] start enr_jack_client as deamon..."
../src/enr_jack_client &


sleep 1
echo -e "[i] current jack connections"
jack_lsp -c


echo -e "[i] done"
