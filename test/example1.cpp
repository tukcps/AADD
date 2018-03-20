#include "../src/aadd.h"

doubleS a, b, c; 

int main() 
{
    a = doubleS(0,2);      // From range [0,2] => 1+e1 
    ifS(a > 1)
    {
       a = a + 2;
    }
    elseS
    {
      a = a - 2;
    }
    endS;
   
    // expected: 
    // cond: -1 
    // --------> 3+e1  
    // - - - - > -1+e1
    cout << "a is: " << endl; 
    cout << a; 
}
