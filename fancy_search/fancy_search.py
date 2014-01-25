#!/usr/bin/env python
# -*- coding: utf-8 -*-

import subprocess
import time
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


class ManPageTextDataElement(TextDataElement):
    def __init__(self, origin, timestamp, rawTextData, manPageSection):
        super(ManPageTextDataElement, self).__init__(origin, timestamp, rawTextData)
        self.__manPageSection = manPageSection




class LocalManPageImporter(object):

    def __init__(self):
        print "[i] HELLO FROM CLASS %s" % (self.__class__.__name__)

    def __getLocalSystemManPageRecordsList(self):
        my_man_page_list_process = subprocess.Popen(['man', '-k', '.'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = my_man_page_list_process.communicate()

        localSystemManPageRecordList = out.rstrip().rsplit("\n")

        # dev hack
        print "DEV HACK DEV HACK DEV HACK"
        outLinesCopy = localSystemManPageRecordList[:]
        localSystemManPageRecordList = []
        for line in outLinesCopy:
            if len(line.split(" ")[0].rstrip()) < 2:
                localSystemManPageRecordList.append(line)
                #print line

        print "[i] %d man pages found on the system" % (len(localSystemManPageRecordList))
        return localSystemManPageRecordList


    def __createManPageTextDataElementsFromManPageRecords(self, manPageRecordList):
        manPageTextDataElemntList = []

        # expect someting like this: 
        # update-xmlcatalog (8) - maintain XML catalog files
        for line in manPageRecordList:
            # drop short description
            line = line.split(")")[0]
            manPageName = line.split("(")[0].rstrip()
            manPageSection = line.split("(")[1].rstrip()

            my_man_page_location_process = subprocess.Popen(['man', '--location', manPageName], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            origin, err = my_man_page_location_process.communicate()

            my_man_page_content_process = subprocess.Popen(['man', manPageName], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            rawTextData, err = my_man_page_content_process.communicate()

            manPageTextDataElemntList.append(ManPageTextDataElement(origin.rstrip(), time.time(), rawTextData.rstrip(), manPageSection))

        return manPageTextDataElemntList


    def processData(self):
        """function to import the man page data
        """
        localSystemManPageRecordList = self.__getLocalSystemManPageRecordsList()
        return self.__createManPageTextDataElementsFromManPageRecords(localSystemManPageRecordList)


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


class DataElementIndexer(object):
    """
    """
    def __init__(self):
        """
        """
        self.__dataElementIndex = DataElementIndex()
    
    
    def getNumberOfDataBuckets(self):
        """
        """
        return len(self.__dataElementIndex.getNumberOfDataBuckets())
    
    
    def processData(self, textDataElemntList):
        """
        """
        for textDataElement in textDataElemntList:
            self.__dataElementIndex.insertTextDataElement(textDataElement)


    def getDataElementIndex(self):
        """simple accessor function
        """
        return self.__dataElementIndex


class TextDataQuery(object):
    """
    """

    def __init__(self, textDataQueryString):
        """
        """
        self.__textDataQueryString = textDataQueryString

    def getQueryTerms(self):
        """
        """
        return [textDataQueryStringElement.strip().upper() for textDataQueryStringElement in self.__textDataQueryString.split(" ")]


if __name__ == "__main__":
    """good old main...
    starts the whole crap
    """

    print "[i] HELLO FROM MAIN"

    local_man_page_importer = LocalManPageImporter()
    manPageTextDataElemntList = local_man_page_importer.processData()

    my_data_element_indexer = DataElementIndexer()

    my_data_element_indexer.processData(manPageTextDataElemntList)
    dataElementIndex = my_data_element_indexer.getDataElementIndex()

    my_text_data_query_string = "file type hello beer"
    my_textDataQuery = TextDataQuery(my_text_data_query_string)
    textDataElementDict = dataElementIndex.query(my_textDataQuery)

    print "[i] query result:"
    for key, value in textDataElementDict.iteritems():
        print key

        for element in value:
            print element.getOrigin()

    print "[i] LEAVE MAIN"
