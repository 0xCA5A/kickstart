#!/usr/bin/env python

import getopt
import sys
import pickle

import LocalManPageImporter
import DataElementIndexer
import TextDataQuery


if __name__ == "__main__":
    """good old main...
    starts the whole crap
    """
    print "[MAIN] HELLO FROM MAIN"

    try:
        opts, args = getopt.getopt(sys.argv[1:], "l:s:", ["load=", "store="])
    except getopt.GetoptError:
        "[MAIN] bad user input, check command line"
        sys.exit(2)

    load_index_dump_file_flag = False
    store_index_dump_file_flag = False
    for opt, arg in opts:
        if opt in ("-l", "--load"):
            load_index_dump_file_flag = True
            load_index_dump_file_name = arg
        elif opt in ("-s", "--store"):
            store_index_dump_file_flag = True
            store_index_dump_file_name = arg

    if load_index_dump_file_flag and store_index_dump_file_flag:
        print "[MAIN] use load or store, not both..."
        sys.exit(3)

    # generate index from data
    if not load_index_dump_file_flag:

        local_man_page_importer = LocalManPageImporter.LocalManPageImporter()
        print "[MAIN] start local man page importer..."
        man_page_text_data_element_list = local_man_page_importer.process_data()

        my_data_element_indexer = DataElementIndexer.DataElementIndexer()

        print "[MAIN] start indexer..."
        my_data_element_indexer.process_data(man_page_text_data_element_list)
        data_element_index = my_data_element_indexer.get_data_element_index()

        # store index to file
        if store_index_dump_file_flag:
            print "[MAIN] store index dump to file %s" % store_index_dump_file_name
            store_index_dump_file_handle = open(store_index_dump_file_name, "w")
            pickle.dump(data_element_index, store_index_dump_file_handle)
            store_index_dump_file_handle.close()

    # load index from file
    else:
        print "[MAIN] load index dump from file %s" % load_index_dump_file_name
        load_index_dump_file_handle = open(load_index_dump_file_name, "r")
        data_element_index = pickle.load(load_index_dump_file_handle)
        load_index_dump_file_handle.close()

    print "[MAIN] current number of data buckets in index: %d" % (data_element_index.get_number_of_data_buckets())

    my_text_data_query_string = "gcc hello guido world"
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
