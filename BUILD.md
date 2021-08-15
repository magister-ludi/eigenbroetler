eigenbrötler is still in development. It builds and runs in Linux, but will
require changes to build and run in Windows.

To compile eigenbrötler, you will need a C++ compiler, as well as the
utilities flex and bison. The build uses cmake, which also need to be
installed, using your package manager, or by downloading cmake from
http://www.cmake.org/cmake/resources/software.html

eigenbrötler also requires the following software to be installed:

 - Qt5
 - cfitsio
 - fftw3

Many Linux distributions will provide suitable packages for these. For
instance openSUSE 12.3 has libQt5Core-devel, libQt5Widgets-devel,
libcfitsio-devel and fftw3-devel. If your distribution doesn't provide these
packages, they are all open source and free. They can be obtained from the
following locations:

 - Qt open source at http://qt-project.org/
 - fftw3 at http://www.fftw.org/
 - cfitsio at http://heasarc.gsfc.nasa.gov/fitsio/

These need to be compiled and installed according to the instructins provided
by the respective web sites.

When the libraries have been installed, eigenbrötler can be compiled using the
following commands at a shell prompt:

    cd directory/where/you/cloned/code
    mkdir build
    cd build && cmake .. && make

If the build is succesful, a library (libeigenbroetler.so) will be constructed
in the build directory, and the application (eigenbroetler) will be
constructed int the directory above it.

There is currently no installer for eigenbrötler. Run the application from the
directory in which is was built.
