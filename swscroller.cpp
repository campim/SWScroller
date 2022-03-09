/*
StarWars Style Simple Scroller
Win32 and UNIX demo
Copyright (C) 2011 Daniel G. Campos

Gstreamer code provided by http://gstreamer.freedesktop.org

Doxygen documented by http://www.stack.nl/~dimitri/doxygen

FreeGlut http://freeglut.sourceforge.net/

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

/** \file swscroller.cpp main file of star wars style simple scroller */

/** \var float linewidth
    \brief width of line by default, can be changed by parameter -lw <valid_float_value>

    \var float scalewidth
    \brief scale of the font, default is 0.010, can be changed by parameter -sw <valid_float_value>   from 0.0 to 1.0

    \var float startx
    \brief X coordinate where the text starts printing from the left. default -3.5, can be changed by parameter -sx

    \var float defaultwidth
    \brief width of the main window, default is 640 , can be changed by parameter -w <valid_integer_value>

    \var float defaultheight
    \brief height of the main window, default is 480, can be changed by parameter -h<valid_integer_value>

    \var float framedelay
    \brief speed of the letters , default is 0.09, can be changed by parameter -fd <valid_float_value>

    \var float delayAux
    \brief auxiliar variable

    \var float inclinacion
    \brief slope of the letters , default is 10, can be changed by parameter -i <valid_integer_valud>

    \var string sArchivo
    \brief name of the text file for the text to be output, set by parameter -t <valid_text_file>

    \var string sLogfile
    \brief name of the log file, not set by default, set by parameter -l <name_log_file>

    \var list stlLista
    \brief template of list of strings

    \var GmainLoop* loop
    \brief variable for Gstreamer functions

    \var GstBus* bus
    \brief variable for Gstreamer functions, main bus pipeline

    \var GstElement* pipeline
    \brief variable for elements for the Gstreamer functions

    \var GstElement* source
    \brief variable for elements for the Gstreamer functions

    \var GstElement* demuxer
    \brief variable for elements for the Gstreamer functions

    \var GstElement* decoder
    \brief variable for elements for the Gstreamer functions

    \var GstElement* conv
    \brief variable for elements for the Gstreamer functions

    \var GstElement* sink
    \brief variable for elements for the Gstreamer functions
*/

#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <list>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <gst/gst.h>
#include <glib.h>
#ifdef WIN32
#include <windows.h>

/** delay : inline function for windows or Linux, depending on the platform to be compiled */
inline void delay( unsigned long ms )
{
    Sleep( ms );
}
#else
#include <unistd.h>
/** delay : inline function for windows or Linux, depending on the platform to be compiled */
inline void delay( unsigned long ms )
{
    usleep( ms * 1000 );
}
#endif

using namespace std;

float           linewidth  = 2.0f;
float           scalewidth = 0.010f;
float           startx = 0.0f;
float           defaultwidth = 640.0;
float           defaultheight = 480.0f;
float           framedelay = 0.09f;
float           delayAux = 0;
float           inclinacion = 10.0f;
string          sArchivo;
list<string>    stlLista;
GMainLoop       *loop;
GstElement      *pipeline, *source, *demuxer, *decoder, *conv, *sink;
GstBus          *bus;

/** bus_call : gstreamer function to manage pipeline */
static gboolean bus_call (GstBus *bus, GstMessage *msg,gpointer data)
{
    GMainLoop *loop = (GMainLoop *) data;

    switch (GST_MESSAGE_TYPE (msg)) {

        case GST_MESSAGE_EOS:
            g_main_loop_quit (loop);
            break;

        case GST_MESSAGE_ERROR: {
            gchar  *debug;
            GError *error;
            gst_message_parse_error (msg, &error, &debug);
            g_free (debug);

            g_printerr ("Error: %s\n", error->message);
            g_error_free (error);

            g_main_loop_quit (loop);
            break;
        }
        default:
            break;
        }

    glutMainLoop();
    return TRUE;
}

/** on_pad_added : function that retrieves a pad from element by name. */
static void on_pad_added (GstElement *element, GstPad *pad,gpointer data)
{
    GstPad *sinkpad;
    GstElement *decoder = (GstElement *) data;

    sinkpad = gst_element_get_static_pad (decoder, "sink");

    gst_pad_link (pad, sinkpad);

    gst_object_unref (sinkpad);
}

