# -*- coding: utf-8 -*-


import re


class TextDataElement(object):
    """simple class to store textual data
    """

    def __init__(self, origin, timestamp, raw_text_data):
        """
        """
        # print "[i] HELLO FROM OBJECT %s" % (self.__class__.__name__)
        self.__origin = origin
        self.__timestamp = timestamp
        self.__raw_text_data = raw_text_data

        self.__filtered_text_data_dict = self.__filter_raw_text_data(self.__raw_text_data)

    def __filter_raw_text_data(self, raw_text_data):
        """
        """
        word_regex = re.compile('^\w*$', re.IGNORECASE)
        raw_word_list = raw_text_data.split(" ")

        filtered_text_data_dict = {}
        original_text_data_element_position = -1
        for word in raw_word_list:
            original_text_data_element_position += 1

            # remove unwanted symbols
            re.sub(r'[^\w]', ' ', word)
            word = word.rstrip().upper()

            # BUG(sam): fix this in a proper way...
            if (len(word) == 0):
                continue

            if word_regex.match(word):
                if word in filtered_text_data_dict:
                    filtered_text_data_dict[word].append(original_text_data_element_position)
                else:
                    filtered_text_data_dict[word] = [original_text_data_element_position]

        return filtered_text_data_dict

    def get_origin(self):
        """simple member accessor function
        """
        return self.__origin

    def get_filtered_text_data_dict(self):
        """simple member accessor function
        """
        return self.__filtered_text_data_dict

    def get_nr_of_filtered_words(self):
        return len(self.__filtered_text_data_dict)

    def get_position_data_list_by_word(self, word):
        """
        """
        if word in self.__filtered_text_data_dict:
            return self.__filtered_text_data_dict[word]
        else:
            return []
