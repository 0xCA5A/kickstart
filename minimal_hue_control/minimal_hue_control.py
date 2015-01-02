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
import colorsys
import sys
import pprint
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
    DEFAULT_HUE_USERNAME = 'developer'

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
        logger.info("INFO list supported colors")
        for color in MinimalHUEControl.colors:
            logger.info("  * %s" % repr(color))

    def __init__(self,
                 _bridge_address=DEFAULT_HUE_BRIDGE_ADDRESS_STRING,
                 _user_name=DEFAULT_HUE_USERNAME):

        self._bridge_address = _bridge_address
        self._hue_bridge = phue.Bridge(_bridge_address, _user_name)

    def set_light_color(self, _light_name, _color):

        print "[TODO] add lib call"

        for _valid_color in self.colors:
            if _valid_color[0] == _color:
                logger.info("set color %s for light %s" % (_color, _light_name))
                break
        else:
            logger.error("color %s unknown, do not set anything" % _color)



        # TODO(casasam): this seems to be better, check this lib

        #
        # colorsys.rgb_to_hls(r, g, b)
        #
        #     Convert the color from RGB coordinates to HLS coordinates.

        # check this stuff, https://github.com/issackelly/python-hue/blob/master/hue.py
        #  def rgb(self, red, green=None, blue=None, transitiontime=5):
        # if isinstance(red, basestring):
        # # assume a hex string is passed
        # rstring = red
        # red = int(rstring[1:3], 16)
        # green = int(rstring[3:5], 16)
        # blue = int(rstring[5:], 16)
        # print red, green, blue
        # # We need to convert the RGB value to Yxy.
        # redScale = float(red) / 255.0
        # greenScale = float(green) / 255.0
        # blueScale = float(blue) / 255.0
        # colormodels.init(
        # phosphor_red=colormodels.xyz_color(0.64843, 0.33086),
        # phosphor_green=colormodels.xyz_color(0.4091, 0.518),
        # phosphor_blue=colormodels.xyz_color(0.167, 0.04))
        # logger.debug(redScale, greenScale, blueScale)
        # xyz = colormodels.irgb_color(red, green, blue)
        # logger.debug(xyz)
        # xyz = colormodels.xyz_from_rgb(xyz)
        # logger.debug(xyz)
        # xyz = colormodels.xyz_normalize(xyz)
        # logger.debug(xyz)


    def connect_application(self):
        logger.info("connect application to bridge")
        # self._hue_bridge.connect()
        # logger.error('Failed to open file', exc_info=True)
        print "[TODO] add lib call"

    def turn_off_light(self, _light_name):
        logger.info("turn off light %s" % _light_name)
        print "[TODO] add lib call"

    def list_known_lights(self):
        logger.info("list the lights known by the HUE bridge")
        print "[TODO] add lib call"
    #     for light in bridge.lights:
    # light.on = True
    # light.hue = 0
    #

    def is_connected(self, light_name):
        # try something with getaip here...
        print "[TODO] add lib call"



if __name__ == "__main__":

    try:
        # parse cli arguments, use file docstring as a parameter definition
        cli_arguments = docopt.docopt(__doc__)

    # handle invalid options
    except docopt.DocoptExit as e:
        logger.error(e.message)
        sys.exit(1)

    _main(cli_arguments)