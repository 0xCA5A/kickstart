#!/usr/bin/env python


import LocalManPageImporter
import DataElementIndexer
import TextDataQuery


if __name__ == "__main__":
    """good old main...
    starts the whole crap
    """
    print "[MAIN] HELLO FROM MAIN"

    local_man_page_importer = LocalManPageImporter.LocalManPageImporter()
    print "[MAIN] start local man page importer..."
    manPageTextDataElementList = local_man_page_importer.processData()

    my_data_element_indexer = DataElementIndexer.DataElementIndexer()

    print "[MAIN] start indexer..."
    my_data_element_indexer.processData(manPageTextDataElementList)
    dataElementIndex = my_data_element_indexer.getDataElementIndex()

    print "[MAIN] current number of data buckets in index: %d" % (dataElementIndex.getNumberOfDataBuckets())

    my_text_data_query_string = "file type hello"
    my_textDataQuery = TextDataQuery.TextDataQuery(my_text_data_query_string)
    print "[MAIN] start query %s" % (my_text_data_query_string)
    textDataElementList = dataElementIndex.query(my_textDataQuery)

    print "[MAIN] query result:"
    for resultElement in textDataElementList:
        (word, dataElementObjectList) = resultElement
        print " word: %s found in data objects" % word
        for dataElementObject in dataElementObjectList:
            print "  * %s - %s (hit count: %d)" % (dataElementObject.getOrigin(), dataElementObject.getShortDescription(), len(dataElementObject.getPositionDataListByWord(word)))

    my_text_data_query_string = "gcc optimization optimisation make"
    my_textDataQuery = TextDataQuery.TextDataQuery(my_text_data_query_string)
    print "[MAIN] start query: %s" % (my_text_data_query_string)
    textDataElementList = dataElementIndex.query(my_textDataQuery)

    print "[MAIN] query result:"
    for resultElement in textDataElementList:
        (word, dataElementObjectList) = resultElement
        print " word: %s found in data objects" % word
        for dataElementObject in dataElementObjectList:
            print "  * %s - %s (%d hits)" % (dataElementObject.getOrigin(), dataElementObject.getShortDescription(), len(dataElementObject.getPositionDataListByWord(word)))

    print "[MAIN] LEAVE MAIN"
