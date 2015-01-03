#!/usr/bin/env python

"""simple application to easily change the light color from the command line

TODO(casasam): write which lib we wrap and where this guy comes from

https://github.com/Q42/hue-libs


Usage:
    minimal_hue_control.py --light=NAME (--set-color=COLOR | --turn-off) [--address=ADDRESS] [--log=LEVEL]
    minimal_hue_control.py --connect [--address=ADDRESS] [--log=LEVEL]
    minimal_hue_control.py --list-lights [--address=ADDRESS] [--log=LEVEL]
    minimal_hue_control.py --list-colors [--log=LEVEL]

Options:
    --light=NAME                light name
    --set-color=COLOR           set color and turn light on
    --turn-off                  turn light off
    --address=ADDRESS           HUE bridge address
    --log=LEVEL                 set module log level
    --connect                   register application at HUE bridge (necessary only once)
    --list-lights               list connected lights
    --list-colors               list supported colors
"""


import docopt
import logging
import sys
import collections

import phue


# configure module logger, default log level is configured to info
logging.basicConfig()
logger = logging.getLogger()
logger.setLevel(logging.DEBUG)


def _main(_cli_arguments):

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

    _bridge_address = MinimalHUEControl.DEFAULT_HUE_BRIDGE_ADDRESS_STRING
    if _cli_arguments['--address'] is not None:
        _bridge_address = _cli_arguments['--address']

    minimal_hue_control = MinimalHUEControl(_bridge_address=_bridge_address)

    _list_known_lights = _cli_arguments['--list-lights']
    if _list_known_lights:
        minimal_hue_control.list_known_lights()
        _exit_gracefully()

    _connect_application = _cli_arguments['--connect']
    if _connect_application:
        minimal_hue_control.connect_application()
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


def _exit_gracefully():
    logger.info('exit gracefully')
    sys.exit(0)


class MinimalHUEControl(object):
    """simple interface to control a philips HUE from the command line

    this implementation uses the phue module to handle the HUE REST API calls
        https://github.com/studioimaginaire/phue

    there are a lot of libs available, check here for more detail:
        https://github.com/Q42/hue-libs
    """

    DEFAULT_HUE_BRIDGE_ADDRESS_STRING = '192.168.1.123'
    DEFAULT_LOG_LEVEL = logging.INFO
    DEFAULT_HUE_USERNAME = 'defaultuser'

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

    @staticmethod
    def list_supported_colors():
        """ function to list the internally supported colors"""
        logger.info("INFO list supported colors")
        for color in MinimalHUEControl.colors:
            logger.info("  * %s" % repr(color))

    def __init__(self,
                 _bridge_address=DEFAULT_HUE_BRIDGE_ADDRESS_STRING,
                 _user_name=DEFAULT_HUE_USERNAME):

        self._bridge_address = _bridge_address
        self._hue_bridge = phue.Bridge(_bridge_address, _user_name)

    def set_light_color(self, _light_name, _color):
        """function to set a color by name

        :param _light_name: light name
        :param _color: color name as a string
        :return:
        """

        logger.info("set light color %s on light %s" % (_color, _light_name))
        light = self._hue_bridge.get_light_objects(mode='name')[_light_name]
        light.on = True

        rgb_color_tuple = [color for color in self.colors if color[0] == _color][0][1]
        x, y = self._rgb_to_xy(rgb_color_tuple[0], rgb_color_tuple[1], rgb_color_tuple[2])
        light.xy = [x, y]

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

        # convert the RGB values to XYZ using the Wide RGB D65 conversion formula
        X = red * 0.649926 + green * 0.103455 + blue * 0.197109
        Y = red * 0.234327 + green * 0.743075 + blue * 0.022598
        Z = red * 0.000000 + green * 0.053077 + blue * 1.035763

        # TODO: check this matrix parameter from here:
        #   http://de.wikipedia.org/wiki/CIE-Normvalenzsystem
        # X = 0.4124 * red + 0.3576 * green + 0.1805 * blue
        # Y = 0.2126 * red + 0.7152 * green + 0.0722 * blue
        # Z = 0.0193 * red + 0.1192 * green + 0.9505 * blue

        if X + Y + Z == 0:
            return 0,0

        # calculate the xy values from the XYZ values
        x = X / (X + Y + Z)
        y = Y / (X + Y + Z)

        logger.debug("converted rgb value %s to x: %f and y: %f" % (repr((_red, _green, _blue)), x, y))
        return x, y

    def connect_application(self):
        logger.info("connect application to bridge")
        # self._hue_bridge.connect()
        # logger.error('Failed to open file', exc_info=True)
        print "[TODO] add lib call"

    def turn_off_light(self, _light_name):
        """library call to disable a light

        :param _light_name: light name
        :return: None
        """
        logger.info("turn off light %s" % _light_name)
        lights_by_name = self._hue_bridge.get_light_objects(mode='name')
        lights_by_name[_light_name].on = False

    def list_known_lights(self):
        """library call to get a list of the connected lights

        :return:
        """
        logger.info("list the lights known by the HUE bridge")
        lights_by_name = self._hue_bridge.get_light_objects(mode='name')
        logger.info(lights_by_name.keys())

if __name__ == "__main__":

    try:
        # parse cli arguments, use file docstring as a parameter definition
        cli_arguments = docopt.docopt(__doc__)

    # handle invalid options
    except docopt.DocoptExit as e:
        logger.error(e.message)
        sys.exit(1)

    _main(cli_arguments)