
import os
import sys


class FanControlConfiguration:
    """class to store configuration data"""

    FAN_CONTROL_FILE = '/proc/acpi/ibm/fan'
    FAN_SPEED_STATE_CONFIG_VALUES = {'off':'0', 'low':'2', 'medium':'4', 'maximum':'7',  'automatic':'auto',  'disengaged':'disengaged'}
    FAN_SPEED_STATE_CONFIG_VALUES_MAPPING = ['off', 'low', 'medium', 'maximum', 'automatic', 'disengaged']

    
##        print command_string

##        print _config.FAN_SPEED_STATE_CONFIG_VALUES['off']
##        print _config.FAN_SPEED_STATE_CONFIG_VALUES['low']
##        print _config.FAN_SPEED_STATE_CONFIG_VALUES['medium']
##        print _config.FAN_SPEED_STATE_CONFIG_VALUES['maximum']
##        print _config.FAN_SPEED_STATE_CONFIG_VALUES['automatic']
##        print _config.FAN_SPEED_STATE_CONFIG_VALUES['disengaged']


    
    PROC_THERMAL_INFORMATION_FILE = '/proc/acpi/ibm/thermal'

    UPDATE_INTERVAL_IN_SECONDS = 1
    

    
    def __init__(self):
        print '[i] hello from FanControlConfiguration constructor'
        self.current_fan_level_string = 'automatic'
        #self.current_fan_level_value = FAN_SPEED_STATE_CONFIG_VALUES[self.current_fan_level_string]
        self.sensorsPresent = False
        self.procThermalPresent = False

        self.lastCPUTemp = 0
        self.lastGPUTemp = 0



    def checkConfig(self):
        #print '[i] hello from checkConfig'

        #check FAN_CONTROL_FILE
        if not os.path.isfile(self.FAN_CONTROL_FILE):
            sys.exit('[!] bad configuration, cant open config.FAN_CONTROL_FILE: %s' %  self.FAN_CONTROL_FILE)


        #check if sensors installed
        if not os.system("which sensors &> /dev/null"):
            self.sensorsPresent = True
        else:
            self.sensorsPresent = False
            print '[i] sensors not present!'

        #check if thermal information accessable through proc fs, check PROC_THERMAL_INFORMATION_FILE
        if os.path.isfile(self.PROC_THERMAL_INFORMATION_FILE):
            self.procThermalPresent = True
        else:
            self.procThermalPresent = False
            print '[i] no access to thermal data through proc fs!'

        #decide if config valid or not...
        if not self.procThermalPresent and not self.sensorsPresent:
            sys.exit('[!] bad configuration, neighther sensors present nor proc thermal info accessable!')


        print "[i] checkConfig: config seems to be ok!"



