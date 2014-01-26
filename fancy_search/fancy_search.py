#!/usr/bin/env python


import LocalManPageImporter
import DataElementIndexer
import TextDataQuery


if __name__ == "__main__":
    """good old main...
    starts the whole crap
    """

    print "[i] HELLO FROM MAIN"

    local_man_page_importer = LocalManPageImporter.LocalManPageImporter()
    manPageTextDataElementList = local_man_page_importer.processData()

    my_data_element_indexer = DataElementIndexer.DataElementIndexer()
    my_data_element_indexer.processData(manPageTextDataElementList)
    dataElementIndex = my_data_element_indexer.getDataElementIndex()

    print "[i] current number of data buckets in index: %d" % (dataElementIndex.getNumberOfDataBuckets())

    my_text_data_query_string = "file type hello"
    my_textDataQuery = TextDataQuery.TextDataQuery(my_text_data_query_string)
    textDataElementList = dataElementIndex.query(my_textDataQuery)

    print "[i] query result:"
    for resultElement in textDataElementList:
        (word, dataElementObjectList) = resultElement
        print " word: %s found in data objects" % word
        for dataElementObject in dataElementObjectList:
            #elementString = "".append("  * %s" % (dataElementObject.getOrigin())
            print "  * %s - %s (%d hits)" % (dataElementObject.getOrigin(), dataElementObject.getShortDescription(), len(dataElementObject.getPositionDataListByWord(word)))
            #print elementString

    print "[i] LEAVE MAIN"
