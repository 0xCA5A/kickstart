#!/usr/bin/env python
# -*- coding: utf-8 -*-


import ManPageTextDataElement
import LocalManPageImporter
import DataElementIndex
import DataElementIndexer
import TextDataQuery




if __name__ == "__main__":
    """good old main...
    starts the whole crap
    """

    print "[i] HELLO FROM MAIN"

    local_man_page_importer = LocalManPageImporter.LocalManPageImporter()
    manPageTextDataElemntList = local_man_page_importer.processData()

    my_data_element_indexer = DataElementIndexer.DataElementIndexer()

    my_data_element_indexer.processData(manPageTextDataElemntList)
    dataElementIndex = my_data_element_indexer.getDataElementIndex()

    my_text_data_query_string = "file type hello beer"
    my_textDataQuery = TextDataQuery.TextDataQuery(my_text_data_query_string)
    textDataElementDict = dataElementIndex.query(my_textDataQuery)

    print "[i] query result:"
    for key, value in textDataElementDict.iteritems():
        print key

        for element in value:
            print element.getOrigin()

    print "[i] LEAVE MAIN"
