


alix3d3

alsa configuration, 0db gain




headphone 
pcm





10 ms chunks

$> /usr/bin/jackd --realtime --realtime-priority 99 --port-max 128 --clock-source s --driver alsa --capture hw:0 --device hw:0 --duplex --inchannels 2 --nperiods 4 --outchannels 2 --playback hw:0 --period 480 --rate 48000
