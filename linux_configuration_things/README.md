# bash
## alias
```
alias hibernate='sudo pm-hibernate; dm-tool lock'
alias suspend='sudo pm-suspend; dm-tool lock'

alias cpuinfo='cat /proc/cpuinfo | grep MHz'
alias kate='kate &>/dev/null'

alias bat='upower -i /org/freedesktop/UPower/devices/battery_BAT0| grep -E "state|to\ full|percentage"'
```
