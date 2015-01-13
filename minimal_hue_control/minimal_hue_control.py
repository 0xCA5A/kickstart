#!/usr/bin/env python

"""simple application to easily change the light color from the command line
for further information about philips hue, check here:
    * https://github.com/Q42/hue-libs
    * http://www.developers.meethue.com/documentation/core-concepts
    * http://www.developers.meethue.com/documentation/getting-started

Usage:
    minimal_hue_control.py --light=NAME (--set-color=COLOR | --turn-off | --test) [--user=USERNAME] [--address=ADDRESS] [--log=LEVEL]
    minimal_hue_control.py --register --user=USERNAME [--address=ADDRESS] [--log=LEVEL]
    minimal_hue_control.py (--list-lights | --search) [--user=USERNAME] [--address=ADDRESS] [--log=LEVEL]
    minimal_hue_control.py --list-colors [--log=LEVEL]

Options:
    --light=NAME                light name
    --set-color=COLOR           set color and turn light on
    --turn-off                  turn light off
    --test                      test mode to show all the configured colors
    --user=USERNAME             application identifier / username (register first at bridge!)
    --address=ADDRESS           HUE bridge address
    --log=LEVEL                 set module log level
    --register                  register application at HUE bridge (necessary only once)
    --search                    search for new lights
    --list-lights               list connected lights
    --list-colors               list supported colors
"""


import docopt
import logging
import sys
import collections
import time

from phue import phue

# configure module logger, default log level is configured to info
logging.basicConfig()
logger = logging.getLogger()
logger.setLevel(logging.INFO)


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
        MinimalHUEControl.list_supported_colors()
        _exit_gracefully()

    _bridge_address = MinimalHUEControl.DEFAULT_HUE_BRIDGE_ADDRESS
    if _cli_arguments['--address'] is not None:
        _bridge_address = _cli_arguments['--address']

    _user_name = MinimalHUEControl.DEFAULT_HUE_USERNAME
    if _cli_arguments['--user'] is not None:
        _user_name = _cli_arguments['--user']

    minimal_hue_control = MinimalHUEControl(_bridge_address=_bridge_address, _user_name=_user_name)

    _list_known_lights = _cli_arguments['--list-lights']
    if _list_known_lights:
        minimal_hue_control.list_known_lights()
        _exit_gracefully()

    _search_lights = _cli_arguments['--search']
    if _search_lights:
        minimal_hue_control.search_for_lights()
        _exit_gracefully()

    _register_username = _cli_arguments['--register']
    if _register_username:
        minimal_hue_control.register_username()
        _exit_gracefully()

    _light_name = _cli_arguments['--light']

    _turn_off_light = _cli_arguments['--turn-off']
    if _turn_off_light:
        minimal_hue_control.turn_off_light(_light_name)
        _exit_gracefully()

    _set_color = _cli_arguments['--set-color']
    if _set_color:
        minimal_hue_control.set_light_color(_light_name, _set_color)
        _exit_gracefully()

    _test = _cli_arguments['--test']
    if _test:
        minimal_hue_control.test_colors(_light_name)
        _exit_gracefully()


def _exit_gracefully():
    """helper function to log am message and exit the application"""
    logger.info('exit gracefully')
    sys.exit(0)


def _exit_failure():
    """helper function to log am message and exit the application"""
    logger.error('exit failure')
    sys.exit(1)


