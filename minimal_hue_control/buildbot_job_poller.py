#!/usr/bin/env python

"""simple application to poll a buildmaster (buildbot) builder to show its state on a philips hue light

Usage:
    buildmaster_builder_poller.py [--config=PATH.JSON] [--log=LEVEL]
    buildmaster_builder_poller.py --list-colors [--log=LEVEL]

Options:
    --config=PATH.JSON          json configuration file
    --log=LEVEL                 set log level
    --list-colors               list supported colors
"""


import json
import docopt
import logging
import pprint
import signal
import sys
import time
import minimal_hue_control
import pycurl
import cStringIO

# configure module logger, default log level is configured to info
logging.basicConfig()
logger = logging.getLogger()
logger.setLevel(logging.INFO)



class BuildMasterBuildJobState(object):
    """a super cheap, printable enum
    """
    COMPILE_FAILED = "compile_failed"
    TEST_FAILED = "test_failed"
    UNKNOWN_FAILED = "unknown_failed"
    SUCCESS = "success"

class BuildMasterBuilderState(object):
    """a super cheap, printable enum

    this string come from the buildbot api

    """
    IDLE = "idle"
    BUILDING = "building"



class BuildMasterBuilderPoller(object):

    DEFAULT_CONFIG_FILE_PATH = "config.json"
    DEFAULT_POLL_INTERVAL_IN_SEC = 4

    GLOBAL_RUN_FLAG = True

    def __init__(self, _json_config_file_path=DEFAULT_CONFIG_FILE_PATH):
        logger.info("create %s object", self.__class__.__name__)

        self._hue_bridge_address = None
        self._hue_bridge_user = None
        self._hue_light_name = None
        self._buildmaster_address = None
        self._buildmaster_builder = None

        self._color_building = None
        self._color_success = None
        self._color_build_failed = None
        self._color_test_failed = None
        self._color_unknown_failed = None

        self._my_mmhc = None

        self._cached_buildmaster_builder_state = None

        self._poller_interval_in_sec = self.DEFAULT_POLL_INTERVAL_IN_SEC


    def read_config_file(self, _json_config_file_path=DEFAULT_CONFIG_FILE_PATH):
        logging.info("read configuration file %s", _json_config_file_path)
        with open(_json_config_file_path) as json_config_file:
            config_data = json.load(json_config_file)

        if "poller" in config_data:
            if "interval_in_sec" in config_data["poller"]:
                self._poller_interval_in_sec = config_data["poller"]["interval_in_sec"]


        if "hue" in config_data:
            if "address" in config_data["hue"]:
                self._hue_bridge_address = config_data["hue"]["address"]
            if "user" in config_data["hue"]:
                self._hue_bridge_user = config_data["hue"]["user"]
            if "light_name" in config_data["hue"]:
                self._hue_light_name = config_data["hue"]["light_name"]

        if "buildmaster" in config_data:
            if "address" in config_data["buildmaster"]:
                self._buildmaster_address = config_data["buildmaster"]["address"]
            if "builder" in config_data["buildmaster"]:
                self._buildmaster_builder = config_data["buildmaster"]["builder"]

        if "color_state_matcher" in config_data:
            if "building" in config_data["color_state_matcher"]:
                self._color_building = config_data["color_state_matcher"]["building"]
            if "success" in config_data["color_state_matcher"]:
                self._color_success = config_data["color_state_matcher"]["success"]
            if "build_failed" in config_data["color_state_matcher"]:
                self._color_build_failed = config_data["color_state_matcher"]["build_failed"]
            if "test_failed" in config_data["color_state_matcher"]:
                self._color_test_failed = config_data["color_state_matcher"]["test_failed"]
            if "unknown_failed" in config_data["color_state_matcher"]:
                self._color_unknown_failed = config_data["color_state_matcher"]["unknown_failed"]

    def setup_objects(self):
        self._my_minimal_hue_control = minimal_hue_control.MinimalHUEControl(_bridge_address=self._hue_bridge_address, _user_name=self._hue_bridge_user)

    def _update_hue_light(self, _light_color):
        logger.info("update hue light, new color is %s",  _light_color)
        self._my_minimal_hue_control.set_light_color(_light_name=self._hue_light_name, _color=_light_color)

    def _get_buildmaster_builder_state(self):

        # curl -X GET -H "Content-type: application/json"  https://openwrt.neratec.com/buildbot/json/builders/DT50_Alstom/ | python -m json.tool

        buf = cStringIO.StringIO()

        print self._buildmaster_address + "/json/builders/" + self._buildmaster_builder

        c = pycurl.Curl()
        c.setopt(c.URL, self._buildmaster_address + "/json/builders/" + self._buildmaster_builder)
        c.setopt(c.WRITEFUNCTION, buf.write)
        c.perform()
        json_data = json.loads(buf.getvalue())
        buf.close()

        return json_data["state"]

    def _get_last_cached_build(self):

        # curl -X GET -H "Content-type: application/json"  https://openwrt.neratec.com/buildbot/json/builders/DT50_Alstom/ | python -m json.tool

        buf = cStringIO.StringIO()
        url = self._buildmaster_address + "/json/builders/" + self._buildmaster_builder
        c = pycurl.Curl()
        c.setopt(c.URL, url)
        c.setopt(c.WRITEFUNCTION, buf.write)
        c.perform()
        json_data = json.loads(buf.getvalue())
        buf.close()

        # return biggest number in list
        cached_builds = json_data["cachedBuilds"]
        return max(map(int, cached_builds))




    def _get_last_cached_build_info(self, last_cached_build_id):

        # curl -X GET -H "Content-type: application/json" https://openwrt.neratec.com/buildbot/json/builders/DT50_Alstom/builds/354 | python -m json.tool

        buf = cStringIO.StringIO()

        url = self._buildmaster_address + "/json/builders/" + self._buildmaster_builder + "/builds/" + str(last_cached_build_id)

        c = pycurl.Curl()
        c.setopt(c.URL, url)
        c.setopt(c.WRITEFUNCTION, buf.write)
        c.perform()
        json_data = json.loads(buf.getvalue())
        buf.close()

        return json_data

    def _get_state_from_build_info(self, _build_info):
        # pprint.pprint(_build_info)

        # print _build_info["text"]
        # pprint.pprint(_build_info)
        #
        # print _build_info.keys()



        if "failed" in _build_info["text"]:
            logger.info("failed build detected")

            # why did it fail?
            for step in _build_info["steps"]:

                # we assume that the build stops after a compile fail
                if 2 in step["results"] and "make" in step["name"]:
                    return BuildMasterBuildJobState.COMPILE_FAILED

                # check for failing tests


            # default fail state
            return BuildMasterBuildJobState.UNKNOWN_FAIL

        else:
            logger.info("successful build detected")
            return BuildMasterBuildJobState.SUCCESS



    def poll_buildmaster_builder(self):
        """

        :return:
        """

        logger.info("start buildmaster poller, poll interval is %ds" % (self._poller_interval_in_sec))
        while self.GLOBAL_RUN_FLAG:

            current_buildmaster_builder_state = self._get_buildmaster_builder_state()

            if current_buildmaster_builder_state != self._cached_buildmaster_builder_state:

                logger.info("buildmaster builder state changed from %s to %s" % (self._cached_buildmaster_builder_state, current_buildmaster_builder_state))

                if current_buildmaster_builder_state == BuildMasterBuilderState.BUILDING:
                    new_color = self._get_color_by_state(current_buildmaster_builder_state)
                    self._update_hue_light(new_color)

                # if we switch to idle we have to check the state
                if current_buildmaster_builder_state == BuildMasterBuilderState.IDLE:
                    last_cached_build_id = self._get_last_cached_build()
                    build_info = self._get_last_cached_build_info(last_cached_build_id)
                    state = self._get_state_from_build_info(build_info)

                    new_color = self._get_color_by_state(state)
                    print new_color
                    if new_color:
                        self._update_hue_light(new_color)


                self._cached_buildmaster_builder_state = current_buildmaster_builder_state


            time.sleep(self._poller_interval_in_sec)

    def _get_color_by_state(self, _buildmaster_builder_state):

        if _buildmaster_builder_state == BuildMasterBuildJobState.COMPILE_FAILED:
            return self._color_build_failed
        if _buildmaster_builder_state == BuildMasterBuildJobState.SUCCESS:
            return self._color_success
        if _buildmaster_builder_state == BuildMasterBuildJobState.TEST_FAILED:
            return self._color_test_failed
        if _buildmaster_builder_state == BuildMasterBuildJobState.UNKNOWN_FAILED:
            return self._color_unknown_failed

        if _buildmaster_builder_state == BuildMasterBuilderState.BUILDING:
            return self._color_building

        # idle has no color
        if _buildmaster_builder_state == BuildMasterBuilderState.IDLE:
            return None



    @classmethod
    def signal_handler(cls, signum, frame):
        logger.info("got signal, lets exit")
        BuildMasterBuilderPoller.GLOBAL_RUN_FLAG = False



