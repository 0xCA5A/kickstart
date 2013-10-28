get audio files from youtube videos
===================================

use the youtube to mp3 converter to get the mp3 audio file from the videos
* http://www.youtube-mp3.org/de

screaming people: http://www.youtube.com/watch?v=yF-Rl5acQdA
screaming at people 2: http://www.youtube.com/watch?v=tTiLD86zSws

convert mp3 to mono wave file
=============================
mpg123 --mono -w result.wav input.mp3

convert sampling rate to 16khz
==============================
sox input.wav -b 16 output.wav channels 1 rate 16000

or use script i wrote...

