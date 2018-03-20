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
    whileS(a < 10)      // startwhile(condition);  while ( condition )
    {
        cout << a << endl;
        a = a + 1;
        b = b - 10; 
    } endS
 
    cout << endl << "After loop, a is: " << endl;
    cout << a; 
    cout << endl << "After loop, b is: " << endl << b; 
    // a leaves now have different representations of 10. 
}
