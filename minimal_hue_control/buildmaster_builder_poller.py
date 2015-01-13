#!/usr/bin/env python

"""simple application to poll a buildmaster (buildbot) builder to show its state on a philips hue light

get more information about buildbot and hue:
    * http://buildbot.net/
    * http://www.developers.meethue.com/documentation/core-concepts

Usage:
    buildmaster_builder_poller.py [--config=PATH.JSON] [--log=LEVEL]
    buildmaster_builder_poller.py --list-colors [--log=LEVEL]

Options:
    --config=PATH.JSON          json configuration file
    --log=LEVEL                 set log level
    --list-colors               list supported colors
"""

# pylint: disable=C0301

import json
import docopt
import logging
import sys
import time
import minimal_hue_control
import pycurl
import cStringIO


class BuildMasterBuilderStepState(object):
    """a super cheap, printable enum representing buildmaster builder step states
    """
    COMPILE_FAILED = "compile_failed"
    TEST_FAILED = "test_failed"
    UNKNOWN_FAILED = "unknown_failed"
    SUCCESS = "success"

class BuildMasterBuilderState(object):
    """a super cheap, printable enum representing buildmaster builder states (this strings come from the buildbot api)
    """
    IDLE = "idle"
    BUILDING = "building"

class BuildMasterBuilderPoller(object):
    """implementation of a buildmaster builder poller
    """

    DEFAULT_CONFIG_FILE_PATH = "config.json"
    DEFAULT_POLL_INTERVAL_IN_SEC = 4
    GLOBAL_RUN_FLAG = True

    def __init__(self):
        logging.info("create %s object", self.__class__.__name__)

        self._hue_bridge_address = None
        self._hue_bridge_user = None
        self._hue_light_name = None
        self._buildmaster_address = None
        self._buildmaster_builder = None

        self._color_building = "RED"
        self._color_success = "GREEN"
        self._color_compile_failed = "BLUE"
        self._color_test_failed = "WHITE"
        self._color_unknown_failed = "YELLOW"

        self._my_mmhc = None
        self._my_minimal_hue_control = None
        self._cached_buildmaster_builder_state = None
        self._poller_interval_in_sec = self.DEFAULT_POLL_INTERVAL_IN_SEC
        self._poller_build_fail_string = None
        self._poller_test_fail_string = None

    def log_current_configuration(self):
        """function to log the current configuration

        :return: None
        """
        logging.info("current configuration")
        interesting_class_variable_prefix = ("_hue", "_buildmaster", "_color", "_poller")
        for key in sorted(self.__dict__.keys()):
            if key.startswith(interesting_class_variable_prefix):
                logging.info(" * %s: %s", key, self.__dict__[key])

    def read_config_file(self, _json_config_file_path=DEFAULT_CONFIG_FILE_PATH):
        """function to read JSON configuration file

        :param _json_config_file_path:
        :return:
        """

        logging.info("read configuration file %s", _json_config_file_path)
        with open(_json_config_file_path) as json_config_file:
            config_data = json.load(json_config_file)

        if "poller" in config_data:
            if "interval_in_sec" in config_data["poller"]:
                self._poller_interval_in_sec = config_data["poller"]["interval_in_sec"]
            if "build_fail_string" in config_data["poller"]:
                self._poller_build_fail_string = config_data["poller"]["build_fail_string"]
            if "test_fail_string" in config_data["poller"]:
                self._poller_test_fail_string = config_data["poller"]["test_fail_string"]

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
            if "compile_failed" in config_data["color_state_matcher"]:
                self._color_compile_failed = config_data["color_state_matcher"]["compile_failed"]
            if "test_failed" in config_data["color_state_matcher"]:
                self._color_test_failed = config_data["color_state_matcher"]["test_failed"]
            if "unknown_failed" in config_data["color_state_matcher"]:
                self._color_unknown_failed = config_data["color_state_matcher"]["unknown_failed"]

    def setup_objects(self):
        """helper function to setup objects

        :return:
        """
        self._my_minimal_hue_control = minimal_hue_control.MinimalHUEControl(_bridge_address=self._hue_bridge_address, _user_name=self._hue_bridge_user)

    def _update_hue_light(self, _light_color):
        """wrapper function to set light color

        :param _light_color: string
        :return:
        """
        logging.info("update hue light, new color is %s", _light_color)
        self._my_minimal_hue_control.set_light_color(_light_name=self._hue_light_name, _color=_light_color)

    def _get_buildmaster_builder_data(self):
        """function to get builder data from the buildmaster

        try this command to analyze the JSON data from the buildmaster
            curl -X GET -H "Content-type: application/json"  https://openwrt.neratec.com/buildbot/json/builders/YOURBUILDER/ | python -m json.tool

        :return: JSON data
        """

        logging.debug("get builder data from buildmaster")

        buf = cStringIO.StringIO()
        url = self._buildmaster_address + "/json/builders/" + self._buildmaster_builder
        curl = pycurl.Curl()
        curl.setopt(curl.URL, url)
        curl.setopt(curl.WRITEFUNCTION, buf.write)
        curl.perform()
        build_master_builder_data_json = json.loads(buf.getvalue())
        buf.close()

        return build_master_builder_data_json

    def _get_buildmaster_builder_state(self):
        """get state from buildmaster builder data

        :return:
        """
        build_master_builder_data_json = self._get_buildmaster_builder_data()
        return build_master_builder_data_json["state"]

    def _get_last_cached_build_id(self):
        """get last cached build id

        :return:
        """
        build_master_builder_data_json = self._get_buildmaster_builder_data()

        # return highest number in list
        cached_builds = build_master_builder_data_json["cachedBuilds"]
        last_cached_build_id = max(cached_builds)

        return last_cached_build_id

    def _get_last_cached_build_info(self, last_cached_build_id):
        """function to get information about latest cached builder build on buildmaster by id

        try this command to analyze the JSON data from the buildmaster
            curl -X GET -H "Content-type: application/json" https://openwrt.neratec.com/buildbot/json/builders/YOURBUILDER/42 | python -m json.tool

        :param last_cached_build_id: build id
        :return: JSON data, build information
        """

        buf = cStringIO.StringIO()
        url = self._buildmaster_address + "/json/builders/" + self._buildmaster_builder + "/builds/" + str(last_cached_build_id)
        curl = pycurl.Curl()
        curl.setopt(curl.URL, url)
        curl.setopt(curl.WRITEFUNCTION, buf.write)
        curl.perform()
        json_data = json.loads(buf.getvalue())
        buf.close()

        return json_data

    def _get_builder_build_state_from_build_info(self, _build_info):
        """parse build info to get build state

        here we try to find the strings in self._poller_test_fail_string and self._poller_build_fail_string in the
        failed test steps to find out what failed, tests or build.

        user curl on the command line to analyze the data:
            curl -X GET -H "Content-type: application/json" https://openwrt.neratec.com/buildbot/json/builders/YOURBUILDER/builds/42 | python -m json.tool | less

        :param _build_info: json data describing buildbot steps
        :return: BuildMasterBuilderStepState field
        """

        # did the builder fail?
        if "failed" in _build_info["text"]:
            logging.debug("builder fail detected")

            # why did it fail? tests or build or something unknown
            for step in _build_info["steps"]:

                # skip the successful completed steps
                if "failed" not in step["text"]:
                    continue

                logging.debug("first failing step: %s", step["text"])
                logging.debug("full failing step dump: %s", step)

                # detect failing build step
                if self._poller_build_fail_string in step["text"]:
                    return BuildMasterBuilderStepState.COMPILE_FAILED

                # detect failing test steps
                # NOTE: strip ' here, its part of the first token
                if self._poller_test_fail_string in [element.strip("'") for element in step["text"]]:
                    return BuildMasterBuilderStepState.TEST_FAILED

            # default fail, show unknown fail pattern
            return BuildMasterBuilderStepState.UNKNOWN_FAILED

        else:
            logging.info("successful build detected")
            return BuildMasterBuilderStepState.SUCCESS

    def poll_buildmaster_builder(self):
        """ main while loop

        function checks current build state and adjusts hue light color accordant to the configuration

        :return: should never return
        """

        logging.info("start buildmaster poller, poll interval is %ds", self._poller_interval_in_sec)
        while self.GLOBAL_RUN_FLAG:

            current_buildmaster_builder_state = self._get_buildmaster_builder_state()

            # detect change in buildmaster buidler state (None, IDLE, BUILDING)
            if current_buildmaster_builder_state != self._cached_buildmaster_builder_state:

                logging.info("buildmaster builder state changed from %s to %s", self._cached_buildmaster_builder_state, current_buildmaster_builder_state)

                if current_buildmaster_builder_state == BuildMasterBuilderState.BUILDING:
                    new_color = self._get_color_by_state(current_buildmaster_builder_state)
                    self._update_hue_light(new_color)

                # if we switch to idle we have to check the state
                if current_buildmaster_builder_state == BuildMasterBuilderState.IDLE:
                    last_cached_build_id = self._get_last_cached_build_id()

                    build_info = self._get_last_cached_build_info(last_cached_build_id)
                    current_builder_build_state = self._get_builder_build_state_from_build_info(build_info)
                    logging.info("last cached buildmaster builder build state is %s (build id: %d)", current_builder_build_state, last_cached_build_id)

                    new_color = self._get_color_by_state(current_builder_build_state)
                    if new_color:
                        self._update_hue_light(new_color)

                self._cached_buildmaster_builder_state = current_buildmaster_builder_state

            time.sleep(self._poller_interval_in_sec)

    def _get_color_by_state(self, _buildmaster_builder_state):
        """function to transform state to color information

        :param _buildmaster_builder_state:
        :return:
        """

        if _buildmaster_builder_state == BuildMasterBuilderStepState.COMPILE_FAILED:
            return self._color_compile_failed
        if _buildmaster_builder_state == BuildMasterBuilderStepState.SUCCESS:
            return self._color_success
        if _buildmaster_builder_state == BuildMasterBuilderStepState.TEST_FAILED:
            return self._color_test_failed
        if _buildmaster_builder_state == BuildMasterBuilderStepState.UNKNOWN_FAILED:
            return self._color_unknown_failed

        if _buildmaster_builder_state == BuildMasterBuilderState.BUILDING:
            return self._color_building

        # idle has no color, keep current color
        if _buildmaster_builder_state == BuildMasterBuilderState.IDLE:
            return None

def _main(_cli_arguments):
    """main function to handle command line arguments
    """


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

    bmbp = BuildMasterBuilderPoller()
    bmbp.read_config_file(_config_file_path)
    bmbp.log_current_configuration()

    bmbp.setup_objects()
    bmbp.poll_buildmaster_builder()

if __name__ == "__main__":

    # setup logger
    logger = logging.getLogger()
    logger.handlers = []
    logging.basicConfig(
            level=logging.INFO,
            format="[%(asctime)s] %(levelname)s [%(name)s.%(funcName)s:%(lineno)d] %(message)s",
            datefmt=None, stream=sys.stdout)
    logger.setLevel(logging.INFO)

    try:
        # parse cli arguments, use file docstring as a parameter definition
        CLI_ARGUMENTS = docopt.docopt(__doc__)
        _main(CLI_ARGUMENTS)

    # handle invalid options
    except docopt.DocoptExit as exception:
        logger.error(exception.message)
        sys.exit(1)

    # handle ctrl c
    except KeyboardInterrupt as exception:
        logger.info("^C received, exit application")
