#include "../src/aadd.h"


//
// We check the while loop macro and implementation. 
//

doubleS a, b; 

int main() 
{
    a = doubleS(0,2);   // From range [0,2]
    b = doubleS(10,12); 

    // a is incremented until it reaches 10. 
    // if 10 is reached, no assignment will be made / no stmt. executed. 
    whileS(a < 10)  
    {
        cout << a << endl;
        a = a + 1;
        b = b - 10; 
    } endS
 
    cout << endl << "After loop, a is: " << endl;
    cout << a;
    // Print conditions of a
    a.printConds();
    // Get Conditions of a
    vector<AAF> conds=a.getConds();
    // print a in a.dot file that can be visualised by Graphviz package
    a.printf("a");
    cout << endl << "After loop, b is: " << endl << b; 
    // a leaves now have different representations of 10. 
}
