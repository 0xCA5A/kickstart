#include "AudioAnalyzer.h"
#include "JackClient.h"

#include <jack/ringbuffer.h>

#include "fftw3.h"
#include <complex.h>
#include <math.h>

#include "debug.h"

#include <jack/jack.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>



// BAD STUFF!
volatile bool g_runFlag = true;


void signalCallbackHandler(int signum)
{
    DEBUG_PRINT("caught signal %d, exit now\n", signum);
    g_runFlag = false;
}


inline uint32_t assembleReadyHMIPattern(uint8_t redValue, uint8_t greenValue, uint8_t blueValue) {
    return 0x00000000 | redValue << 24 | greenValue << 16 | blueValue << 8;
}


#pragma GCC optimize 3
inline void hannWindow(float dataIn[], double dataOut[], uint32_t arraySize) {
    const double PI = 3.141592653589793238463;

    for (uint32_t i = 0; i < arraySize; i++) {
        double multiplier = 0.5 * (1 - cos(2 * PI * i / (arraySize - i)));
        dataOut[i] = multiplier * dataIn[i];
    }
}
#pragma GCC reset_options


void setLEDValueInPercent(uint8_t* ledValue, uint8_t ledMaxValue, uint8_t percent) {
    *ledValue = ledMaxValue * percent / 100;
}



void updateLEDValuesRMS(jack_default_audio_sample_t* p_sampleBuffer, uint32_t nrOfSamplesPerPeriod, uint8_t* p_redLEDValue, uint8_t* p_greenLEDValue, uint8_t* p_blueLEDValue)
{
    uint8_t localRedLEDValue = 0;
    uint8_t localGreenLEDValue = 0;
    uint8_t localBlueLEDValue = 0;

    const uint8_t blueLEDMaxValue = 24;
    const uint8_t greenLEDMaxValue = 24;
    const uint8_t redLEDMaxValue = 24;
    static double rmsMaxValue = 0;

    static uint32_t rmsMaxValueAdjustCounter = 0;


    // get rms value
    double rmsValue = 0;
    for (uint32_t i = 0; i < nrOfSamplesPerPeriod; i++) {
        rmsValue += (p_sampleBuffer[i] * p_sampleBuffer[i]);
    }
    rmsValue = rmsValue / nrOfSamplesPerPeriod;
    rmsValue = sqrt(rmsValue);

    // check max rms value
    if (rmsMaxValue < rmsValue) {
        rmsMaxValue = rmsValue;
        (rmsMaxValueAdjustCounter % 2) ? localRedLEDValue  = redLEDMaxValue : localBlueLEDValue = blueLEDMaxValue;
        rmsMaxValueAdjustCounter++;
        goto exit;
    }

    localGreenLEDValue = (rmsValue * 100) / rmsMaxValue;
    setLEDValueInPercent(&localGreenLEDValue, localGreenLEDValue, greenLEDMaxValue);

#if 0
    printf("\nrmsValue: %f\n", rmsValue);
    printf("rmsMaxValue: %f\n", rmsMaxValue);
    printf("localGreenLEDValue: 0x%x %d\n", localGreenLEDValue, localGreenLEDValue);
#endif


exit:

    // decrease rms max value
    if (rmsMaxValue > 0) {
        rmsMaxValue -= 0.01;
    }

    // write led values
    *p_redLEDValue = localRedLEDValue;
    *p_greenLEDValue = localGreenLEDValue;
    *p_blueLEDValue = localBlueLEDValue;

}


