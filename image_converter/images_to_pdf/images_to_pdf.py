#!/usr/bin/env python3

"""usage: images_to_pdf.py [--output_dir=<PATH>] [--scale_factor=1.0] --image_dir=<PATH>

arguments:
    none

options:
    --output_dir=<PATH>  output path, cwd if not defined
    --scale_factor=0.4   image scale factor relative to paper width
    --image_dir=<PATH>   path to the images
"""


import logging
import docopt
import os
import sys
import pprint
import subprocess

logging.basicConfig(format='%(asctime)s %(levelname)-8s %(name)-15s %(message)s', level=logging.DEBUG)
logger = logging.getLogger(__name__)

_LATEX_SRC_FILE = 'all_images.tex'
_LATEX_OUT_FILE = 'all_images.pdf'

if __name__ == '__main__':

    cfg = {}
    cfg["image_dir"] = None
    cfg["scale_factor"] = None
    cfg["output_dir"] = None

    try:
        # parse arguments, use file docstring as a parameter definition
        arguments = docopt.docopt(__doc__)

        # count is a mandatory option, caps is optional
        cfg["image_dir"] = arguments['--image_dir']
        cfg["scale_factor"] = arguments['--scale_factor']
        cfg["output_dir"] = arguments['--output_dir']

    # handle invalid options
    except docopt.DocoptExit as exception:
        logger.error(exception)
        sys.exit(1)

    # check arguments
    if not os.path.exists(cfg["image_dir"]):
        logger.error("image directory path %s does not exist", cfg["image_dir"])
        sys.exit(1)

    if not cfg["scale_factor"]:
        cfg["scale_factor"] = 1.0

    if not cfg["output_dir"]:
        cfg["output_dir"] = os.getcwd()
    if not os.path.exists(cfg["output_dir"]):
        logger.error("output directory path %s does not exist", cfg["output_dir"])
        sys.exit(1)

    logger.debug("current configuration:\n%s", pprint.pformat(cfg))

    image_file_path_list = list()
    for top, dirs, files in os.walk(cfg["image_dir"]):
        for nm in files:
            image_file_path_list.append(os.path.join(top, nm))

    with open(_LATEX_SRC_FILE, 'w+') as latex_source_file:
        latex_source_file.write("\documentclass[a4paper]{article}\n")
        latex_source_file.write("\\usepackage[utf8]{inputenc}")
        latex_source_file.write("\\usepackage[cm]{fullpage}\n")
        latex_source_file.write("\\usepackage{graphicx}\n")
        latex_source_file.write("\\begin{document}\n")
        latex_source_file.write("\\begin{center}\n")

        for image_file_path in image_file_path_list:
            latex_source_file.write("\includegraphics[width=%f\\textwidth]{%s}\n" % (float(cfg["scale_factor"]), image_file_path))
            latex_source_file.write("\\newline")
            latex_source_file.write("\\vfill")

        latex_source_file.write("\\end{center}\n")
        latex_source_file.write("\end{document}\n")

    subprocess.call(["/usr/bin/pdflatex", _LATEX_SRC_FILE])
