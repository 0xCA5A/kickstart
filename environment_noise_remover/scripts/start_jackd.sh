#!/bin/bash


#echo -e "[i] the audio devices"
#aplay -l


################################################################################
# defines
################################################################################
JACK_AUDIO_DEVICE_INDEX=0
JACK_SAMPLING_RATE=32000
JACK_NPERIODS=2                     # alsa wants this like this
JACK_PERIOD=320                     # chunk size
JACK_CHANNELS=2
JACK_INCHANNELS=${JACK_CHANNELS}
JACK_OUTCHANNELS=${JACK_CHANNELS}
################################################################################


################################################################################
# debug start settings
################################################################################
DAEMON=
OUTPUT=
[[ ${DEBUG} ]] && DAEMON_MODE='' || DAEMON_MODE='&'
[[ ${DEBUG} ]] && OUTPUT_MODE='\| tee' || OUTPUT_MODE='\&\>'
[[ ${DEBUG} ]] && echo "[i] run in debug mode" || echo ""

# exit
################################################################################
# main
################################################################################
echo -e "[i] start jackd..."

echo -e "[i] server playback latency: $(( ${JACK_NPERIODS} * ${JACK_PERIOD} * 1000 / ${JACK_SAMPLING_RATE} ))ms"
echo -e "[i] data chunk length: $(( 1000 / ( ${JACK_SAMPLING_RATE} / ${JACK_PERIOD} ) ))ms"

COMMAND="/usr/bin/jackd --no-mlock --realtime --realtime-priority 99 \
--port-max 32 --clock-source s --driver alsa \
--capture hw:${JACK_AUDIO_DEVICE_INDEX} --device hw:${JACK_AUDIO_DEVICE_INDEX} \
--duplex --inchannels ${JACK_INCHANNELS} --nperiods ${JACK_NPERIODS} \
--outchannels ${JACK_OUTCHANNELS} --playback hw:${JACK_AUDIO_DEVICE_INDEX} \
--period ${JACK_PERIOD} --rate ${JACK_SAMPLING_RATE} \
--shorts ${OUTPUT_MODE} /tmp/jack_server.log ${DAEMON_MODE}"

eval ${COMMAND}


sleep 1
echo -e "[i] enable real time scheduling for jackd"
sudo chrt -f -p 99 $(pidof jackd)

echo "[i] done"

