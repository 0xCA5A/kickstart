#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include <jack/jack.h>


jack_port_t *line_out_port;
jack_port_t *mic_in_port;

jack_client_t *client;


int process_callback(jack_nframes_t nframes, void *arg) {

    jack_default_audio_sample_t* line_out_audio_samples;
    jack_default_audio_sample_t* mic_in_audio_samples;

    //get output audio buffer
    line_out_audio_samples = jack_port_get_buffer(line_out_port, nframes);

    //get input audio buffer
    mic_in_audio_samples = jack_port_get_buffer(mic_in_port, nframes);

    memcpy(line_out_audio_samples, mic_in_audio_samples, nframes * sizeof(jack_default_audio_sample_t));

    return 0;
}


// JACK calls this shutdown_callback if the server ever shuts down or decides to disconnect the client
void jack_shutdown_callback (void *arg)
{
    exit(1);
}


int main (int argc, char *argv[])
{
    const char *client_name = "enr";
    jack_options_t options = JackNullOption;
    const char *server_name = NULL;

    // open a client connection to the JACK server
    jack_status_t status;
    client = jack_client_open(client_name, options, &status, server_name);
    if (client == NULL) {
        fprintf(stderr, "jack_client_open() failed, "
             "status = 0x%2.0x\n", status);
        if(status & JackServerFailed)
        {
            fprintf (stderr, "Unable to connect to JACK server\n");
        }
        exit(1);
    }
    if (status & JackServerStarted)
    {
        fprintf(stderr, "JACK server started\n");
    }
    if (status & JackNameNotUnique)
    {
        client_name = jack_get_client_name(client);
        fprintf (stderr, "unique name `%s' assigned\n", client_name);
    }

    // tell the JACK server to call 'process()' whenever there is work to be done
    jack_set_process_callback(client, process_callback, 0);

    // tell the JACK server to call 'jack_shutdown()' if it ever shuts down,
    // either entirely, or if it just decides to stop calling us
    jack_on_shutdown(client, jack_shutdown_callback, 0);

    // display the current sample rate.
    printf("engine sample rate: %" PRIu32 "\n", jack_get_sample_rate(client));

    printf("register ports...");
    line_out_port = jack_port_register(client, "connect_me_to_the_output",
                     JACK_DEFAULT_AUDIO_TYPE,
                     JackPortIsOutput, 0);

    mic_in_port = jack_port_register(client, "connect_me_to_the_input",
                      JACK_DEFAULT_AUDIO_TYPE,
                      JackPortIsInput, 0);

    if ((line_out_port == NULL) || (mic_in_port == NULL)) {
        fprintf(stderr, "no more JACK ports available\n");
        exit (1);
    }

    // tell the JACK server that we are ready to roll.
    // our process_callback() callback will start running now
    if (jack_activate(client)) {
        fprintf (stderr, "cannot activate client");
        exit (1);
    }


    //connect input port
    const char **input_ports = jack_get_ports(client, NULL, NULL,
                            JackPortIsPhysical|JackPortIsOutput);
    if (input_ports == NULL) {
            fprintf(stderr, "no physical capture ports\n");
            exit (1);
    }

    if (jack_connect(client, input_ports[0], jack_port_name(mic_in_port))) {
            fprintf (stderr, "cannot connect input port\n");
    }

    free (input_ports);


    // connect output port
    const char **output_ports = jack_get_ports(client, NULL, NULL,
                            JackPortIsPhysical|JackPortIsInput);
    if (output_ports == NULL) {
            fprintf(stderr, "no physical playback ports\n");
            exit (1);
    }

    if (jack_connect(client, jack_port_name(line_out_port), output_ports[0])) {
            fprintf(stderr, "cannot connect output ports\n");
    }

    free (output_ports);

    


    
    // keep running until stopped by the user
    sleep (-1);

    // this is never reached but if the program had some other way
    // to exit besides being killed, they would be important to call
    jack_client_close (client);
    exit (0);
}
