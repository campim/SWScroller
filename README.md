### SWScroller - Simple text scroller like star wars

Thank you for downloading SWScroller - Simple text scroller like star wars.
The idea of this program is to make a simple star wars like text, adding a file for music presentation. 
So you can use an .ogg file with this scroller and a simple text file to make a presentation.

Among with this idea, is also the use of different libraries and tools for c++. So we have a program that uses :

* Simple logging with ofstream
* Use of gstreamer library so we can use .ogg files to listen to music
* Use of FreeGlut glutStrokeCharacter function to draw simple font on the screen
* Use of STL library for a simple list
* Comment the code for doxygen documentation

Created by Daniel G. Campos 2011.

### REQUIREMENTS
* GNU/ Linux system must have to run this software.
* The host system must have openGL, FreeGlut and Gstreamer libraries and all its dependencies, g++ for building.

### LICENSING
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.


### REPORTING BUGS

please send bugs to : 4966631+campim@users.noreply.github.com

### Installation instructions for Debian / Ubuntu systems

Host system must have : Freeglut, GStreamer and dependencies, and also g++  for building.

Freeglut on Debian / Ubuntu : freeglut libraries and source code, run this code on the command line : 

    sudo apt-get install freeglut3 freeglut3-dev


Xfree 86 (some systems need it), run this code on the command line :

    sudo apt-get install libxxf86vm-dev


Gstreamer, run this code on the command line :

    sudo apt-get install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-bad1.0-dev gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl gstreamer1.0-gtk3 gstreamer1.0-qt5 gstreamer1.0-pulseaudio


lib glib, run this code on the command line :

    sudo apt-get install libglib2.0-dev

### Building the application
Move to the folder where the source code is and run this on command line :

    g++ -Wall -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -g -I/usr/include/gstreamer-1.0 -I/usr/lib/x86_64-linux-gnu -c swscroller.cpp -o swscroller.o

link it :

    g++ -L/usr/lib/x86_64-linux-gnu -L/usr/lib/x86_64-linux-gnu/gstreamer-1.0 -L/usr/lib/x86_64-linux-gnu/glib-2.0/include -o swscroller swscroller.o  -pthread -I/usr/include/libmount -I/usr/include/blkid -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -lgthread-2.0 -pthread -lgio-2.0 -lgobject-2.0 -lglib-2.0 -lglut -lGL -lGLU -lXxf86vm -lgobject-2.0 -lgstreamer-1.0


### DOCUMENTATION
In order to create a full documentation doxygen like, you must install it:

    sudo apt-get install doxygen

and then run this command line on the same folder to create a full .HTML documentation for the software:

    doxygen doxygenconf.conf

### EXAMPLES
move to the folder where the application is and run this from command line :

    ./swscroller -m crazy.ogg -lw 2 -sw 0.010 -sx -8.5 -w 1024 -h 768 -t archivo.txt -l -i 1 -fd 0.04**

where :

-m crazy.ogg    : plays the crazy.ogg file if present.<br>
-lw 2           : linewidth 2<br>
-sw 0.010       : scale of the font from 0.0 to 1.0f<br>
-sx 8.5         : coordinate where the text starts.<br>
-w 1024         : width of the window<br>
-h 768          : height of the window<br>
-t archivo.txt  : text file<br>
-l              : logging TRUE<br>
-i 1            : slope of the characters.<br>
-fd 0.04        : speed of the characters.<br>


### About the author

Daniel G. Campos

https://www.linkedin.com/in/danielcampos/
