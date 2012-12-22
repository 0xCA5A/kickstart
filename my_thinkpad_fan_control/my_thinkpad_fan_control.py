#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys

from temp_reader import *
from load_reader import *
from thinkpad_fan_control_configuration import *
from view import *


class MyThinkpadFanControl(object):
    """class to control ibm thinkpad fan. developed for thinkpad t60. my baby..."""

    def __init__(self, config):
        print '[i] hello from MyThinkpadFanControl constructor'
        self._config = config
        self._temp_reader = TempReader()
        self._load_reader = LoadReader()
        self._view = View(self)



    def update_fan_level(self, fan_level):
        print '[i] update fanlevel to: %s' % fan_level

        #DANGER DANGER DANGER
        _fan_control_command_string = 'echo level %s | tee %s' % (self._config.FAN_SPEED_STATE_CONFIG_VALUES[fan_level], self._config.FAN_CONTROL_FILE)
        #print '[i] run command: ' + _fan_control_command_string
        os.system(_fan_control_command_string)



    def clean_up(self):
        print '[i] clean up, set fan level to automatic!'

        self.update_fan_level('automatic')

        sys.exit(1)




if __name__ == "__main__":

    #check if root or not...
    if not os.geteuid() == 0:
        sys.exit('script must be run as root...')

    config = ThinkpadFanControlConfiguration()
    config.check_config()

    my_fan_control = MyThinkpadFanControl(config)





