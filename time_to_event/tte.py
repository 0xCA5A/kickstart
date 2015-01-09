#!/usr/bin/env python
# -*- coding: utf-8 -*-

""" tte.py

    Usage:
        tte.py --year=2015 [--month=2 --day=19 --hour=22 --minute=10 --title=EVENT_TITLE --verbose]

    Options:
        --year=YEAR              : event year
        --month=MONTH            : event month
        --day=DAY                : event day
        --hour=HOUR              : event hour
        --minute=MINUTE          : event minute
        --title=TITLE_STRING     : event title
        --verbose                : verbose, more detailed time difference in formation
"""

import docopt
import datetime


DEFAULT_MONTH = 1
DEFAULT_DAY = 1
DEFAULT_HOUR = 0
DEFAULT_MINUTE = 0


def get_date_object_from_args(docopt_args):

    _get_key_from_arguments = lambda key, default_value: docopt_args[key] if docopt_args[key] else default_value

    # don't check year, this is mandatory!
    _year = int(docopt_args["--year"])
    _month = int(_get_key_from_arguments('--month', DEFAULT_MONTH))
    _day = int(_get_key_from_arguments('--day', DEFAULT_DAY))
    _hour = int(_get_key_from_arguments('--hour', DEFAULT_HOUR))
    _minute = int(_get_key_from_arguments('--minute', DEFAULT_MINUTE))

    return datetime.datetime(year=_year, month=_month, day=_day, hour=_hour, minute=_minute)


def main(docopt_args):
    """ main-entry point for program, expects dict with arguments from docopt() """
    # NOTE: no checking for -h, or --help is written here.
    # docopt will automagically check for it and use your usage string.

    date_x = get_date_object_from_args(docopt_args)
    date_now = datetime.datetime.now()
    date_delta = date_x - date_now

    event_title = "event"
    if docopt_args["--title"]:
        event_title = docopt_args["--title"]

    print "[INFO] '%s' date:\t\t%s" % (event_title, date_x)
    print "[INFO] time to event from now:\t\t%s" % date_delta

    if docopt_args["--verbose"]:
        in_seconds = date_delta.total_seconds()
        in_minutes = in_seconds / 60.0
        in_hours = in_minutes / 60.0
        print "\t\t\t\t\t\t~ %s seconds" % in_seconds
        print "\t\t\t\t\t\t~ %s minutes" % in_minutes
        print "\t\t\t\t\t\t~ %s hours" % in_hours


# START OF SCRIPT
if __name__ == "__main__":
    docopt_args = docopt.docopt(__doc__)
    main(docopt_args)