void updateLEDValuesFFT(jack_default_audio_sample_t* p_sampleBuffer, uint32_t nrOfSamplesPerPeriod, uint8_t* p_redLEDValue, uint8_t* p_greenLEDValue, uint8_t* p_blueLEDValue)
{
    uint8_t localRedLEDValue = 0;
    uint8_t localGreenLEDValue = 0;
    uint8_t localBlueLEDValue = 0;


    double doubleSampleBuffer[nrOfSamplesPerPeriod];

    // FFT stuff
    const uint32_t N = nrOfSamplesPerPeriod;


    uint32_t nrOfCoefficients = N / 2;

    // NOTE(scasa): abuse this to 'not use the whole value range'...
    // value has to be lower than N / 2
//     nrOfCoefficients = N / 2 / 2;

    fftw_complex complexResults[nrOfCoefficients];
    double magnitude[nrOfCoefficients];

    double blueMagnitudeSum = 0;
    double greenMagnitudeSum = 0;
    double redMagnitudeSum = 0;

    fftw_plan myFftwPlan = NULL;


//     const uint32_t blueLEDMaxValue = 32;
    const uint32_t greenLEDMaxValue = 24;
//     const uint32_t redLEDMaxValue = 32;


    const uint32_t blueBoundary = (nrOfCoefficients / 3);
    const uint32_t greenBoundary = (2 * nrOfCoefficients / 3);

    static double magnitudeSumMaximum = 0;


        // hanning windows function
        hannWindow(p_sampleBuffer, doubleSampleBuffer, nrOfSamplesPerPeriod);

        // run dft
        myFftwPlan = fftw_plan_dft_r2c_1d(N, doubleSampleBuffer, complexResults, FFTW_ESTIMATE);
        fftw_execute(myFftwPlan);

        blueMagnitudeSum = 0;
        greenMagnitudeSum = 0;
        redMagnitudeSum = 0;
        for (uint32_t i = 0; i < nrOfCoefficients; i++) {
            magnitude[i] = sqrt(complexResults[i][0] * complexResults[i][0] + complexResults[i][1] * complexResults[i][1]);

            // blue, low freq's
            if (i < blueBoundary) {
                blueMagnitudeSum += magnitude[i];
            } else
            // green, middle freq's
            if (i < greenBoundary) {
                greenMagnitudeSum += magnitude[i];
            }
            // red, high freq's
            else {
                redMagnitudeSum += magnitude[i];
            }

        }


        const double magnitudeSum = blueMagnitudeSum + greenMagnitudeSum + redMagnitudeSum;
        if (magnitudeSum > magnitudeSumMaximum) {
            magnitudeSumMaximum = magnitudeSum;
            localRedLEDValue = 0xFF;
        }

/*
        const uint32_t bluePercent = blueMagnitudeSum * 100 / magnitudeSum;
        const uint32_t greenPercent = greenMagnitudeSum * 100 / magnitudeSum;
        const uint32_t redPercent = redMagnitudeSum * 100 / magnitudeSum;

        uint8_t newBlueLEDValue = blueLEDMaxValue * bluePercent / 100;
        uint8_t newGreenLEDValue = greenLEDMaxValue * greenPercent / 100;
        uint8_t newRedLEDValue = redLEDMaxValue * redPercent / 100;

        // avoid jumps
        if (blueLEDValue < newBlueLEDValue) {
            blueLEDValue++;
        } else {
            blueLEDValue--;
        }

        if (greenLEDValue < newGreenLEDValue) {
            greenLEDValue++;
        } else {
            greenLEDValue--;
        }

        if (redLEDValue < newRedLEDValue) {
            redLEDValue++;
        } else {
            redLEDValue--;
        }*/


        localGreenLEDValue = magnitudeSum * 100 / magnitudeSumMaximum;
        localGreenLEDValue = greenLEDMaxValue * localGreenLEDValue / 100;

//         if (greenLEDValue < newGreenLEDValue) {
//             greenLEDValue++;
//         } else {
//             greenLEDValue--;
//         }



//         if (counter % 6 == 0)
//         {
//             printf("\n\nfpp: %d\n", jackClient.getNumberOfFramesPerPeriod());
//             printf("sample rate: %d\n", jackClient.getSampleRate());
//             printf("frame time: %f [ms]\n", (1.0 / jackClient.getSampleRate() * jackClient.getNumberOfFramesPerPeriod()));
//
//             printf("mag sum: %f\n", magnitudeSum);
//
// //             printf("bluePercent: %d, %d \n", bluePercent, blueLEDValue);
// //             printf("greenPercent: %d, %d\n", greenPercent, greenLEDValue);
// //             printf("redPercent: %d, %d\n", redPercent, redLEDValue);
//         }


        // decrease magnitude sum maximum
        if (magnitudeSumMaximum > 1) {
            magnitudeSumMaximum -= 4;
       }


    // write led values
    *p_redLEDValue = localRedLEDValue;
    *p_greenLEDValue = localGreenLEDValue;
    *p_blueLEDValue = localBlueLEDValue;

}


