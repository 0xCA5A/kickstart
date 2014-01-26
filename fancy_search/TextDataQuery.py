# -*- coding: utf-8 -*-


class TextDataQuery(object):
    """
    """

    def __init__(self, textDataQueryString):
        """
        """
        print "[i] HELLO FROM OBJECT %s" % (self.__class__.__name__)
        self.__textDataQueryString = textDataQueryString

    def getQueryTerms(self):
        """
        """
        return [textDataQueryStringElement.strip().upper() for textDataQueryStringElement in self.__textDataQueryString.split(" ")]