class MinimalHUEControl(object):
    """simple interface to control a philips HUE from the command line

    this implementation uses the phue python module to handle the philips HUE REST API calls
        https://github.com/studioimaginaire/phue

    there are a lot of libs available, check here for more detail:
        https://github.com/Q42/hue-libs
    """

    DEFAULT_HUE_BRIDGE_ADDRESS = '192.168.1.123'
    DEFAULT_LOG_LEVEL = logging.INFO
    DEFAULT_HUE_USERNAME = 'defautluser'
    DEFAULT_COLOR_TEST_SLEEP_TIME_IN_SEC = 2
    DEFAULT_BRIGHTNESS_MAX = 254

    Color = collections.namedtuple('Color', 'name, rgb')

    # define and register the supported colors in RGB representation
    colors = [
        Color('WHITE', (255, 255, 255)),
        Color('RED', (255, 0, 0)),
        Color('GREEN', (0, 255, 0)),
        Color('BLUE', (0, 0, 255)),
        Color('CYAN', (0, 255, 255)),
        Color('MAGENTA', (255, 0, 255)),
        Color('YELLOW', (255, 255, 0))
    ]

    def __init__(self,
                 _bridge_address=DEFAULT_HUE_BRIDGE_ADDRESS,
                 _user_name=DEFAULT_HUE_USERNAME):

        self._bridge_address = _bridge_address
        # NOTE(casasam): does it make sense from the API just to have one user per bridge instance?
        self._hue_bridge = phue.Bridge(_bridge_address, _user_name)

    def check_username_decorator(func):
        """ function decorator to check username

        in good case: the call returns a dict (json)

        in error case: the call returns a list with a dict (json)
        we look for something like this:
            [{u'error': {u'type': 1, u'description': u'unauthorized user', u'address': u'/'}}]

        :param func:
        :return:
        """
        def func_wrapper(self, *args, **kwargs):
            logger.debug("in check username function wrapper")
            response = self._hue_bridge.request('GET', '/api/' + self._hue_bridge.username)

            if isinstance(response, list):
                logger.error(response[0]['error']['description'])
                _exit_failure()

            # call wrapped function
            func(self, *args, **kwargs)

        return func_wrapper

    def check_light_decorator(func):
        """ function decorator to check if light is known

        in good case: the call returns a dict (json)

        in error case: the call returns a list with a dict (json)
        we look for something like this:
            [{u'error': {u'type': 3, u'description': u'resource, /lights/12, not available', u'address': u'/lights/12'}}]

        :param func:
        :return:
        """
        def func_wrapper(self, *args, **kwargs):
            logger.debug("in check light function wrapper")

            # expect light name in first argument
            _UNDEFINED_NAME_PATTERN = "dummy_undefined_pattern_string"
            light_name = _UNDEFINED_NAME_PATTERN
            if len(args) > 0:
                light_name = args[0]

            # get all the lights from the bridge (raw)
            response = self._hue_bridge.request('GET', '/api/' + self._hue_bridge.username + '/lights/')

            # catch any kind of error
            if isinstance(response, list):
                logger.error(response[0]['error']['description'])
                _exit_failure()

            if light_name is not _UNDEFINED_NAME_PATTERN:
                # try to find the light by name
                for key in response:
                    if response[key]["name"] == light_name:
                        break
                else:
                    logger.error("light named '%s' not found", light_name)
                    _exit_failure()

            # call wrapped function
            func(self, *args, **kwargs)

        return func_wrapper

    @staticmethod
    def list_supported_colors():
        """ function to list the internally supported colors"""
        logger.info("list supported colors")
        for color in MinimalHUEControl.colors:
            logger.info("  * %s", repr(color))

    def test_colors(self, _light_name):
        """test function to test all the known color patterns

        :param _light_name: light name
        :return: None
        """

        logger.info("show supported colors on light %s", _light_name)
        for color in MinimalHUEControl.colors:
            self.set_light_color(_light_name, color[0])
            time.sleep(self.DEFAULT_COLOR_TEST_SLEEP_TIME_IN_SEC)

    @check_username_decorator
    @check_light_decorator
    def set_light_color(self, _light_name, _color):
        """function to set a color by name

        :param _light_name: light name
        :param _color: color name as a string
        :return: None
        """

        logger.info("set color %s on light %s", _color, _light_name)
        light = self._hue_bridge.get_light_objects(mode='name')[_light_name]

        rgb_color_tuple = [color for color in self.colors if color[0] == _color][0][1]
        x, y = self._rgb_to_xy(rgb_color_tuple[0], rgb_color_tuple[1], rgb_color_tuple[2])
        light.xy = [x, y]
        light.brightness = self.DEFAULT_BRIGHTNESS_MAX
        light.on = True

    @staticmethod
    def _rgb_to_xy(_red, _green, _blue):
        """function to convert rgb color values to x y values

        :param _red: red color value, 0 - 255
        :param _green: green color value, 0 - 255
        :param _blue: blue color value, 0 - 255
        :return:
        """

        # color gets more complicated...
        # http://www.developers.meethue.com/documentation/core-concepts

        # check this page to understand the HSV / HSB color model
        # http://colorizer.org/

        # check here for further information about this color conversion:
        #   https://github.com/PhilipsHue/PhilipsHueSDK-iOS-OSX/blob/master/ApplicationDesignNotes/RGB%20to%20xy%20Color%20conversion.md

        # scale rgb values, normalize to one
        red = _red / 255.0
        green = _green / 255.0
        blue = _blue / 255.0

        # gamma correction
        gamma_correction = lambda scaled_color_value: pow((scaled_color_value + 0.055) / (1.0 + 0.055), 2.4) if (scaled_color_value > 0.04045) else (scaled_color_value / 12.92)
        red = gamma_correction(red)
        green = gamma_correction(green)
        blue = gamma_correction(blue)

        # NOTE(casasam): the resulting colors with this matrix look strange for me
        # convert the RGB values to XYZ using the Wide RGB D65 conversion formula
        # X = red * 0.649926 + green * 0.103455 + blue * 0.197109
        # Y = red * 0.234327 + green * 0.743075 + blue * 0.022598
        # Z = red * 0.000000 + green * 0.053077 + blue * 1.035763

        # NOTE(casasam): this parameter from here look better for me, source:
        #   http://de.wikipedia.org/wiki/CIE-Normvalenzsystem
        #   http://en.wikipedia.org/wiki/CIE_1931_color_space
        X = red * 0.4124 + green * 0.3576 + blue * 0.1805
        Y = red * 0.2126 + green * 0.7152 + blue * 0.0722
        Z = red * 0.0193 + green * 0.1192 + blue * 0.9505

        if X + Y + Z == 0:
            return 0, 0

        # calculate the xy values from the XYZ values
        x = X / (X + Y + Z)
        y = Y / (X + Y + Z)

        logger.debug("converted rgb value %s to x: %f and y: %f", repr((_red, _green, _blue)), x, y)
        return x, y

    def register_username(self):
        """function to register a username

        this is used as security concept.
        you have to register a username first (while pressing the button on the bridge), to access the bridge

        :return:
        """
        logger.info("register username on bridge")
        # self._hue_bridge.connect()
        logger.error("[TODO] LIBRARY FUNCTION CALL NOT IMPLEMENTED, USE THE CLIP API DEBUGGER (debug/clip.html)")

    @check_username_decorator
    @check_light_decorator
    def turn_off_light(self, _light_name):
        """library call to disable a light

        :param _light_name: light name
        :return: None
        """
        logger.info("turn off light %s", _light_name)
        lights_by_name = self._hue_bridge.get_light_objects(mode='name')
        lights_by_name[_light_name].on = False

    @check_username_decorator
    def list_known_lights(self):
        """library call to print a list of the connected lights (by name)"""
        logger.info("lights known by the HUE bridge")
        lights_by_name = self._hue_bridge.get_light_objects(mode='name')
        logger.info(lights_by_name.keys())
        logger.debug(repr(lights_by_name))

    @check_username_decorator
    def search_for_lights(self):
        """function to search for new lights / update the list of known lights

        use this function to scan for new HUE lights

        :return: None
        """
        logger.info("search for lights")
        self._hue_bridge.request('POST', '/api/' + self._hue_bridge.username + '/lights')


if __name__ == "__main__":

    try:
        # parse cli arguments, use file docstring as a parameter definition
        CLI_ARGUMENTS = docopt.docopt(__doc__)

    # handle invalid options
    except docopt.DocoptExit as exception:
        logger.error(exception.message)
        sys.exit(1)

    _main(CLI_ARGUMENTS)
