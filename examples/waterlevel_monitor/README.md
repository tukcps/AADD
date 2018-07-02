This example is a model of a famous water level monitor. 
It is written in SystemC AMS using the discrete-time TDF model of computation. 
It writes a file that has the upper and lower bounds of the water level in it. 
The file can be plotted with gnuplot. 
There are two scripts for translating it to PDF or to SVG file format.

To compile and run this example do these steps:

1. go to the waterlevel_monitor folder and create build folder with:

  > mkdir build

2. Go to the directory build and build it:

 > cd build
 > cmake ..
 > make 

3. Go to src in the build directory and run executable:

 > cd src
 > ./run



