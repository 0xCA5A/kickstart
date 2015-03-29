# bash
## alias
```
alias hibernate='sudo pm-hibernate; dm-tool lock'
alias suspend='sudo pm-suspend; dm-tool lock'

alias cpuinfo='while true; do clear; echo "### cpuinfo alias ###"; cat /proc/cpuinfo | grep cpu | grep -i MHZ | cut -d ':' -f 2; sleep 1; done'
alias bat='upower -i /org/freedesktop/UPower/devices/battery_BAT0| grep -E "state|to\ full|percentage"'

alias wlan0_watch='watch -n 0.1 iw wlan0 station dump' 

alias kate='/usr/bin/kate 2>&- 1>&-'
alias makemp='make -j $(nproc)'
alias find_tab_files_recursive='grep -rP '\t' | cut -d ':' -f 1 | sort | uniq | grep -v Binary'
alias lock='dm-tool lock'
alias mute='amixer -q -D pulse sset Master toggle'

alias keyboard_us='setxkbmap -model pc105 -layout us -variant intl -option nodeadkeys'
alias keyboard_ch='setxkbmap -model pc105 -layout ch -variant de -option nodeadkeys'

alias fix_screens='xrandr --output HDMI2 --left-of HDMI3' 

```
