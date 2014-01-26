# -*- coding: utf-8 -*-


import DataElementIndex


class DataElementIndexer(object):
    """
    """

    def __init__(self):
        """
        """
        print "[i] HELLO FROM OBJECT %s" % (self.__class__.__name__)
        self.__dataElementIndex = DataElementIndex.DataElementIndex()

    def processData(self, textDataElemntList):
        """add data to index
        """
        for textDataElement in textDataElemntList:
            self.__dataElementIndex.insertTextDataElement(textDataElement)

    def getDataElementIndex(self):
        """simple member accessor function
        """
        return self.__dataElementIndex

    def getNumberOfDataBuckets(self):
        """simple member accessor function
        """
        return len(self.__dataElementIndex.getNumberOfDataBuckets())
