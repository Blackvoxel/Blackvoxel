[Blackvoxel](http://blackvoxel.com/)
==========

The story
---------

The story take place in the future. Tired of the civilisation, you decided to retire to a distant world.
You bought a new discovered planet in your local planet selling agency.

The dealer made a good price on a paradisiac planet with beautiful landscape with islands, sandy beaches, palm forest and a beautiful blue sky.
The catalog showed a friendly world populated of peacefull animals.

You take your new ship and go through the long journey.
During the trip, you dreamed of your new world.
And finaly, after a year of boring travell you reached the orbit and started the landing procedure.

Infortunately, you hit an asteroid that was not on the map and your ship was heavily damaged.
The ship wasn't controllable anymore and finally you crashed on your planet.

By chance, you still alive, but most of your equipment and materials was destroyed.

As soon as you looked around you, your realized that the planet wasn't what the dealer promised to you.

What a strange and desolated world with a black sky and strange luminous blocks !!!

All what you see is uncommon and you know nothing about it...

As you look on your personal digital assistant, you noticed you received an important email that arrived using powerfull long distance intergalactic emitters: the agency said that they may have made a mistake because someone confused planet data files.
They said that in the eventuality you remain alive, the sales contract did not mention the details of the surface of the planet, and infortunately, they can't made any exchange and refund.

But before you can do something, you have no choice: you have no intergalactic ship to escape and no suitable communication equipment to query some help.

What you will do now is up to you...

The game
--------

Blackvoxel is a new sandbox game based on a voxel engine.

Developpment started in Aug. 2010 as a hobby research project because we wanted to play with voxel based paradigms.
With time, the bunch of code slowly became a true game project.

The game is written in C/C++ programming langage using OpenGL and running on Linux and Windows operating system.

MacOs version may be planed in the future if users loves it...

Licence
-------
The game is released under the [GNU GPL V3 free licence](https://www.gnu.org/licenses/gpl.html) or (at your option) any later version.

(c) 2010-2014 Laurent Thiebaut and Olivia Merle.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the GNU General Public License for more
details.

(See below for 3rd party libraries copyright and licence).

3rd party library licence
=========================
Blackvoxel include in the source tree it's own modified version of the squirrel langage by Alberto Demichelis.

Here is the copyright and licence for the squirrel project.

Copyright (c) 2003-2012 Alberto Demichelis

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

Compile Blackvoxel from source
==============================

Here are informations for compiling and running Blackvoxel from source.

Don't forget to report any difficulty and errors in the [forum](http://forum.blackvoxel.com/) in the troubleshooting section.

If you've made a port of Blackvoxel or compiled it for an unlisted (or
not yet supported) platform, we would be happy to get feedback to
improve this tutorial.

Linux
-----

Compiling and running blackvoxel with most Linux distributions is very
simple.

See bellow for per-distro step-by-step instructions.

If you want to go further, modify and debug, we recommend using [Eclipse
CDT](https://www.eclipse.org/cdt/).

### Ubuntu

1. Download source tarball (.tar.bz2 source archive) [here](http://blackvoxel.com/view.php?node=1541) and put it in your home directory
2. Open a terminal type: "term" in ubuntu lancher or press 'ctrl-alt-t'
3. Enter the following commands:
```
    sudo apt-get install build-essential libglew-dev libsdl1.2-dev
    tar -xjvf blackvoxel_source*
    cd blackvoxel_source*
    make
```

Once build succeded, you can launch Blackvoxel with the following command:

    cd ~/blackvoxel_source* && ./blackvoxel

### Debian

1. Download source tarball (.tar.bz2 source archive) [here](http://blackvoxel.com/view.php?node=1541) and put it in your home directory
2. Open a terminal:
-   Open the activity menu
-   Type "terminal" on the "type to search" field at the right of the
    screen.
-   Click on the "terminal" (not the "root terminal") icon.
3. Enter the following commands :
```
    sudo apt-get install build-essential libglew-dev libsdl1.2-devexit
    tar -xjvf blackvoxel_source*
    cd blackvoxel_source*
    make
```

Once build succeded, you can launch Blackvoxel with the following
command:

    cd ~/blackvoxel_source* && ./blackvoxel

### openSUSE

    sudo zypper in gcc-c++ glew-devel libSDL-devel
    tar -xf blackvoxel_source*
    make
    ./blackvoxel

### PCLinuxOS

    sudo apt-get install gcc-c++ libglew-devel libSDL-devel
    tar -xf blackvoxel_source*
    make
    ./blackvoxel

### Manjaro

    sudo pacman -S base-devel glew1.10 sdl
    tar xf blackvoxel_source*
    make
    ./blackvoxel

### Other Linux distributions

Blackvoxel can be compiled in debian and ubuntu derived (like Mint) in
the same ways described above.

For non debian distributions, you'll have to find the exact way using
the following indications:

-   Install the gcc with g++ compiler, linker, tools and headers.
-   Find and Install equivalent packages for required libraries with the
    package manager in your Linux Distribution.

We hope to add distributions to this list with time.

### Non Linux various Unix...

No attempt to compile Blackvoxel on other unixes has been done at this
time. In theory, it should work with minimal adaptations.

Anyway, you need to have hardware 3D OpenGL support to run Blackvoxel.

Windows
-------

Infortunately, building Blackvoxel in Windows isn't as simple as in
Linux because lacking of an equivalent software package management
system.

So, it will be harder than in Linux if your aren't realy accustomed with
programming.

Here is what you need to get for compiling Blackvoxel in Window

-   The [mingw](http://www.mingw.org/) gcc compiler suite.
-   Download and compile the following libraries : freeglut, glew,
    libsdl.
-   [Eclipse CDT](https://www.eclipse.org/cdt/) developpment IDE
    (Optionnal).

We have tested successfully the tools listed here for the Windows
version.

Compilation with other developpement tools is untested.

MacOs X
-------

At current time, Blackvoxel hasn't been ported to MacOs (We don't own
any Mac).

Some programmers told us it should work with minimal effort.
We'll post any solution as soon as someone would publish it.
Stay tuned with the forum.



