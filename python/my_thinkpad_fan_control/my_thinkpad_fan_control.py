#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys

from TempReader import *
from FanControlConfiguration import *
from View import *


class MyFanControl:
    """class to control ibm thinkpad fan. developed for thinkpad t60"""
    
    def __init__(self, config):
        print '[i] hello from MyFanControl constructor'
        self._config = config
        self._tempReader = TempReader()
        self._view = View(self)

        
    def updateFanLevel(self, fanLevel):
        print '[i] update fanlevel to: %s' % fanLevel

        _fan_control_command_string = 'echo level %s | tee %s' % (self._config.FAN_SPEED_STATE_CONFIG_VALUES[fanLevel], self._config.FAN_CONTROL_FILE)
        #print '[i] run command: ' + _fan_control_command_string
        os.system(_fan_control_command_string)
        

    def cleanUp(self):
        print '[i] clean up!'

        self.updateFanLevel('automatic')

        sys.exit(1)


        

if __name__ == "__main__":

    #check if root or not...
    if not os.geteuid() == 0:
        sys.exit('script must be run as root...')

    config = FanControlConfiguration()
    config.checkConfig()

    myFanControl = MyFanControl(config)





