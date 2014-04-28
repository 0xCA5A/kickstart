
#include "debug.h"

#include "JackClient.h"
#include <jack/jack.h>
#include <stdlib.h>

#include <math.h>
#include <iostream>


const std::string JackClient::s_jackClientLeftChannelInputPortName = "leftChannelInputPort";
const std::string JackClient::s_jackClientRightChannelInputPortName = "rightChannelInputPort";
const std::string JackClient::s_jackClientName = "mpg123HMIEqualizer";
const std::string JackClient::s_portNameToConnectLeftChannelRegex = "mpg123-.*:left";
const std::string JackClient::s_portNameToConnectRightChannelRegex = "mpg123-.*:right";

volatile bool* JackClient::m_p_runFlag;
jack_client_t* JackClient::m_p_jackClient;
jack_port_t* JackClient::m_p_leftChannelInputPort;
jack_port_t* JackClient::m_p_rightChannelInputPort;
jack_default_audio_sample_t JackClient::m_lastAudioSample;


void JackClient::connectionJackCallback(jack_port_id_t a, jack_port_id_t b,
					int connect, void *arg)
{
    static uint32_t callCounter = 0;

    // super cheap way to exit client on disconnect... :)
    // do not reconnect, just exit gracefully
    if (callCounter >= 2) {
        *m_p_runFlag = false;
    }
    callCounter++;
}


void JackClient::shutdownJackCallback(void *arg)
{
    (void)arg;
    shutdown();
}


void JackClient::shutdown()
{
    jack_client_close(m_p_jackClient);
    *m_p_runFlag = false;
}


bool JackClient::registerPorts()
{
    DEBUG_PRINT("register input port %s", s_jackClientLeftChannelInputPortName.c_str());
    m_p_leftChannelInputPort = jack_port_register(m_p_jackClient, s_jackClientLeftChannelInputPortName.c_str(),
                   JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);

    DEBUG_PRINT("register input port %s", s_jackClientRightChannelInputPortName.c_str());
    m_p_rightChannelInputPort = jack_port_register(m_p_jackClient, s_jackClientRightChannelInputPortName.c_str(),
                   JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);

    if (m_p_leftChannelInputPort == NULL || m_p_rightChannelInputPort == NULL ) {
        return false;
    }

    return true;
}


bool JackClient::activateClientAndAutoConnectPorts()
{
    //Tell the JACK server that we are ready to roll. our
    // process() callback will start running now!
    if (jack_activate(m_p_jackClient)) {
        DEBUG_PRINT("[!] cannot activate client, exit immediately");
        return false;
    }

    return autoConnectPortsToMPG123();
}


bool JackClient::autoConnectPortsToMPG123()
{
    const char **pp_ports;
    int retval;

    // connect left channel
    pp_ports = jack_get_ports(m_p_jackClient,
                  s_portNameToConnectLeftChannelRegex.c_str(),
                  NULL, JackPortIsOutput);
    if (pp_ports == NULL) {
        DEBUG_PRINT("[!] no output port with name %s found (regex)",
                    s_portNameToConnectLeftChannelRegex.c_str());
    return false;
    }
    retval = jack_connect(m_p_jackClient, pp_ports[0],
                  jack_port_name(m_p_leftChannelInputPort));
    free(pp_ports);
    if (retval) {
        DEBUG_PRINT("[!] cannot connect input port %s (%d)",
                jack_port_name(m_p_leftChannelInputPort), retval);
    return false;
    } else {
        DEBUG_PRINT("successfully connected %s to %s",
                s_portNameToConnectLeftChannelRegex.c_str(),
                jack_port_name(m_p_leftChannelInputPort));
    }

    // connect right channel
    pp_ports = jack_get_ports(m_p_jackClient,
                  s_portNameToConnectRightChannelRegex.c_str(),
                  NULL, JackPortIsOutput);
    if (pp_ports == NULL) {
        DEBUG_PRINT("[!] no output port with name %s found (regex)",
                s_portNameToConnectRightChannelRegex.c_str());
    return false;
    }
    retval = jack_connect(m_p_jackClient, pp_ports[0],
                  jack_port_name(m_p_rightChannelInputPort));
    free(pp_ports);
    if (retval) {
        DEBUG_PRINT("[!] cannot connect input port %s (%d)",
                jack_port_name(m_p_rightChannelInputPort), retval);
    return false;
    } else {
        DEBUG_PRINT("successfully connected %s to %s",
                s_portNameToConnectRightChannelRegex.c_str(),
                jack_port_name(m_p_rightChannelInputPort));
    }

    return true;
}



