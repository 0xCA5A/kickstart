
import Tkinter

class View():    
    
    def __init__(self, control):
        print '[i] hello from View constructor'
        
        self._control = control

        self._gui = Tkinter.Tk()
        self._gui.title('thinkpad fan control')

        self._gui.protocol("WM_DELETE_WINDOW", self._control.cleanUp)


        self._cpu_temp_label = Tkinter.Label()
        #self._cpu_temp_label.grid(row = 0, column = 0)
        self._cpu_temp_label.pack()

        self._gpu_temp_label = Tkinter.Label()
        #self._gpu_temp_label.grid(row = 0, column = 1)
        self._gpu_temp_label.pack()


        self._fan_speed_scale = Tkinter.Scale(self._gui, showvalue=0, from_=0, to=len(self._control._config.FAN_SPEED_STATE_CONFIG_VALUES)-1, orient=Tkinter.HORIZONTAL, length=200, command=self.updateFanControl)

        #set scale default value, auto
        self._fan_speed_scale.set(4)
        #self._fan_speed_scale.grid(row=2, column=0)
        self._fan_speed_scale.pack()

        self.updateView()

        
        try: 
            self._gui.mainloop()
        except:
            self._control.cleanUp()

        

    def updateFanControl(self, scaleValueString):
        _fan_speed_scale_level_string = 'fan level: %s' % self._control._config.FAN_SPEED_STATE_CONFIG_VALUES_MAPPING[int(scaleValueString)]
        self._fan_speed_scale.configure(label=_fan_speed_scale_level_string)

        self._control.updateFanLevel(self._control._config.FAN_SPEED_STATE_CONFIG_VALUES_MAPPING[int(scaleValueString)])

        
        

        
    def updateView(self):

        _current_cpu_temp = self._control._tempReader.getCurrentCPUTemp(self._control._config)
        _current_gpu_temp = self._control._tempReader.getCurrentGPUTemp(self._control._config)
        
        _current_cpu_temp_string = 'cpu: %d' % _current_cpu_temp
        _current_gpu_temp_string = 'gpu: %d' % _current_gpu_temp

        self._cpu_temp_label.configure(text=_current_cpu_temp_string)
        self._gpu_temp_label.configure(text=_current_gpu_temp_string)

        self._gui.after(self._control._config.UPDATE_INTERVAL_IN_SECONDS * 1000, self.updateView)

        