/** print_letters : function to print all the letters of the string to the opengl matrix, then pop matrix */
void print_letters (GLfloat x, GLfloat y, const char *format,...)
{
    va_list args;
    char buffer[200], *p;

    va_start(args, format);
        vsprintf(buffer, format, args);
    va_end(args);

    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scalewidth, scalewidth, scalewidth);
    for (p = buffer; *p; p++)
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *p);

    glPopMatrix();
}

/** display : prepares the output for the letters, taking values by default and some other hardcoded, like background color and perspective */
void display(void)
{
    float linea = 1.0f;
    float faspectratio = ((defaultwidth - 64) / (defaultheight - 64));

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPerspective(100.0, faspectratio, 1.0, 20.0);
    glViewport(0, 0, defaultwidth, defaultheight);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    delayAux = delayAux - framedelay;
    delay(88);
    gluLookAt(0.0, delayAux, 4.0,
              0.0, delayAux+inclinacion, 0.0,     /* center moves with delayAux value  */
              0.0, 1.0, 0.0);                     /* in Y direction */
    glPushMatrix();
    glTranslatef(0, 0, -4);


    list<string>::const_iterator it;
    for (it=stlLista.begin(); it!=stlLista.end(); ++it)
    {
        /* calculate long of string, to center*/
        startx = 0.0f;                      /*start calculating in the center*/
        startx = ((*it).length() /2) ;
        startx *= (-1);

        print_letters(startx,  linea--, (*it).c_str());
    }

    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glFlush();
}

/** key : waits for a key pressed , then exit if esc or q pressed */
void key(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27 :
    case 'q':
        gst_element_set_state (pipeline, GST_STATE_NULL);
        gst_object_unref (GST_OBJECT (pipeline));
        exit(EXIT_SUCCESS);
        break;

    default:
        break;
    }

    glutPostRedisplay();
}

/** idle : standard idle function */
void idle(void)
{
    glutPostRedisplay();
}

/** showhelp : shows parameters options from command line */
void showhelp(void)
{
    cout << "sw star wars style simple scroller  - a simple program using FreeGLUT to see a text like star wars movies." << endl;
    cout << "usage : sw <options> " << endl;
    cout << "-m <ogg_file>                :.ogg audio file " << endl;
    cout << "-lw <value>       linewidth  :integer value from 1 to 9 " << endl;
    cout << "-sw <value>       scalewidth :scale of the font, float value from 0.0f to 1.0f " << endl;
    cout << "-sx <value>       startx     :coordinate where the text starts printing from the left, default is -3.5f " << endl;
    cout << "-w <value>        width      :width of the main window default is 640 " << endl;
    cout << "-h <value>        height     :height of the main window, default is 480 " << endl;
    cout << "-fd <value>       framedelay :speed of the letters , default is 0.09 " << endl;
    cout << "-i <value>        slope      :slope of the letters , default is 10 " << endl;
    cout << "-t <txt_file>     txt_file   :name of the text file to be output " << endl;
    cout << "-l                           :Log current session, name of log file has format YYYY-MM-DD-HH-MI-SS.log" << endl;
}

