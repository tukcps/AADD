# Affine Arithmetic Decition Diagrams (AADD) 
This software provides  a C++ library for the creation and manipulation of affine arithmetic decision diagrams (AADD).
This permits in particular the abstract symbolic interpretation of C++. 
It can also be used for symbolic simulation of SystemC or SystemC AMS/TLM models which requires in addition installation of the SystemC library.

## Usage
Using AADD as simple as: 

```
	#include "aadd.h"
	doubleS a;  // also: AADD a; 
	boolS b;    // or: BDD b; 
        
	int main()

  	{
    	  a = doubleS(0,100); // a takes double value from range [0,100]
	  ifS(a > 1)          // symbolic cond. and iteration statements
	    a = a + 2;
      elseS
	    a = a - 2;
	  endS;
	  
	  b = (a>0);        // b is now a (RO)BDD. 
	  
	  cout << "a is: " << a << " b is: " << b << endl;
	}
```
After execution of this program, a is a decision diagram that represent all possible results, assuming that a was initially from the range [0,100]: The condition at the root node is (a>1), and the leaf nodes have the ranges [-2,98] and [2,102], depending on the condition. Ranges are represented and computed by affine forms to yield scalability. Note that by considering the condition, these ranges can be further reduced significantly to [-2,-1] and [3,102]. This is done by GLPK that improves accuracy of the affine forms, while maintaining scalability.


## Installation  

For installation, do the following steps: 

0. We assume you have a UNIX style system, e.g. OSX or Linux. 

1. Install GLPK e.g. into /usr/local. Get GLPK e.g. from https://www.gnu.org/software/glpk/ 

2. Set an environment variable GLPKLIB to this directory 
```
	> export GLPKLIB=PathToInstallationDirectory (e.g. /usr/local)
```
3. make a directory build in the top level directory 
```
	> mkdir build
```
4. Go to the directory build 
```
	> cd build
```
5. Build it
```	
        > cmake .. 
	> make 
        > make doc
```
In case you want to install the package in another place than 
the default directory (/usr/local) add installation path to cmake command:
```
	> cmake -DCMAKE_INSTALL_PREFIX=PathToInstallationDirectory   .. 
```
The build will also create some (still very incomplete) regression tests. 
To run them, do: 
```
        > ctest
```
If all tests are ok, proceed to 6.
Else, send an email to the authors. 

6. Install it 
```
	> make install
```



In case of problems or other feedback contact:

Carna Zivkovic  
Chair of Design of Cyber-Physical Systems  
TU Kaiserslautern  
Postfach 3049   
67653 Kaiserslautern  
zivkovic@cs.uni-kl.de  
https://cps.cs.uni-kl.de/mitarbeiter/carna-radojicic-msc/  

Christoph Grimm  
Chair of Design of Cyber-Physical Systems  
TU Kaiserslautern  
Postfach 3049  
67653 Kaiserslautern  
grimm@cs.uni-kl.de  