def _main(_cli_arguments):
    """main function to handle command line arguments"""

    # set the log level from the command line argument, not set if not valid
    # valid log levels (from python logging module):
    #   'CRITICAL', 'ERROR', 'WARNING', 'INFO', 'DEBUG'
    _log_level = _cli_arguments['--log']
    if _log_level and hasattr(logging, _log_level):
        logger.setLevel(_log_level)

    _list_supported_colors = _cli_arguments['--list-colors']
    if _list_supported_colors:
        minimal_hue_control.MinimalHUEControl.list_supported_colors()
        sys.exit(0)

    _config_file_path = BuildMasterBuilderPoller.DEFAULT_CONFIG_FILE_PATH
    if _cli_arguments['--config'] is not None:
        _config_file_path = _cli_arguments['--config']

    BBMBP = BuildMasterBuilderPoller()
    BBMBP.read_config_file(_config_file_path)
    BBMBP.setup_objects()
    BBMBP.poll_buildmaster_builder()



if __name__ == "__main__":
    signal.signal(signal.SIGINT, BuildMasterBuilderPoller.signal_handler)
    try:
        # parse cli arguments, use file docstring as a parameter definition
        CLI_ARGUMENTS = docopt.docopt(__doc__)

    # handle invalid options
    except docopt.DocoptExit as exception:
        logger.error(exception.message)
        sys.exit(1)

    _main(CLI_ARGUMENTS)