/** main : main function */
int main (int argc, char *argv[])
{
    ifstream        myfile;
    int             i;
    int             aux = 1;
    bool            TEXT_FILE_FOUND = false;
    bool            LOG_FILE = false;
    GMainLoop       *loop;
    streambuf       *old_rdbuf = clog.rdbuf();      //current streambuf
    ofstream        oslogfile;

    /* logging file details */
    char            strdest[25] = {0};
    time_t          curtime = time(0);
    const char      format[]="%F-%H-%M-%S";

    tm              now = *localtime(&curtime);

    strftime(strdest, sizeof(strdest)-1, format, &now);

    strcat(strdest, ".log");

    /*command line parameters */
    for (i=1;argv[i]!=NULL;i++)
    {
        if (strcmp(argv[i],"-lw")==0)               //linewidth : integer parameter from 1 to  9
        {
            linewidth=atoi(argv[i+1]);

            if (linewidth > 9.0f)                   //check if value is within range
                linewidth = 9.0f;
        }
        if (strcmp(argv[i], "-m")==0)
            aux = i + 1;

        if (strcmp(argv[i], "-t")==0)
        {
            sArchivo = argv[i+1];
            TEXT_FILE_FOUND = true;
        }

        if (strcmp(argv[i], "-l")==0)               //name of log file
        {
            LOG_FILE = true;
        }

        if (strcmp(argv[i], "-sw")==0)              //scalewidth : float parameter
            scalewidth=atof(argv[i+1]);
        if (strcmp(argv[i],"-sx")==0)               //startx :
            startx=atof(argv[i+1]);
        if (strcmp(argv[i], "-w")==0)               //defaultwidth :
            defaultwidth=atof(argv[i+1]);
        if (strcmp(argv[i], "-h")==0)               //defaultheigth :
            defaultheight=atof(argv[i+1]);
        if (strcmp(argv[i], "-fd")==0)              //framedelay :
            framedelay=atof(argv[i+1]);
        if (strcmp(argv[i], "-i")==0)               //inclinacion :slope of the letters
            inclinacion=atof(argv[i+1]);
        if (strcmp(argv[i], "--help")==0)           //showhelp :
        {
            showhelp();
            exit(EXIT_SUCCESS);
        }
    }

    if (!TEXT_FILE_FOUND)
    {
        cout << "text file not found. Please use --help for other options." << endl;
        exit(EXIT_FAILURE);
    }

    if (LOG_FILE)
    {
        oslogfile.open(strdest);
        clog.rdbuf(oslogfile.rdbuf());
    }

    /* Initialisation of gstreamer object, this source code came out from gstreamer example http://gstreamer.freedesktop.org/  */
    if (LOG_FILE) clog << "initializing gstreamer..." << endl;
    gst_init (&aux, &argv);

    loop = g_main_loop_new (NULL, FALSE);

    /* Create gstreamer elements */
    if (LOG_FILE) clog << "creating gstreamer elements..." << endl;
    pipeline = gst_pipeline_new ("audio-player");
    source   = gst_element_factory_make ("filesrc",       "file-source");
    demuxer  = gst_element_factory_make ("oggdemux",      "ogg-demuxer");
    decoder  = gst_element_factory_make ("vorbisdec",     "vorbis-decoder");
    conv     = gst_element_factory_make ("audioconvert",  "converter");
    sink     = gst_element_factory_make ("autoaudiosink", "audio-output");

    if (!pipeline || !source || !demuxer || !decoder || !conv || !sink)
    {
        if (LOG_FILE) clog << "Error in the gstreamer elements creation..." << endl;
        g_printerr ("Error in the Gstreamer elements creation...\n");
        exit(EXIT_FAILURE);
    }

    g_object_set (G_OBJECT (source), "location", argv[aux], NULL);

    bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
    gst_bus_add_watch (bus, bus_call, loop);
    gst_object_unref (bus);

    gst_bin_add_many (GST_BIN (pipeline), source, demuxer, decoder, conv, sink, NULL);

    gst_element_link (source, demuxer);
    gst_element_link_many (decoder, conv, sink, NULL);
    g_signal_connect (demuxer, "pad-added", G_CALLBACK (on_pad_added), decoder);

    gst_element_set_state (pipeline, GST_STATE_PLAYING);

    //open menu file
    if (LOG_FILE) clog << "opening text file ..." << endl;
    myfile.open(sArchivo.c_str());

    if (myfile.is_open())
    {
        while (! myfile.eof() )
        {
            getline (myfile,sArchivo);
            if (sArchivo.length() != 0 )
                stlLista.push_back(sArchivo);
        }
        myfile.close();
    }
    else
    {
        if (LOG_FILE)
        {
            clog << "no text file to process." << endl;
            clog.rdbuf(old_rdbuf);
            oslogfile.close();
        }

        cout << "no text file to process. Send filename with -t parameter. " << endl;
        exit(EXIT_FAILURE);
    }

    //initialize glut
    if (LOG_FILE) clog << "initialize glut ..." << endl;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA );
    glutInitWindowSize(defaultwidth, defaultheight);

    glutCreateWindow("Star Wars Style Simple scroller");
    glLineWidth(linewidth);
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(key);

    //gstreamer loop
    g_main_loop_run (loop);

    //close log file
    if (LOG_FILE)
    {
        clog << "closing ..." << endl;
        clog.rdbuf(old_rdbuf);
        oslogfile.close();
    }

    return EXIT_SUCCESS;

}

