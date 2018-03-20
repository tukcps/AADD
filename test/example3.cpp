#include "../src/aadd.h"


intAS get_sign(intAS x)
{
  intAS r; 
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
  intAS input = intAS(-100,100); 
  intAS out = get_sign(input);

  cout << out;
}
