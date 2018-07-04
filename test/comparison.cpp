#include "../src/aadd.h"
#include <assert.h>
#include <stdio.h>


int main() 
{
    AADD a(0.0);
    AAF  b(0,2);
 
    a = a+b;  // B is now 1+1e1

    BDD condition = (a > 4.0);
    cout << condition;           // must be false
    cout << BDD(true);
    cout << BDD(false);
    
    // ITE statement executes then both true and false parts and generates
    // an AADD with two leaf nodes.
    BDD  con = (a>1.0);
    cout << "con=" << con;
    assert(con.numLeaves() == 2);
    
    cout << a+10;
    cout << a-10;
    cout << con;
    cout << !con;
    a = a.ITE(con, a+10, a-10);
    cout << a;
    
    cout << "a: " << a;
    assert ( a.numLeaves() == 2 );

    BDD cd = (a<5.0);
    cout << cd;
    a=a.ITE(cd, 10, a+1);           // here ...

    AADD g(0.0); 
    AAF h(0,2); 
    g=g+h; 
    
    cout << "g: " << g;

    // test symbolic if macros
    
    boolS e = (g > 1);
    
    ifS (e) 
    {
      g = 5+g; 
    }
    elseS
    {
      g = 9+g; 
    }  endS
    cout << " g ist dann .." << endl << g; 
    return 0; 
}
