# -*- coding: utf-8 -*-


import subprocess
import time

import ManPageTextDataElement


class LocalManPageImporter(object):

    def __init__(self):
        print "[i] HELLO FROM OBJECT %s" % (self.__class__.__name__)

    def __get_local_system_man_page_records_list(self):
        my_man_page_list_process = subprocess.Popen(['man', '-k', '.'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = my_man_page_list_process.communicate()

        local_system_man_page_record_list = out.rstrip().split("\n")

        ## NOTE(sam): dev hack
        #print "DEV HACK DEV HACK DEV HACK"
        #max_man_page_name_length = 4
        #out_lines_copy = local_system_man_page_record_list[:]
        #local_system_man_page_record_list = []
        #for line in out_lines_copy:
            #if len(line.split(" ")[0].rstrip()) < max_man_page_name_length:
                #local_system_man_page_record_list.append(line)

        print "[i] %d man pages found on the system" % (len(local_system_man_page_record_list))
        return local_system_man_page_record_list

    def __create_man_page_text_data_elements_from_man_page_records(self, man_page_record_list):
        man_page_text_data_elemnt_list = []

        print "[i] %d create man page text data elements..."

        # expect someting like this:
        # update-xmlcatalog (8) - maintain XML catalog files
        record_counter = 0
        for line in man_page_record_list:

            # monster hack...
            short_description = line.split(")")[1].replace(")", '').strip()[2:]

            line = line.split(")")[0]
            man_page_name = line.split("(")[0].rstrip()
            man_page_section = line.split("(")[1].rstrip()

            my_man_page_location_process = subprocess.Popen(['man', '--location', man_page_name], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            origin, err = my_man_page_location_process.communicate()

            my_man_page_content_process = subprocess.Popen(['man', man_page_name], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            raw_text_data, err = my_man_page_content_process.communicate()

            print " * process record %d / %d, add data from origin %s" % (record_counter, len(man_page_record_list), origin.rstrip())
            man_page_text_data_elemnt_list.append(ManPageTextDataElement.ManPageTextDataElement(origin.rstrip(), time.time(), raw_text_data.rstrip(), man_page_section, short_description))
            record_counter += 1

        return man_page_text_data_elemnt_list

    def process_data(self):
        """function to import the man page data
        """
        local_system_man_page_record_list = self.__get_local_system_man_page_records_list()
        man_page_text_data_elemnt_list = self.__create_man_page_text_data_elements_from_man_page_records(local_system_man_page_record_list)

        filtered_data_word_sum = 0
        for data_element in man_page_text_data_elemnt_list:
            filtered_data_word_sum += data_element.get_nr_of_filtered_words()

        print "[i] %d filtered words processed" % filtered_data_word_sum

        return man_page_text_data_elemnt_list
