#include "../src/aadd.h"


intS get_sign(intS x)
{
  intS r; 
  ifS(x == 0)
     r=0; 
  elseS
     r=2;
  endS;

  ifS(x < 0)
    r=-1;
  elseS
    r=1;
  endS; 
  return r;
}

int main() 
{
  intS input = intS(-100,100); 
  intS out = get_sign(input);

  cout << out;
}
