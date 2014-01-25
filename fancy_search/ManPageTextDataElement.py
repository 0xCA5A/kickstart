# -*- coding: utf-8 -*-

import TextDataElement

class ManPageTextDataElement(TextDataElement.TextDataElement):
    def __init__(self, origin, timestamp, rawTextData, manPageSection):
        super(ManPageTextDataElement, self).__init__(origin, timestamp, rawTextData)
        self.__manPageSection = manPageSection

