# -*- coding: utf-8 -*-


class DataElementIndex(object):
    """
    """

    def __init__(self):
        """
        """
        self.__wordBucketMap = {}

    def query(self, textDataQuery):
        """
        """
        resultTextDataElementDict = {}

        queryTermList = textDataQuery.getQueryTerms()
        for queryTerm in queryTermList:
            if self.__wordBucketMap.has_key(queryTerm):
                textDataElementTupleList = self.__wordBucketMap[queryTerm]

                #print type(textDataElementTupleList)

                for textDataElementTuple in textDataElementTupleList:
                    #print type(textDataElementTuple)
                    #print textDataElementTupleList
                    print textDataElementTuple
                    (textDataElemen, _) = textDataElementTuple
                    #if textDataElemen not in resultTextDataElementList:

                    if resultTextDataElementDict.has_key(queryTerm):
                        resultTextDataElementDict[queryTerm].append(textDataElemen)
                    else:
                        resultTextDataElementDict[queryTerm] = [textDataElemen]

                        #resultTextDataElementDict.append(textDataElemen)



        return resultTextDataElementDict


    def getNumberOfDataBuckets(self):
        return len(self.__wordBucketMap)

    def insertTextDataElement(self, textDataElement):

        for wordTuple in textDataElement.getFilteredTextDataList():
            (word, originalPosition) = wordTuple

            # check stuff here, there are unwanted words!
            #print word

            #TODO(sam): do this with a second map
            if self.__wordBucketMap.has_key(word):
                self.__wordBucketMap[word].append((textDataElement, originalPosition))
            else:
                self.__wordBucketMap[word] = [(textDataElement, originalPosition)]