bool JackClient::setup(volatile bool* p_runFlag, uint32_t nrOfFramesInRingBuffer)
{
    m_p_runFlag = p_runFlag;

    const char *server_name = NULL;
    jack_options_t options = JackNoStartServer;
    jack_status_t status;

    // create a jack client
    m_p_jackClient = jack_client_open(s_jackClientName.c_str(), options, &status, server_name);
    if (m_p_jackClient == NULL) {
        DEBUG_PRINT("jack_client_open() failed, status = 0x%2.0x",
                status);
        if (status & JackServerFailed) {
            DEBUG_PRINT("unable to connect to JACK server");
        }
        return false;
    }
    if (status & JackServerStarted) {
        DEBUG_PRINT("jack server up and running");
    }
    if (status & JackNameNotUnique) {
        char* client_name = jack_get_client_name(m_p_jackClient);
        DEBUG_PRINT("unique name `%s' assigned", client_name);
    }


    // create ringbuffer
    m_p_ringBuffer = jack_ringbuffer_create(sizeof(jack_default_audio_sample_t) * jack_get_buffer_size(m_p_jackClient) * nrOfFramesInRingBuffer);
    if (m_p_ringBuffer == NULL) {
        DEBUG_PRINT("failed to create ringbuffer");
        return false;
    }

    // setup callbacks
    int check;
    if ((check = jack_set_process_callback(m_p_jackClient, JackClient::processJackCallback, this))) {
        return false;
    }

    jack_on_shutdown(m_p_jackClient, JackClient::shutdownJackCallback, this);

    if ((check = jack_set_port_connect_callback(m_p_jackClient, JackClient::connectionJackCallback, this))) {
        return false;
    }

    if (!registerPorts()) {
        return false;
    }


    return true;
}



int JackClient::processJackCallback(jack_nframes_t nFrames, void* arg)
{
    JackClient* jackClient = static_cast<JackClient*>(arg);

    jack_default_audio_sample_t* p_leftChannelInputAudioSamples = reinterpret_cast<jack_default_audio_sample_t*>(jack_port_get_buffer(m_p_leftChannelInputPort, nFrames));
    jack_default_audio_sample_t* p_rightChannelInputAudioSamples = reinterpret_cast<jack_default_audio_sample_t*>(jack_port_get_buffer(m_p_rightChannelInputPort, nFrames));
    jack_default_audio_sample_t sampleBuffer[nFrames];

    uint32_t sampleIndex = 0;
    double avgSampleValue = 0;
    for (sampleIndex = 0; sampleIndex < nFrames; sampleIndex++) {
        avgSampleValue += fabs(p_leftChannelInputAudioSamples[sampleIndex]);
        avgSampleValue += fabs(p_rightChannelInputAudioSamples[sampleIndex]);

    }
    m_lastAudioSample = avgSampleValue / (2 * nFrames);

    // get avg of the two channels
    // TODO(scasa):check if this is right...
    for (uint32_t i = 0; i < nFrames; i++) {
        sampleBuffer[i] = (p_leftChannelInputAudioSamples[i] + p_rightChannelInputAudioSamples[i]) / 2;
    }

    const uint32_t writeSizeInByte = nFrames * sizeof(jack_default_audio_sample_t);
    if (jack_ringbuffer_write_space(jackClient->getRingBuffer()) >= writeSizeInByte) {
//         DEBUG_PRINT("write to ringbuffer, %d byte", writeSizeInByte);
        jack_ringbuffer_write(jackClient->getRingBuffer(), reinterpret_cast<const char*>(sampleBuffer), writeSizeInByte);
    }

    return 0;
}