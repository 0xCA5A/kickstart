#!/usr/bin/env python

"""Simple pyhue wrapper to easily change the color a bulb identified by its name

Usage:
    buildbot_hue_control.py --bulb=NAME (--set-color=COLOR | --turn-off) [--address=ADDRESS] [--debug]
    buildbot_hue_control.py --connect [--debug]
    buildbot_hue_control.py --list-bulbs [--address=ADDRESS] [--debug]
    buildbot_hue_control.py --list-colors [--debug]


Options:
    --address=ADDRESS           HUE bridge address
    --bulb=NAME                 bulb identifier by name
    --turn-off                  turn bulb off
    --set-color=COLOR           set color and urn light on
    --list-colors               list supported colors
    --connect                   register application at HUE bridge
    --list-bulbs                list connected bulbs
    --debug                     show debug output


    light pattern proposal for a software build with tests:

    idle:                                   do nothing, show last set bulb color
    build job running:                      yellow
    software build failed:                  red
    bbs run failed:                         violet


    GET LIST OF PYTHON COLOR DEFNITION < CHECK COLOR DEFINITION IN PYTHON

    software build and bbs run successful: green

    software build
    build successful: green


    led set color

    get list of lights connected to the bridge
    enable light
    disable light
    set color to light


    http://anotherpythonprogrammer.blogspot.de/2013/05/pyhue-python-library-for-philips-hue.html

    SOURCE:
    https://github.com/studioimaginaire/phue


    may: fix address for this hue guy??



"""

import docopt
import logging
import phue
import colorsys
import sys
import pprint
import collections


DEFAULT_HUE_BRIDGE_ADDRESS_STRING = '192.168.1.123'
DEFAULT_LOG_LEVEL = logging.INFO
DEFAULT_HUE_USERNAME = 'developer'


def main(_cli_arguments):

    print "debug"
    pprint.pprint(_cli_arguments)


    _list_supported_colors = _cli_arguments['--list-colors']
    if _list_supported_colors:
        bb_hue_control = BuildBotHUEControl()
        bb_hue_control.list_supported_colors()
        sys.exit(0)

    _list_known_bubls = _cli_arguments['--list-bulbs']
    if _list_known_bubls:
        bb_hue_control = BuildBotHUEControl()
        bb_hue_control.list_known_bulbs()
        sys.exit(0)

    _bulb_name = _cli_arguments['--bulb']

    _debug = False
    if _cli_arguments['--debug']:
        _debug = True

    _bridge_address = DEFAULT_HUE_BRIDGE_ADDRESS_STRING
    if _cli_arguments['--address'] is not None:
        _bridge_address = _cli_arguments['--address']

    bb_hue_control = BuildBotHUEControl(_debug=_debug, _bridge_address=_bridge_address)

    _list_known_bulbs = _cli_arguments['--list-bulbs']
    if _list_known_bulbs:
        bb_hue_control.list_known_bulbs()
        sys.exit(0)

    _connect_application = _cli_arguments['--connect']
    if _connect_application:
        bb_hue_control.connect_application()
        sys.exit(0)

    _turn_off_bulb = _cli_arguments['--turn-off']
    if _turn_off_bulb:
        bb_hue_control.turn_off_bulb(_bulb_name)
        sys.exit(0)

    _set_color = _cli_arguments['--set-color']
    if _set_color:
        bb_hue_control.set_bulb_color(_bulb_name, _set_color)
        sys.exit(0)




class BuildBotHUEControl(object):
    """

    """

    Color = collections.namedtuple('Color', 'name, rgb_value')

    def __init__(self, _bulb_name=None, _debug=False, _bridge_address="192.168.1.51", _user_name=DEFAULT_HUE_USERNAME):
        self._bulb_name = _bulb_name
        self._bridge_address = _bridge_address

        self._hue_bridge = phue.Bridge(_bridge_address, _user_name)

        logging.basicConfig()
        self._logger = logging.getLogger(__name__)

        # define and register the supported colors
        self._colors = []
        self._colors.append(self.Color('red', (255, 0, 0)))
        self._colors.append(self.Color('green', (0, 255, 0)))
        self._colors.append(self.Color('blue', (0, 0, 255)))
        self._colors.append(self.Color('magenta', (255, 0, 255)))
        self._colors.append(self.Color('cyan', (0, 255, 255)))
        self._colors.append(self.Color('yellow', (255, 255, 0)))
        self._colors.append(self.Color('white', (255, 255, 255)))

        if _debug:
            self._logger.setLevel(logging.DEBUG)
        else:
            self._logger.setLevel(DEFAULT_LOG_LEVEL)


    def list_supported_colors(self):
        self._logger.info("list supported colors")
        for color in self._colors:
            self._logger.info(" * %s" % repr(color))


    def set_bulb_color(self, _bulb_name, _color):

        if _color in self._colors[0]:
            self._logger.info("set color %s for bulb %s" % (_color, _bulb_name))
            # self._hue_bridge
        else:
            self._logger.error("color %s unknown, exit immediately" % _color)

        #
        # colorsys.rgb_to_hls(r, g, b)
        #
        #     Convert the color from RGB coordinates to HLS coordinates.

    def connect_application(self):
        self._logger.info("connect application to bridge")
        # self._hue_bridge.connect()
        # logger.error('Failed to open file', exc_info=True)

    def turn_off_bulb(self, _bulb_name):
        self._logger.info("turn off bulb %s" % _bulb_name)


    def list_known_bulbs(self):
        self._logger.info("list the bulbs known by the HUE bridge")

    #     for light in bridge.lights:
    # light.on = True
    # light.hue = 0
    #



    def is_connected(self, bulb_name):
        # try something with getaip here...
        pass



if __name__ == "__main__":

    try:
        # parse cli arguments, use file docstring as a parameter definition
        cli_arguments = docopt.docopt(__doc__)

    # handle invalid options
    except docopt.DocoptExit as e:
        print e.message
        sys.exit(1)

    main(cli_arguments)