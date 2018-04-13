#include "../src/aadd.h"
#include <assert.h>

//
// checks copy constructor and some operations with AADD that have only one leaf node.
//
int main()
{
    AADD a;
    a = 2.0;

    AADD b(a);
    AADD c(0);
    AADD d(1);
    
    c = a;      // c must be deleted, a copied to c.
    c = a + b;  // 2 a = 4.
    d = c;      // c still there? ONE still there?
   
    BDD bddtest; // BDD constructor.  
    assert (bddtest == false); 
    bddtest = true; 
    assert (bddtest == true); 

    AADD e(0);   // NULL still there? Mixed up with BDD leafs? 
    AADD f(1);   // ONE still there? 
    
  //  assert(c == 4.0);
    
    // arithmetic operations
    a = -a+1*2.0+b+c/2.3*7-a;
    b = b + 3;
    d = d;
    a = a;
    b = b = c;
    a = 2.3;
    cout << a;
    a = a+3;
    cout << a;
    
    for (int i=1; i<1000000; i++)
    {
        (a = a+1.0);
    }
    cout << a;
    cout << b;

}
