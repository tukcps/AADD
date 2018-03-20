# Affine Arithmetic Decition Diagrams (AADD) 

The AADD package is a C++ library for the creation and 
manipulation of affine arithmetic decision diagrams (AADD).
The package is for abstract symbolic interpretation of C++. 
It can also be added to SystemC or SystemC AMS/TLM.
This requires an additional SystemC library.

## Executive Summary
For the impatient: 
```
	> export GLPKLIB=PathToInstallationDirectory
	> mkdir build
	> cmake ..
	> make 
	> ctest
	> make install
```

## Installation in details 

If it doesn't work, carefully do the following steps: 

0. We assume you have a UNIX style system, e.g. OSX or Linux. 

1. Install GLPK e.g. into /usr/local

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

## Usage

Using AADD as simple as: 

```
	#include "aadd.h"
	doubleS a;  // also with same semantics: AADD a; 
        
	int main()

  	{
    	  a = doubleS(0,100); // a takes double value from range [0,100]
	  ifS(a > 1)          // symbolic cond. and iteration statements
	    a = a + 2;
    	  elseS
	    a = a - 2;
	  endS;
	  cout << "a is: " << endl;
	  cout << a;
	}
```

FEEDBACK:

Send feedback to:

Carna Radojicic  
Chair of Design of Cyber-Physical Systems  
TU Kaiserslautern  
Postfach 3049   
67653 Kaiserslautern  
radojicic@cs.uni-kl.de  
https://cps.cs.uni-kl.de/mitarbeiter/carna-radojicic-msc/  

Christoph Grimm  
Chair of Design of Cyber-Physical Systems  
TU Kaiserslautern  
Postfach 3049  
67653 Kaiserslautern  
grimm@cs.uni-kl.de  
