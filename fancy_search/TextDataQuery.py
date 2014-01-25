# -*- coding: utf-8 -*-


class TextDataQuery(object):
    """
    """

    def __init__(self, textDataQueryString):
        """
        """
        self.__textDataQueryString = textDataQueryString

    def getQueryTerms(self):
        """
        """
        return [textDataQueryStringElement.strip().upper() for textDataQueryStringElement in self.__textDataQueryString.split(" ")]
