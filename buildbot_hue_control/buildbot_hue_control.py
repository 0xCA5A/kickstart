#!/usr/bin/env python

"""Simple pyhue wrapper to easily change the color a bulb identified by its name

Usage:
    buildbot_hue_control.py --bulb=NAME [--set-color=COLOR] [--address=ADDRESS] [--debug]
    buildbot_hue_control.py --connect [--debug]
    buildbot_hue_control.py --list [--address=ADDRESS] [--debug]

Options:
    --address=ADDRESS           HUE bridge address
    --bulb=NAME                 bulb identifier by name
    --set-color=COLOR           set color and urn light on
    --connect                   register application at HUE bridge
    --list                      list connected bulbs
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


    may: fix address for this hue guy??



"""

# Docopt is a library for parsing command line arguments
import docopt
import logging
import pyhue
import sys
import pprint


DEFAULT_HUE_BRIDGE_ADDRESS_STRING = '192.168.1.123'


def main(_cli_arguments):


    pprint.pprint(_cli_arguments)

    _list_connected_bubls = _cli_arguments['--list']
    if _list_connected_bubls:
        bb_hue_control = BuildBotHUEControl()
        bb_hue_control.list_connected_bulbs()
        sys.exit(0)

    _bulb_name = _cli_arguments['--bulb']

    _debug = False
    if _cli_arguments['--debug']:
        _debug = True

    _bridge_address = DEFAULT_HUE_BRIDGE_ADDRESS_STRING
    if _cli_arguments['--address'] is not None:
        _bridge_address = _cli_arguments['--address']


    _list_connected_bubls = _cli_arguments['--list']
    if _list_connected_bubls:
        bb_hue_control = BuildBotHUEControl(_debug=_debug, _bridge_address=_bridge_address)
        bb_hue_control.connect_application()
        sys.exit(0)




    bb_hue_control = BuildBotHUEControl(_bulb_name, _debug, _bridge_address)


class BuildBotHUEControl(object):
    """

    """

    def __init__(self, _bulb_name=None, _debug=False, _bridge_address="192.168.1.51"):
        self._bulb_name = _bulb_name
        self._bridge_address = _bridge_address

        self._logger = logging.getLogger(__name__)

        if _debug:
            self._logger.setLevel(logging.DEBUG)
        else:
            self._logger.setLevel(logging.INFO)


    def connect_application(self):


# logger.error('Failed to open file', exc_info=True)

        pass

    def list_connected_bulbs(self):
        self._logger.info("list connected bulbs")



    def is_connected(self, bulb_name):
        pass


    def set_color(self):
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