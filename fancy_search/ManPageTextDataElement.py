# -*- coding: utf-8 -*-


import TextDataElement


class ManPageTextDataElement(TextDataElement.TextDataElement):

    def __init__(self, origin, timestamp, raw_text_data, man_page_section, short_description):
        super(ManPageTextDataElement, self).__init__(origin, timestamp, raw_text_data)

        # print "[i] HELLO FROM OBJECT %s" % (self.__class__.__name__)
        self.__man_page_section = man_page_section
        self.__short_description = short_description

    def get_man_page_section(self):
        return self.__man_page_section

    def get_short_description(self):
        return self.__short_description
