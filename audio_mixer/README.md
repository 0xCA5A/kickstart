
this simple application allows you to test differen audio mixing algorithms.

general information about audio mixing
======================================
* http://www.soundonsound.com/sos/jun98/articles/20tips.html
* http://www.vttoth.com/CMS/index.php/technical-notes/68

general information about signal processing
===========================================
* http://www.edn.com/design/characterization/4380952/Create-noise-and-signals-with-software#id3507571-0-a


important information for mixing-dummy´s like me
================================================
If you're doing lots of audio processing, you might want to represent your audio levels as floating-point values, and only go back to the 16-bit space at the end of the process. High-end digital audio systems often work this way.
source: http://stackoverflow.com/questions/376036/algorithm-to-mix-sound

"Quieter by half" isn't quite correct. Because of the ear's logarithmic response, dividing the samples in half will make it 6-db quieter - certainly noticeable, but not disastrous.
source: http://stackoverflow.com/questions/376036/algorithm-to-mix-sound



how to
======

get some RIFF wave files you want to mix together using the algorithms or generate them using the script here:
https://github.com/ruf-telematik-ag/audiocore-dsp/blob/master/test_scripts/sine_wave_generator.py

then run the application mixerapplication with all the wave files as arguments
./mixerapplication samples/*

