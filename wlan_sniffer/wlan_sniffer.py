import subprocess
import logging
import abc
import queue
import time


logging.basicConfig(level=logging.DEBUG)
log = logging.getLogger(__name__)



class RawFrameSnifferInterface(object):
    __metaclass__ = abc.ABCMeta

    def __init__(self):
        self._sniffer_interface_name = None
        self._filtered_frame_queue = queue.Queue()

    @abc.abstractmethod
    def start(self):
        log.debug("abstract start implementation")
        return

    @abc.abstractmethod
    def stop(self):
        log.debug("abstract stop implementation")
        return

    @abc.abstractmethod
    def check_host_configuration(self):
        log.debug("abstract check_host_configuration implementation")
        return

    @abc.abstractmethod
    def _configure_interface(self):
        log.debug("abstract _configure_interface implementation")
        return

    def run_system_command(self, system_command):
        log.debug("run system command: %s", system_command)
        p = subprocess.Popen(system_command.split(" "), stdin=subprocess.PIPE,
                                                 stdout=subprocess.PIPE,
                                                 stderr=subprocess.PIPE)
        stdout_data, stderr_data = p.communicate()
        return p.returncode, stdout_data.decode('UTF-8').strip(), stderr_data.decode('UTF-8').strip()

class RawEthFrameSniffer(RawFrameSnifferInterface):

    def start(self):

        log.info("start %S", __name__)






class RawWlanFrameSniffer(RawFrameSnifferInterface):

    def __init__(self, wlan_phy_name, scan_freq):
        self._wlan_phy_name = wlan_phy_name
        self._sniffer_interface_name = "mon0"
        self._scan_freq = scan_freq

    def start(self):

        if not self.check_host_configuration():
            return False

        if not self._configure_interface():
            return False

        log.info("start wlan frame sniffer on interface %s", self._sniffer_interface_name)

    def check_host_configuration(self):
        log.error("TODO: check user sudo permissions for command iw")
        log.error("TODO: check user sudo permissions for command ip")
        return True

    def _configure_interface(self):

        monitor_flags = "none"

        log.info("configure wlan monitor phy %s - device name: %s, scan freq: %s, monitor flags: %s",
                 self._wlan_phy_name,
                 self._sniffer_interface_name,
                 self._scan_freq,
                 monitor_flags)

        # check if wireless phy is present
        iw_phy_command = "sudo iw phy"
        wlan_phy_list = []
        ret_code, stdout, stderr = self.run_system_command(iw_phy_command)
        for line in stdout.splitlines():
            if "wiphy" in line.lower():

                # ~ filter to get rid of set_wiphy_netns
                if "set_wiphy_netns" in line.lower():
                    continue

                wlan_phy = line.split(" ")[1].strip()
                wlan_phy_list.append(wlan_phy)
                if wlan_phy == self._wlan_phy_name:
                    break
        else:
            log.error("wlan phy %s not found (found: %s)", self._wlan_phy_name, wlan_phy_list)
            return False

        # get list with existing interfaces
        iw_if_list = []
        iw_monitor_if_check_command = "sudo iw dev"
        ret_code, stdout, stderr = self.run_system_command(iw_monitor_if_check_command)
        for line in stdout.splitlines():
            if "interface" in line.lower():
                iw_if_list.append(line.strip().split(" ")[1])

        # delete all existing interfaces configured for the device
        for iw_if in iw_if_list:

            # get phy by interface
            iw_if_info_command = "sudo iw %s info" % iw_if
            ret_code, stdout, stderr= self.run_system_command(iw_if_info_command)
            if ret_code != 0:
                log.error("error while getting information about interface %s: %s", iw_if, stderr)
                return False

            # get phy name by interface name
            # TODO(casasam): is there a better way to do this?
            phy_index = -1
            for line in stdout.splitlines():
                if "wiphy" in line:
                    phy_index = int(line.split(" ")[1].strip())

            if phy_index < 0:
                log.error("unable to find phy for interface %s", iw_if)
                return False

            if_phy_name = "phy%d" % phy_index
            if self._wlan_phy_name != if_phy_name:
                log.debug("don't touch interface %s on phy %s", iw_if, if_phy_name)
                continue

            iw_dev_if_del_command = "sudo iw dev %s interface del" % iw_if
            ret_code, _, stderr= self.run_system_command(iw_dev_if_del_command)
            if ret_code != 0:
                log.error("error while removing wlan interface %s: %s", iw_if, stderr)
                return False

        log.info("add monitor interface %s to %s (monitor flags: %s)", self._sniffer_interface_name, self._wlan_phy_name, monitor_flags)
        iw_add_monitor_if_command = "sudo iw phy %s interface add %s type monitor flags %s" % (self._wlan_phy_name,
                                                                                               self._sniffer_interface_name,
                                                                                               monitor_flags)
        ret_code, stdout, stderr = self.run_system_command(iw_add_monitor_if_command)
        if ret_code != 0:
            log.error("error while adding monitor interface: %s", stderr)
            return False


        ip_link_up_command = "sudo ip link set %s up" % self._sniffer_interface_name
        ret_code, stdout, stderr = self.run_system_command(ip_link_up_command)
        if ret_code != 0:
            log.error("error while bringing up link %s: %s", self._sniffer_interface_name, stderr)
            return False

        iw_set_monitor_freq_command = "sudo iw dev %s set freq %d" % (self._sniffer_interface_name, self._scan_freq)
        ret_code, stdout, stderr = self.run_system_command(iw_set_monitor_freq_command)
        if ret_code != 0:
            log.error("error while setting frequency %d on interface %s: %s", self._scan_freq, self._sniffer_interface_name, stderr)
            return False

        return True
    """
 2005  sudo iw phy phy0 interface add mon0
 2006  sudo iw phy phy0 interface add mon0 type monitor
 2007  sudo iw dev
 2008  sudo iw dev wlan0 interface del
 2009  sudo iw dev
 2011  sudo iw dev mon0
 2012  sudo iw dev mon0 set freq 5540
 2013  sudo iw list
 2014  sudo iw dev mon0 set freq 2472
 2015  sudo iw dev
 2018  sudo iw dev mon0 set freq 2412
    """



if __name__ == "__main__":

    my_raw_wlan_sniffer = RawWlanFrameSniffer(wlan_phy_name="phy11", scan_freq=2442)
    my_raw_wlan_sniffer.start()

    my_raw_wlan_sniffer.stop()