
import Tkinter


class View(object):

    def __init__(self, control):
        print '[i] hello from View constructor'

        self._control = control

        self._gui = Tkinter.Tk()
        self._gui.title('thinkpad fan control')

        self._gui.protocol("WM_DELETE_WINDOW", self._control.clean_up)

        self._system_load_label = Tkinter.Label()
        #self._system_load_label.grid(row = 0, column = 0)
        self._system_load_label.pack()

        self._cpu_temp_label = Tkinter.Label()
        #self._cpu_temp_label.grid(row = 0, column = 1)
        self._cpu_temp_label.pack()

        self._gpu_temp_label = Tkinter.Label()
        #self._gpu_temp_label.grid(row = 0, column = 2)
        self._gpu_temp_label.pack()


        self._fan_speed_scale = Tkinter.Scale(self._gui, showvalue=0, from_=0, to=len(self._control._config.FAN_SPEED_STATE_CONFIG_VALUES)-1, orient=Tkinter.HORIZONTAL, length=200, command=self.updateFanControl)

        #set scale default value, auto
        self._fan_speed_scale.set(4)
        #self._fan_speed_scale.grid(row=2, column=0)
        self._fan_speed_scale.pack()

        self.update_view()


        try: 
            self._gui.mainloop()
        except:
            self._control.clean_up()



    def updateFanControl(self, scaleValueString):
        _fan_speed_scale_level_string = 'fan level: %s' % self._control._config.FAN_SPEED_STATE_CONFIG_VALUES_MAPPING[int(scaleValueString)]
        self._fan_speed_scale.configure(label=_fan_speed_scale_level_string)

        self._control.update_fan_level(self._control._config.FAN_SPEED_STATE_CONFIG_VALUES_MAPPING[int(scaleValueString)])



    def update_view(self):

        _curren_sysem_load = self._control._load_reader.get_current_system_load(self._control._config)
        _current_cpu_temp = self._control._temp_reader.get_current_cpu_temp(self._control._config)
        _current_gpu_temp = self._control._temp_reader.get_current_gpu_temp(self._control._config)

        _current_system_load_string = 'system load: %s' % _curren_sysem_load
        _current_cpu_temp_string = 'cpu: %d' % _current_cpu_temp
        _current_gpu_temp_string = 'gpu: %d' % _current_gpu_temp

        self._system_load_label.configure(text=_current_system_load_string)
        self._cpu_temp_label.configure(text=_current_cpu_temp_string)
        self._gpu_temp_label.configure(text=_current_gpu_temp_string)

        self._gui.after(self._control._config.UPDATE_INTERVAL_IN_SECONDS * 1000, self.update_view)



