# -*- coding: utf-8 -*-


import DataElementIndex


class DataElementIndexer(object):
    """
    """

    def __init__(self):
        """
        """
        print "[i] HELLO FROM OBJECT %s" % (self.__class__.__name__)
        self.__data_element_index = DataElementIndex.DataElementIndex()

    def process_data(self, text_data_elemnt_list):
        """add data to index
        """
        for text_data_element in text_data_elemnt_list:
            self.__data_element_index.insert_text_data_element(text_data_element)

    def get_data_element_index(self):
        """simple member accessor function
        """
        return self.__data_element_index

    def get_number_of_data_buckets(self):
        """simple member accessor function
        """
        return len(self.__data_element_index.get_number_of_data_buckets())
