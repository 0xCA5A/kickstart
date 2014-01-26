# -*- coding: utf-8 -*-


import re


class TextDataElement(object):
    """simple class to store textual data
    """

    def __init__(self, origin, timestamp, rawTextData):
        """
        """
        # print "[i] HELLO FROM OBJECT %s" % (self.__class__.__name__)
        self.__origin = origin
        self.__timestamp = timestamp
        self.__rawTextData = rawTextData

        self.__filteredTextDataTupleDict = self.__filterRawTextData(self.__rawTextData)


    def __filterRawTextData(self, rawTextData):
        """
        """
        wordRegex = re.compile('^\w*$', re.IGNORECASE)
        rawWordList = rawTextData.split(" ")

        filteredTextDataTupleDict = {}
        originalTextDataElementPosition = -1
        for word in rawWordList:
            originalTextDataElementPosition += 1

            # remove unwanted symbols
            re.sub(r'[^\w]', ' ', word)
            word = word.rstrip().upper()

            # BUG(sam): fix this in a proper way...
            if (len(word) == 0):
                continue

            if wordRegex.match(word):
                if filteredTextDataTupleDict.has_key(word):
                    filteredTextDataTupleDict[word].append(originalTextDataElementPosition)
                else:
                    filteredTextDataTupleDict[word] = [originalTextDataElementPosition]

        return filteredTextDataTupleDict


    def getOrigin(self):
        return self.__origin


    def getFilteredTextDataDict(self):
        """simple accessor function
        """
        return self.__filteredTextDataTupleDict


    def getPositionDataListByWord(self, word):
        """
        """
        if self.__filteredTextDataTupleDict.has_key(word):
            return self.__filteredTextDataTupleDict[word]
        else:
            return []
