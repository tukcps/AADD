This example is a model of a 3rd delta-sigma modulator. 
The same model is used as a case study in:

[1] T. Dang, A. Donze, and O. Maler, "Verification of Analog and Mixed- Signal Circuits Using Hybrid System Techniques". 
Berlin, Heidelberg: Springer Berlin Heidelberg, 2004, pp. 21–36. [Online]. Available: https://doi.org/10.1007/978- 3- 540- 30494- 4_3

[2] S. Gupta, B. H. Krogh, and R. A. Rutenbar, “Towards Formal Verification of Analog Designs,” 
in IEEE/ACM International Conference on Computer Aided Design, 2004, pp. 210–217. [Online]. Available: http://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=1382573

[3] G. A. Sammane, M. H. Zaki, S. Tahar, and G. Bois, “Constraint-Based Verification of Delta-Sigma Modulators using Interval Analysis,” in 50th Midwest Symposium on Circuits and Systems, 2007, pp. 726–729. [Online]. Available: http://hvg.ece.concordia.ca/Publications/Conferences/MISWEST'07.pdf


Here, it is written in SystemC AMS using the discrete-time TDF model of computation. 
Upper and lower bounds of the quantizer input have been writen to a file.
The file can be plotted with gnuplot. 
There are two scripts for translating it to PDF or to SVG file format. 

To compile and run this example do these steps:

1. go to the modulator folder and create build folder with:

> mkdir build

2. Go to the directory build and build it:

> cd build
> cmake ..
> make 

3. Go to src in the build directory and run executable:

> cd src
> ./run





