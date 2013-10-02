#include <iostream>
#include <typeinfo>

#include "MixerAlgorithmSimpleAddWithNormalization.hpp"



void MixerAlgorithmSimpleAddWithNormalization::algorithm()
{
    std::cout << typeid(*this).name() << std::endl;

/*
    Find out the volume in which you want the output sound to be. It can be the average or maxima of one of the signals.
    factor = average(s1) You assume that both signals are already OK, not overflowing the 32767.0
    Normalize both signals with this factor:
    s1 = (s1/max(s1))*factor
    s2 = (s2/max(s2))*factor
    Add them together and normalize the result with the same factor
    output = ((s1+s2)/max(s1+s2))*factor
*/


}
