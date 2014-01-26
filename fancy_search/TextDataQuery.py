# -*- coding: utf-8 -*-


class TextDataQuery(object):
    """
    """

    def __init__(self, text_data_query_string):
        """
        """
        print "[i] HELLO FROM OBJECT %s" % (self.__class__.__name__)
        self.__text_data_query_string = text_data_query_string

    def get_query_terms(self):
        """
        """
        return [text_data_query_string_element.strip().upper() for text_data_query_string_element in self.__text_data_query_string.split(" ")]
