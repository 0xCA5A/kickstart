# -*- coding: utf-8 -*-


class DataElementIndex(object):
    """
    """

    def __init__(self):
        """
        """
        print "[i] HELLO FROM OBJECT %s" % (self.__class__.__name__)
        self.__word_bucket_map = {}

    def query(self, text_data_query):
        """simple query to the index
        """
        query_term_list = text_data_query.get_query_terms()

        result_text_data_element_list = []
        for query_term in query_term_list:
            if query_term in self.__word_bucket_map:
                result_text_data_element_list.append((query_term, self.__word_bucket_map[query_term]))

        return result_text_data_element_list

    def get_number_of_data_buckets(self):
        return len(self.__word_bucket_map)

    def insert_text_data_element(self, text_data_element):
        """
        """
        for word, positionList in text_data_element.get_filtered_text_data_dict().iteritems():
            if word in self.__word_bucket_map:
                self.__word_bucket_map[word].append(text_data_element)
            else:
                self.__word_bucket_map[word] = [text_data_element]
