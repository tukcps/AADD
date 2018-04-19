#include "../src/aadd.h"
#include <assert.h>
#include <stdio.h>



int main()
{
    cout << "Testing of conditional statements ..." << endl; 
    AADD a ( 0, 2 );
    
    
    BDD bdd_cond=(a>=1.0);
    BDD const_cond=true; 
    BDD tbdd; 
    
    ifS (const_cond) {
        a=a+10;
        tbdd = false; 
    }
    elseS {
        a    = a-10;
        tbdd = true; 
    }endS;
    cout << a << tbdd; 


    ifS (false) {
        a    = a+100;
        tbdd = bdd_cond; 
    }
    elseS {
        tbdd = false; 
    }endS;
    cout << a << tbdd; 


    ifS (const_cond) {
        a    = 1; 
        tbdd = true;  
    } endS; 
    cout << a << tbdd;      

}

