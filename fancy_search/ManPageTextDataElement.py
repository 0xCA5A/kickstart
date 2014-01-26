# -*- coding: utf-8 -*-


import TextDataElement


class ManPageTextDataElement(TextDataElement.TextDataElement):

    def __init__(self, origin, timestamp, rawTextData, manPageSection, shortDescription):
        super(ManPageTextDataElement, self).__init__(origin, timestamp, rawTextData)

        # print "[i] HELLO FROM OBJECT %s" % (self.__class__.__name__)
        self.__manPageSection = manPageSection
        self.__shortDescription = shortDescription


    def getManPageSection(self):
        return self.__manPageSection

    def getShortDescription(self):
        return self.__shortDescription