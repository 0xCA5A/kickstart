the long way to my environment noise remover
============================================

hardware
--------
wandboard, freesace i.mx6 quad core

ubuntu 12.04 sd card image is ready [0]. it's armel, but anyway... we have four cores...


jackd
-----
linux reported a bus error on the first run, (un)fortunately i had already the pleasure with this issue...
```
[ 1929.045106] Alignment trap: not handling instruction edc76a06 at [<401f63da>]
[ 1929.052424] Unhandled fault: alignment exception (0x811) at 0x4014c1e6
```
rebuild the package [1], with this hack

```
sam@nemo:~/projects/github/kickstart/environment_noise_remover/jackd_fix$ diff ./jack-audio-connection-kit-0.121.0+svn4538/debian/rules  ./jack-audio-connection-kit-0.121.0+svn4538_virgin/debian/rules
112,115d111
< 
< # hack
< CFLAGS += -DPOST_PACKED_STRUCTURE=
< 
sam@nemo:~/projects/github/kickstart/environment_noise_remover/jackd_fix$
```

after a little configuration i got the system running with a playback latency of 6ms.
this has to be optimized later...



[0]
http://www.wandboard.org/index.php/downloads

[1]
http://www.cyberciti.biz/faq/rebuilding-ubuntu-debian-linux-binary-package/


