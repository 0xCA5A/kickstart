# -*- coding: utf-8 -*-


import subprocess
import time

import ManPageTextDataElement

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

            manPageTextDataElemntList.append(ManPageTextDataElement.ManPageTextDataElement(origin.rstrip(), time.time(), rawTextData.rstrip(), manPageSection))

        return manPageTextDataElemntList


    def processData(self):
        """function to import the man page data
        """
        localSystemManPageRecordList = self.__getLocalSystemManPageRecordsList()
        return self.__createManPageTextDataElementsFromManPageRecords(localSystemManPageRecordList)
