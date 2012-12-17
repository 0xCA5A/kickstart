
import sys


class TempReader:

    def __init__(self):
        print '[i] hello from TempReader constructor'


    def _getRawTemp(self, _config):

        with open(_config.PROC_THERMAL_INFORMATION_FILE, 'r') as f:
            rawThermalData = f.read()
            f.close()

        rawThermalData = ' '.join(rawThermalData.split())
        return rawThermalData



    def getCurrentCPUTemp(self, _config):

        rawThermalData = self._getRawTemp(_config)

        if _config.procThermalPresent:
            rawThermalData = ' '.join(rawThermalData.split())

            tokens = rawThermalData.split(' ')
            cpuTemp = tokens[1]

            return int(cpuTemp)


        if _config.sensorsPresent:
            #read sensors information
            sys.exit("[!] getCurrentCPUTemp with sensors support not jet implemented...")


    def getCurrentGPUTemp(self, _config):

        rawThermalData = self._getRawTemp(_config)

        if _config.procThermalPresent:
            rawThermalData = ' '.join(rawThermalData.split())

            tokens = rawThermalData.split(' ')
            cpuTemp = tokens[4]

            return int(cpuTemp)


            if _config.sensorsPresent:
                #read sensors information
                sys.exit("[!] getCurrentGPUTemp with sensors support not jet implemented...")

