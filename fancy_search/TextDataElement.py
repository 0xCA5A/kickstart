# -*- coding: utf-8 -*-

import re

class TextDataElement(object):
    """simple class to store textual data
    """

    def __init__(self, origin, timestamp, rawTextData):
        """
        """
        self.__origin = origin
        self.__timestamp = timestamp
        self.__rawTextData = rawTextData


        self.__filteredTextDataTupleList = self.__filterRawTextData(self.__rawTextData)


    def __filterRawTextData(self, rawTextData):
        """TODO
        """

        uppercaseWordRegex = re.compile('[A-Z]')
        filteredTextDataTupleList = []

        rawWordList = rawTextData.split(" ")

        originalTextDataElementPosition = 0
        for word in rawWordList:
            originalTextDataElementPosition += 1
            word = word.rstrip().upper()

            # BUG(sam): fix this in a proper way...
            if (len(word) < 3):
                continue

            # BUG(sam): WORD. is dropped, fix this
            if uppercaseWordRegex.match(word):
                filteredTextDataTupleList.append((word, originalTextDataElementPosition))

        return filteredTextDataTupleList


    def getOrigin(self):
        return self.__origin


    def getFilteredTextDataList(self):
        """simple accessor function
        """
        return self.__filteredTextDataTupleList

