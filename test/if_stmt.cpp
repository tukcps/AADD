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
       
    ifS (const_cond) { // true
        a    = a+10;
        tbdd = false; 
    }
    elseS {
        a    = a-10;
        tbdd = true; 
    }endS;
    // result: a is 11+/-1, tbdd is false. 
    cout << "Expected: 11+-1, false." << endl;  
    cout << a << tbdd; 


    ifS (false) { // false
        a    = a+100;
        tbdd = bdd_cond; 
    }
    elseS {
        // nothing assigned to a. 
        tbdd = false; 
    }endS;
    cout << "Expected: a is 11+/-1, false" << endl;
    cout << a << tbdd; 


    ifS (const_cond) {
        a    = 1; 
        tbdd = true;  
    } endS; 
    cout << "Expected: a is 1, true" << endl; 
    cout << a << tbdd; 


    ifS (bdd_cond) {
       a = a+ 100; 
       tbdd = false; 
    } endS; 
    cout << "Expected: AADD with 101, false for a>1, 1, true else" << endl; 
    cout << a << tbdd; 
}

