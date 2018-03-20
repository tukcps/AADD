#include "../src/aadd.h"
#include <assert.h>
#include <stdio.h>



int main()
{
    AADD a;
    AAF b(0, 2);
    
    a=a+b;
    
    cout << "a= " << a << endl;
    
    BDD c=(a>=1.0);
    
    cout << "c= " << c << endl;
    
    ifS (c)
    {
        a=a-10;
    }
    elseS
    {
        a=a*(-8);
    }endS
    
    cout << "a= " << a << endl;
    
    BDD d=(a<0.);
    
    cout << "d= " <<  d << endl;
    
    ifS(d)
     a=a+12;
    endS
    
    cout << "a= " << a << endl;
    
    AADD g(0.0);
    AAF h(0,2);
    g=g+h;
    
    cout << "g= " << g << endl;
    
    // test symbolic if macros
    boolS e=(g > 1);
    
    cout << "e= " << e << endl;
    
    ifS (e)
    {
      g=2*g;
    }
    elseS
    {
      g=g+a;
    }
    endS
    
    cout << " g ist dann .." << endl << g;
    return 0;
    

    
}



