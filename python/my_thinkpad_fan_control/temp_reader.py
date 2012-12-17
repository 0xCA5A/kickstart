
import sys


class TempReader(object):

    def __init__(self):
        print '[i] hello from TempReader constructor'



    def _get_raw_temp(self, _config):

        with open(_config.PROC_THERMAL_INFORMATION_FILE, 'r') as f:
            raw_thermal_data = f.read()
            f.close()

        raw_thermal_data = ' '.join(raw_thermal_data.split())
        return raw_thermal_data



    def get_current_cpu_temp(self, _config):

        raw_thermal_data = self._get_raw_temp(_config)

        if _config.proc_thermal_present:
            raw_thermal_data = ' '.join(raw_thermal_data.split())

            tokens = raw_thermal_data.split(' ')
            cpu_temp = tokens[1]

            return int(cpu_temp)


        if _config.sensors_present:
            #read sensors information
            sys.exit("[!] get_current_cpu_temp with sensors support not jet implemented...")



    def get_current_gpu_temp(self, _config):

        raw_thermal_data = self._get_raw_temp(_config)

        if _config.proc_thermal_present:
            raw_thermal_data = ' '.join(raw_thermal_data.split())

            tokens = raw_thermal_data.split(' ')
            cpu_temp = tokens[4]

            return int(cpu_temp)


        if _config.sensors_present:
            #read sensors information
            sys.exit("[!] get_current_gpu_temp with sensors support not jet implemented...")

