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
        """
        """
        for textDataElement in textDataElemntList:
            self.__dataElementIndex.insertTextDataElement(textDataElement)


    def getDataElementIndex(self):
        """simple accessor function
        """
        return self.__dataElementIndex


    def getNumberOfDataBuckets(self):
        """simple accessor function
        """
        return len(self.__dataElementIndex.getNumberOfDataBuckets())
