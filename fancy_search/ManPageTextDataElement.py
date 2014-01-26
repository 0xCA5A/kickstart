# -*- coding: utf-8 -*-


import TextDataElement


class ManPageTextDataElement(TextDataElement.TextDataElement):

    def __init__(self, origin, timestamp, rawTextData, manPageSection, shortDescription):
        super(ManPageTextDataElement, self).__init__(origin, timestamp, rawTextData)
        self.__manPageSection = manPageSection
        self.__shortDescription = shortDescription


    def getShortDescription(self):
        return self.__shortDescription
