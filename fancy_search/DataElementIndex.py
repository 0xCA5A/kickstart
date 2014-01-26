# -*- coding: utf-8 -*-


class DataElementIndex(object):
    """
    """

    def __init__(self):
        """
        """
        print "[i] HELLO FROM OBJECT %s" % (self.__class__.__name__)
        self.__wordBucketMap = {}


    def query(self, textDataQuery):
        """
        """
        queryTermList = textDataQuery.getQueryTerms()

        resultTextDataElementList = []
        for queryTerm in queryTermList:
            if self.__wordBucketMap.has_key(queryTerm):
                resultTextDataElementList.append((queryTerm, self.__wordBucketMap[queryTerm]))
        return resultTextDataElementList


    def getNumberOfDataBuckets(self):
        return len(self.__wordBucketMap)


    def insertTextDataElement(self, textDataElement):
        """
        """
        for word, positionList in textDataElement.getFilteredTextDataDict().iteritems():
            #TODO(sam): do this with a second map
            if self.__wordBucketMap.has_key(word):
                self.__wordBucketMap[word].append(textDataElement)
            else:
                self.__wordBucketMap[word] = [textDataElement]
