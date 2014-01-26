# -*- coding: utf-8 -*-


import subprocess
import time

import ManPageTextDataElement


class LocalManPageImporter(object):

    def __init__(self):
        print "[i] HELLO FROM OBJECT %s" % (self.__class__.__name__)

    def __getLocalSystemManPageRecordsList(self):
        my_man_page_list_process = subprocess.Popen(['man', '-k', '.'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = my_man_page_list_process.communicate()

        localSystemManPageRecordList = out.rstrip().rsplit("\n")

        # NOTE(sam): dev hack
        print "DEV HACK DEV HACK DEV HACK"
        max_man_page_name_length = 4
        outLinesCopy = localSystemManPageRecordList[:]
        localSystemManPageRecordList = []
        for line in outLinesCopy:
            if len(line.split(" ")[0].rstrip()) < max_man_page_name_length:
                localSystemManPageRecordList.append(line)

        print "[i] %d man pages found on the system" % (len(localSystemManPageRecordList))
        return localSystemManPageRecordList

    def __createManPageTextDataElementsFromManPageRecords(self, manPageRecordList):
        manPageTextDataElemntList = []

        print "[i] %d create man page text data elements..."

        # expect someting like this:
        # update-xmlcatalog (8) - maintain XML catalog files
        record_counter = 0
        for line in manPageRecordList:

            # monster hack...
            shortDescription = line.split(")")[1].replace(")", '').strip()[2:]

            line = line.split(")")[0]
            manPageName = line.split("(")[0].rstrip()
            manPageSection = line.split("(")[1].rstrip()

            my_man_page_location_process = subprocess.Popen(['man', '--location', manPageName], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            origin, err = my_man_page_location_process.communicate()

            my_man_page_content_process = subprocess.Popen(['man', manPageName], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            rawTextData, err = my_man_page_content_process.communicate()

            print " * process record %d / %d, add data from origin %s" % (record_counter, len(manPageRecordList), origin.rstrip())
            manPageTextDataElemntList.append(ManPageTextDataElement.ManPageTextDataElement(origin.rstrip(), time.time(), rawTextData.rstrip(), manPageSection, shortDescription))
            record_counter += 1

        return manPageTextDataElemntList

    def processData(self):
        """function to import the man page data
        """
        localSystemManPageRecordList = self.__getLocalSystemManPageRecordsList()
        manPageTextDataElemntList = self.__createManPageTextDataElementsFromManPageRecords(localSystemManPageRecordList)

        filteredDateWordSum = 0
        for dateElement in manPageTextDataElemntList:
            filteredDateWordSum += dateElement.getNrOfFilteredWords()

        print "[i] %d filtered words processed" % filteredDateWordSum

        return manPageTextDataElemntList
