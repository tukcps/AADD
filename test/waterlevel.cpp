#include "../src/aadd.h"
#include<assert.h>

//
// A simple, discrete-time water level monitor in C++. 
// - The water level changes with an uncertain rate of 0.8+/-0.2
// - The rate is changed at level 10 and level 2. 
//
int main()
{
  AADD   rate(.8);
  AADD   level(5.0); 
  AAF    uncertainty(-0.2,0.2);

  rate = rate + uncertainty; 

  int hash = 0;

  for (int i=1; i < 100; i++)
  {
    // opt_sol lev = level.GetBothBounds();
    // cout << i << ": " << lev.min << " - " << lev.max << ", no. leafs of level:" << level.numLeaves() << "\n";
    hash += level.numLeaves();

    ifS ( level >= 10.0 )
      rate = -.8+uncertainty; 
    endS; 

    ifS ( level < 2.0 ) 
      rate = .8+uncertainty;
    endS; 

    level = level + rate;
    //cout << "level: " << level; 
  }
  cout << "Hashvalue of numLeafs: " << hash << endl;
  cout << "             expected: 7671" << endl;
  assert (hash == 7671);
}

