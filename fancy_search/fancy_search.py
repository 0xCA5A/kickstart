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
    man_page_text_data_element_list = local_man_page_importer.process_data()

    my_data_element_indexer = DataElementIndexer.DataElementIndexer()

    print "[MAIN] start indexer..."
    my_data_element_indexer.process_data(man_page_text_data_element_list)
    data_element_index = my_data_element_indexer.get_data_element_index()

    print "[MAIN] current number of data buckets in index: %d" % (data_element_index.get_number_of_data_buckets())

    my_text_data_query_string = "gcc hello guido"
    my_text_data_query = TextDataQuery.TextDataQuery(my_text_data_query_string)
    print "[MAIN] start query %s" % (my_text_data_query_string)
    text_data_element_list = data_element_index.query(my_text_data_query)

    print "[MAIN] query result:"
    for result_element in text_data_element_list:
        (word, data_element_object_list) = result_element
        print " word: %s found in data objects" % word
        for data_element_object in data_element_object_list:
            print "  * %s - %s (hit count: %d)" % (data_element_object.get_origin(), data_element_object.get_short_description(), len(data_element_object.get_position_data_list_by_word(word)))

    print "[MAIN] LEAVE MAIN"
