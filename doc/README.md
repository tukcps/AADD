AADD permit symbolic execution of C++ programs or SystemC models. 
For symbolic execution of a C++ program, the original program must 
include the file aadd.h and link with the aadd library. 
Then, the original data types must be replaced with data types that do symbolic execution: 

* double with doubleS
* float  with floatS
* int    with intAS
* bool   with boolS

Furthermore, control flow statements must be instrumented. 
Below a simple example: 

    #include "aadd.h"

    doubleS a; 
    main() 
    {
        a = doubleS(0,2); // From range [0,2]
        ifS(a>2) 
           a = a + 2; 
        elseS
           a = a - 2; 
        endifS; 
        cout << "a is : " << endl; 
        cout << a; 
    }

AADD can be used with C++ compilers such as g++ or clang. 
To switch symbolic execution off, just include the file aadd_disabled.h.
This will redefine the macros for doubleS, floatS, ifS, etc. to regular C++ statements.

AADD is free software under the GPL license. 

(c) 2017 Kaiserslautern University of Technology

Carna Radojicic, Christoph Grimm