void updateLEDValuesFFT2(jack_default_audio_sample_t* p_sampleBuffer, uint32_t nrOfSamplesPerPeriod, uint8_t* p_redLEDValue, uint8_t* p_greenLEDValue, uint8_t* p_blueLEDValue)
{

    static uint32_t counter = 0;

    double doubleSampleBuffer[nrOfSamplesPerPeriod];

    // FFT stuff
    const uint32_t N = nrOfSamplesPerPeriod;


    uint32_t nrOfCoefficients = N / 2;

    // NOTE(scasa): abuse this to 'not use the whole value range'...
    // value has to be lower than N / 2
//     nrOfCoefficients = N / 2 / 2;

    fftw_complex complexResults[nrOfCoefficients];
    double magnitude[nrOfCoefficients];


    fftw_plan myFftwPlan = NULL;

    // hanning windows function
    hannWindow(p_sampleBuffer, doubleSampleBuffer, nrOfSamplesPerPeriod);

    // run dft
    myFftwPlan = fftw_plan_dft_r2c_1d(N, doubleSampleBuffer, complexResults, FFTW_ESTIMATE);
    fftw_execute(myFftwPlan);



    double maxMagnitude = 0;
    double magnitudeSum = 0;
    double magnitudeAVG = 0;
    
    
    uint32_t maxMagnitudeIndex = 0;

    for (uint32_t i = 0; i < nrOfCoefficients; i++) {
            magnitude[i] = sqrt(complexResults[i][0] * complexResults[i][0] + complexResults[i][1] * complexResults[i][1]);
            if (maxMagnitude < magnitude[i]) {
                maxMagnitude = magnitude[i];
                maxMagnitudeIndex = i;
            }
            magnitudeSum += magnitude[i];
            
    }
    magnitudeAVG = magnitudeSum / nrOfCoefficients;

    const uint32_t ferqWithInHz = nrOfCoefficients / 80;

    if (counter % 100 == 0) {  

        printf("\n\nfreq max: %d\n", (maxMagnitudeIndex + 1) * ferqWithInHz);

        // magnitude of a variable: measure of how far, regardless of direction, its quality differs from zero. magnitude are always positive values. 
        printf("mag max: %f\n", maxMagnitude);
        printf("mag avg: %f\n", magnitudeAVG);

        // 2x because of the loss of energy

        // amplitude of a variable: measure how far, and in which direction, that variable differs to zero. signal amplitude can be positive or negative.
        printf("amplitude: %f\n", (2 * maxMagnitude) / nrOfCoefficients );

    }

    // write led values
    *p_redLEDValue = 0x0;
    *p_greenLEDValue = 0x0;
    *p_blueLEDValue = 0x0;

    (counter % 2) ? *p_greenLEDValue = 0xFF : *p_redLEDValue = 0xFF;

    counter++;
}

int main()
{

//     mpg123HMIEqualizer myMpg123HMIEqualizer;

    signal(SIGINT, signalCallbackHandler);
    signal(SIGTERM, signalCallbackHandler);

    EsuHMI esuHMI;


    JackClient jackClient;
    const uint32_t numberOfFramesInRingbuffer = 2;
    if (!jackClient.setup(&g_runFlag, numberOfFramesInRingbuffer)) {
        return -1;
    }

    jack_ringbuffer_t* p_jackRingBuffer = jackClient.getRingBuffer();

    if (!jackClient.activateClientAndAutoConnectPorts()) {
        jackClient.shutdown();
        return -2;
    }


    esuHMI.threadStart();


    uint8_t redLEDValue = 1;
    uint8_t greenLEDValue = 1;
    uint8_t blueLEDValue = 1;

    const uint32_t nrOfSamplesPerPeriod = jackClient.getNumberOfFramesPerPeriod();
    jack_default_audio_sample_t sampleBuffer[nrOfSamplesPerPeriod];

    while(g_runFlag) {

        const uint32_t readSizeInByte = nrOfSamplesPerPeriod * sizeof(jack_default_audio_sample_t);
        if (jack_ringbuffer_read_space(p_jackRingBuffer) >= readSizeInByte) {
            jack_ringbuffer_read(p_jackRingBuffer, reinterpret_cast<char*>(sampleBuffer), readSizeInByte);
        } else {
            usleep(1000);
            continue;
        }


//         updateLEDValuesFFT(sampleBuffer, nrOfSamplesPerPeriod, &redLEDValue, &greenLEDValue, &blueLEDValue);
//         updateLEDValuesRMS(sampleBuffer, nrOfSamplesPerPeriod, &redLEDValue, &greenLEDValue, &blueLEDValue);
    updateLEDValuesFFT2(sampleBuffer, nrOfSamplesPerPeriod, &redLEDValue, &greenLEDValue, &blueLEDValue);

        esuHMI.setLEDPattern(assembleReadyHMIPattern(redLEDValue, greenLEDValue, blueLEDValue));



    }

    DEBUG_PRINT("stop communicating objects");
    jackClient.shutdown();

    esuHMI.threadStop();

    // 16 ms ~ 60hz
    usleep(15000);

    DEBUG_PRINT("exit main gracefully");
    return 0;
}

