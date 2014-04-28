import getopt
import sys


class JackClient(object):

    def __init__(self, name):
        print "[i] hello from %s object" % (self.__class__.__name__)
        self.__name = name
        self.__input_port_list = []
        self.__output_port_list = []

    def get_input_port_list(self):
        return self.__input_port_list

    def get_output_port_list(self):
        return self.__output_port_list


class JackLspPCParser(object):

    def __init__(self):
        print "[i] hello from %s object" % (self.__class__.__name__)
        self.__jack_client_list = []

    def get_client_list(self):
        return self.__jack_client_list

    def parse(self, jack_lsp_file):
        print "[i] parse jack lsp file %s" % (jack_lsp_file)
        f = open(jack_lsp_file)
        lines = f.readlines()
        f.close()

        for line in lines:
            print "line: %s" % (line)


if __name__ == "__main__":
    """good old main...
    starts the whole crap
    """
    _application_name = sys.argv[0]

    print "[%s] lets parse jack_lsp output..." % (_application_name)

    try:
        opts, args = getopt.getopt(sys.argv[1:], "f:", ["file="])
    except getopt.GetoptError:
        print "[%s] bad user input, check command line" % (_application_name)
        sys.exit(2)

    _file_name = ""
    for opt, arg in opts:
        if opt in ("-f", "--file"):
            _file_name = arg
    if not len(_file_name):
        print "[!] need a file, exit immediately"
        sys.exit(1)

    jack_lsppc_parser = JackLspPCParser()
    jack_lsppc_parser.parse(_file_name)


    print "[%s] exit" % (_application_name)
