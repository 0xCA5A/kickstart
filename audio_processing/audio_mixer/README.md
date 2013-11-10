this simple application allows you to test and compare differen audio mixing algorithms.


limitations
===========
* only 16khz samplingrate is supported


how to
======
get some 16khz RIFF wave files you want to mix together using the algorithms or generate them using the script here:
https://github.com/ruf-telematik-ag/audiocore-dsp/blob/master/test_scripts/sine_wave_generator.py

build and run the mixerapplication with all your wave files as arguments
./mixerapplication *.wav


general information about audio mixing
======================================
* http://www.soundonsound.com/sos/jun98/articles/20tips.html
* http://www.vttoth.com/CMS/index.php/technical-notes/68
* http://club15cc.com/code/ios/mixing-audio-without-clipping-limiters-etc
* http://www.musicdsp.org/archive.php?classid=4#274
* http://omniaaudio.com/tech/lookahed.pdf


general information about signal processing and audio stuff
===========================================================
* http://www.edn.com/design/characterization/4380952/Create-noise-and-signals-with-software#id3507571-0-a
* http://en.wikipedia.org/wiki/Dynamic_range_compression
