

class LoadReader(object):

    def __init__(self):
        print '[i] hello from LoadReader constructor'



    def _get_raw_system_load(self, _config):

        with open(_config.SYS_LOAD_INFORMATION_FILE, 'r') as f:
            raw_system_load_data = f.read()
            f.close()

        raw_system_load_data = ' '.join(raw_system_load_data.split())
        return raw_system_load_data



    def get_current_system_load(self, _config):

        raw_system_load_data = self._get_raw_system_load(_config)
        raw_system_load_data_list = raw_system_load_data.split(' ')

        if len(raw_system_load_data_list) < 3:
            system_load_data_string = "fail"
        else:
            system_load_data_string = '%s %s %s' % (raw_system_load_data_list[0], raw_system_load_data_list[1], raw_system_load_data_list[2])

        return system_load_data_string
