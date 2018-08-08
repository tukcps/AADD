/*
 * aa_aafapprox.cpp -- Standart non-affine operations
 * Copyright (c) 2003 EPFL (Ecole Polytechnique Federale de Lausanne)
 * Copyright (c) 2004 LIRIS (University Claude Bernard Lyon 1)
 * Copyright (C) 2009 LUH (Leibniz Universitaet Hannover)
 *
 * This file is part of libaa.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with libaa; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

// {{{ Revisions 
//
// $Log: aa_aafapprox.c,v $
//
// }}} 

#include "aa.h"
#include <cmath>
#include <algorithm>

//#define AAF_DEBUG
#define EPSILON 1E-20
#define AAF_MINRAD 1E-10

double minimum(const double val1, const double val2)
{
    if (val1<val2)
        return val1;
    else
        return val2;
}

double maximum(const double val1, const double val2)
{
    if (val1>val2)
        return val1;
    else
        return val2;
}


/************************************************************
 * INTERPOLATION INFO
 *
 * a is the lower bound of the argument AAF
 * b is the upper bound of the argument AAF
 *
 * alpha is the slope of the line r(x) that interpolates
 * i)  CHEBYSHEV: (a, f(a)) and (b, f(b))
 * ii) MINRANGE:  MIN(f'(a), f'(b))
 *
 * dzeta is the central y-intercept of the parallelogram 
 * including (a,f(a) .. b,f(b))
 *
 * delta is the new deviation value being half a hight of
 * the parallelogram
 ************************************************************/

/************************************************************
 * Operator:      *=
 * Author & Date: Darius Grabowski - 2006
 * Description:   
 *   computes the affine product (*this *= P)
 *
 *   Input  : const AAF & : AAF argument
 *   Output : AAF &       : AAF result
 ************************************************************/
AAF & AAF::operator *=(const AAF & P)
{
 
  unsigned l1 = length;
  unsigned l2 = P.length;
  
  if (l1+l2==0)
  {
    cvalue*=P.cvalue;
    return *this;
  }

  if (l1 == 0)
  {
    // if *this is double
    double temp = cvalue;
    (*this) = P;
    (*this) *= temp;
    return (*this);
  }
  if (l2 == 0)
  {
    // if P is double
    (*this) *= P.cvalue;
    return (*this);
  }

  unsigned * id1 = indexes;
  unsigned * id2 = P.indexes;

  double * va1 = deviations;
  double * va2 = P.deviations;

  unsigned * pu1 = id1;
  unsigned * pu2 = id2;

  unsigned * tempIndexes = new unsigned [l1+l2];
  
  unsigned * idtemp = tempIndexes;

  // Fill the indexes array
  unsigned * fin = std::set_union(id1, id1 + l1, id2, id2 + l2, idtemp);
  unsigned ltemp = fin - idtemp;

  double * tempDeviations = new double [ltemp];
  double * vatempg = tempDeviations;
  
  //double commonTermCenter = 0.0;
  //double commonTermDeviation = 0.0;
  
  double min1, max1;
  double min2, max2;

  min1=cvalue;
  max1=cvalue;
  
  min2=P.cvalue;
  max2=P.cvalue; 
  
  double min, max;

  // Fill the deviations array

  for (unsigned i = 0; i < ltemp; i++)
  {
    unsigned a = pu1 - id1;
    unsigned b = pu2 - id2;
    
    if (a == l1 || id1[a] != idtemp[i])
    {
      vatempg[i] = cvalue*va2[b];  // cvalue*va2[b]+(P.cvalue)*0
      
      if (vatempg[i]>0)
      {
        min2-=va2[b];
        max2+=va2[b];
        
      }
      else
      {
        min2+=va2[b];
        max2-=va2[b]; 
      }
      
      pu2++;
      continue;
    }
    
    if (b == l2 || id2[b] != idtemp[i])
    {
      vatempg[i] = (P.cvalue)*va1[a];  // cvalue*0+(P.cvalue)*va1[a]
      
      if (vatempg[i]>0)
      {
        min1-=va1[a];
        max1+=va1[a];
        
      }
      else
      {
        min1+=va1[a];
        max1-=va1[a]; 
      }
      
      pu1++;
      continue;
    }
    
    vatempg[i] = cvalue*va2[b] + (P.cvalue)*va1[a];

    if ( vatempg[i]<0)
    {
      min1+=va1[a];
      max1-=va1[a];
      
      min2+=va2[b];
      max2-=va2[b];
    
    }
    else
    {
      
      min1-=va1[a];
      max1+=va1[a];
      
      min2-=va2[b];
      max2+=va2[b];
    
    }
   
    pu1++;
    pu2++;
  }
    
    // add offsets of (*this) and P

    if (min1>max1)
    {
        min1+=offset_max;
        max1+=offset_min;
        
    }
    else
    {
        min1+=offset_min;
        max1+=offset_max;
    }
    
    if (min2>max2)
    {
        min2+=P.offset_max;
        max2+=P.offset_min;
    }
    else
    {
        min2+=P.offset_min;
        max2+=P.offset_max;
    }
    
    min=min1*min2;
    max=max1*max2;
    
  offset_min=0;
  offset_max=0;
  
  cvalue *= P.cvalue;

  length = ltemp;
  size = length;

  delete [] indexes;
  delete [] deviations;

  indexes = tempIndexes;
  deviations = tempDeviations;

#ifdef FAST_RAD
  radius = 0.0;
  for (unsigned i = 0; i < length; i++)
    radius += fabs(deviations[i]);
#endif

  if (AAF::approximationType == INTERVAL_EXACT){
   offset_min=min-getMin();
   offset_max=max-getMax();}
  else{
  offset_min=minimum(min,getMin())-getMin();
  offset_max=maximum(max,getMax())-getMax();}
 
  return (*this);    
}

/************************************************************
 * Operator:      *
 * Author & Date: ??? - ???
 * Description:   
 *   computes the affine product (*this * P)
 *
 *   Input  : const AAF & : AAF argument
 *   Output : AAF         : AAF result
 ************************************************************/
AAF AAF::operator * (const AAF & P) const
{

  unsigned l1 = length;
  unsigned l2 = P.length;
  
  if (l1+l2==0)
  {
    AAF Temp(cvalue*P.cvalue);
    return Temp;
  }

  if (l1 == 0)
  {
    // if *this is double
    AAF Temp(P);
    Temp *= cvalue;
    return Temp;
  }
  if (l2 == 0)
  {
    // if P is double
    AAF Temp(*this);
    Temp *= P.cvalue;
    return Temp;
  }

  unsigned * id1 = indexes;
  unsigned * id2 = P.indexes;

  double * va1 = deviations;
  double * va2 = P.deviations;

  unsigned * pu1 = id1;
  unsigned * pu2 = id2;
  
  AAF Temp(cvalue*P.cvalue);

  Temp.indexes = new unsigned [l1+l2];
  
  unsigned * idtemp = Temp.indexes;

  // Fill the indexes array
  unsigned * fin = std::set_union(id1, id1 + l1, id2, id2 + l2, idtemp);
  unsigned ltemp = fin - idtemp;

  Temp.deviations = new double [ltemp];
  double * vatempg = Temp.deviations;
  
  double min1, max1;
  double min2, max2;

  min1=cvalue;
  max1=cvalue;
  
  min2=P.cvalue;
  max2=P.cvalue; 
  
  double min, max;

  // Fill the deviations array

  for (unsigned i = 0; i < ltemp; i++)
  {
    unsigned a = pu1 - id1;
    unsigned b = pu2 - id2;
    
    if (a == l1 || id1[a] != idtemp[i])
    {
      vatempg[i] = cvalue*va2[b];  // cvalue*va2[b]+(P.cvalue)*0
      
      if (vatempg[i]>0)
      {
        min2-=va2[b];
        max2+=va2[b];
        
      }
      else
      {
        min2+=va2[b];
        max2-=va2[b]; 
      }
      
      pu2++;
      continue;
    }
    
    if (b == l2 || id2[b] != idtemp[i])
    {
      vatempg[i] = (P.cvalue)*va1[a];  // cvalue*0+(P.cvalue)*va1[a]
      
      if (vatempg[i]>0)
      {
        min1-=va1[a];
        max1+=va1[a];
        
      }
      else
      {
        min1+=va1[a];
        max1-=va1[a]; 
      }
      
      pu1++;
      continue;
    }
    
    vatempg[i] = cvalue*va2[b] + (P.cvalue)*va1[a];
      
    if ( vatempg[i]<0)
    {
      min1+=va1[a];
      max1-=va1[a];
        
      min2+=va2[b];
      max2-=va2[b];
    
    }
    else
    {
      
      min1-=va1[a];
      max1+=va1[a];
      
      min2-=va2[b];
      max2+=va2[b];
    
    }
   
    pu1++;
    pu2++;
  }

    // add offsets of (*this) and P
    
    if (min1>max1)
    {
        min1+=offset_max;
        max1+=offset_min;
        
    }
    else
    {
        min1+=offset_min;
        max1+=offset_max;
    }
    
    if (min2>max2)
    {
        min2+=P.offset_max;
        max2+=P.offset_min;
    }
    else
    {
        min2+=P.offset_min;
        max2+=P.offset_max;
    }
    
    
     min=min1*min2;
     max=max1*max2;
    
    Temp.length = ltemp;
    Temp.size = ltemp;

#ifdef FAST_RAD
  Temp.radius = 0.0;
  for (unsigned i = 0; i < Temp.length; i++)
    Temp.radius += fabs(Temp.deviations[i]);
#endif
   
  if (AAF::approximationType == INTERVAL_EXACT){
    Temp.offset_min=min-Temp.getMin();
    Temp.offset_max=max-Temp.getMax();
  }else{
    Temp.offset_min=minimum(min, Temp.getMin())-Temp.getMin();
    Temp.offset_max=maximum(max, Temp.getMax())-Temp.getMax();}
    
    return Temp;

}


/************************************************************
 * Operator:      /=
 * Author & Date: ??? - ???
 * Description:
 *   computes the division, we use the identity x/y = x * (1/y)
 *
 *   Input  : const AAF & : AAF argument
 *   Output : AAF         : AAF result
 ************************************************************/
AAF & AAF::operator /= (const AAF & P)
{
    if (this==&P)
    {
        cvalue=1.0;
        length=0;
        size=0;
        
        offset_min=0;
        offset_max=0;
        
        delete [] indexes;
        delete [] deviations;

       #ifdef FAST_RAD
        radius = 0.0;
       #endif
       
       return *this;
        
    }
    else
    {
     
     double fa1, fa2; // max 
     
     double fb1, fb2; // min
     
     double fa, fb;
     
     unsigned l1=length; 
     
     unsigned l2=P.length;
     
     if (l1==0 and l2==0)
     {
       (*this)/=P.cvalue;
       return *this;
     }
     
     if (l1==0)
     {
       *this=cvalue*inv(P);
       return *this;
     }
     
     if (l2==0)
     {
       *this/=P.cvalue;
       return *this;
     }
     
     AAF Temp=(*this)*inv(P);
     Temp.offset_min=Temp.offset_max=0;
 
     unsigned ltemp=Temp.length;
     
     unsigned *idtemp=Temp.indexes;
     
     double *vatempg=Temp.deviations;
     
     unsigned * id1 = indexes;
     unsigned * id2 = P.indexes;

     double * var1 = deviations;
     double * var2 = P.deviations;

     unsigned * pu1 = id1;
     unsigned * pu2 = id2;
     
     fa1=fb1=cvalue;
     
     fa2=fb2=P.cvalue;
     
    for (unsigned i = 0; i < ltemp; i++)
    {
    unsigned a = pu1 - id1;
    unsigned b = pu2 - id2;
    
    if (a == l1 || id1[a] != idtemp[i])
    {
      if (vatempg[i]>0)
      {
        fa2+=var2[b];
        fb2-=var2[b];
      
      }
      else
      {
         fa2-=var2[b];
         fb2+=var2[b];
      }
      
      pu2++;
      continue;
    }
    
    if (b == l2 || id2[b] != idtemp[i])
    {
      if (vatempg[i]>0)
      {
        fa1+=var1[a];
        fb1-=var1[a];
      
      }
      else
      {
         fa1-=var1[a];
         fb1+=var1[a];
      }
      
      pu1++;
      continue;
    }
    
      if (vatempg[i]>0)
      {
      
        fa2+=var2[b];
        fb2-=var2[b];
        
        fa1+=var1[a];
        fb1-=var1[a];
        
        
      
      }
      else
      {
    
         fa2-=var2[b];
         fb2+=var2[b];
         
         fa1-=var1[a];
         fb1+=var1[a];
          
        
      }
    
      
    pu1++;
    pu2++;
   
   }
        
    if (fb1>fa1)
    {
        fb1+=offset_max;
        fa1+=offset_min;
    }
    else
    {
        fb1+=offset_min;
        fa1+=offset_max;
    }
        
    if (fb2>fa2)
    {
        fb2+=P.offset_max;
        fa2+=P.offset_min;
    }
    else
    {
        fb2+=P.offset_min;
        fa2+=P.offset_max;
    }

   
       
    fa=fa1/fa2; // min
    fb=fb1/fb2; // max
   
   
    #ifdef FAST_RAD
    Temp.radius = 0.0;
    for (unsigned i = 0; i < Temp.length; i++)
    Temp.radius += fabs(Temp.deviations[i]);
    #endif
        
        
   if (AAF::approximationType == INTERVAL_EXACT){
    Temp.offset_min=fb-Temp.getMin();
    Temp.offset_max=fa-Temp.getMax();}
   else{
    Temp.offset_min=minimum(fb, Temp.getMin())-Temp.getMin();
    Temp.offset_max=maximum(fa,Temp.getMax())-Temp.getMax();}

    (*this)=Temp;
  }
    
  return (*this);
    
}

/************************************************************
 * Operator:      /
 * Author & Date: ??? - ???
 * Description:   
 *   computes the division, we use the identity x/y = x * (1/y)
 *
 *   Input  : const AAF & : AAF argument
 *   Output : AAF         : AAF result
 ************************************************************/
AAF AAF::operator / (const AAF & P) const
{

    if (this==&P)
    {
        AAF Temp(1.0);
       
       return Temp;
        
    }
    else
    {
     
     double fa1, fa2; // max 
     
     double fb1, fb2; // min
     
     double fa, fb;
     
     unsigned l1=length; 
     
     unsigned l2=P.length;
     
     if (l1==0 and l2==0)
     {
       AAF Temp(cvalue/P.cvalue);
       return Temp;
     }
     
     if (l1==0)
     {
       AAF Temp=cvalue*inv(P);
       return Temp;
     }
     
     if (l2==0)
     {
       AAF Temp=(*this)/P.cvalue;
       return Temp;
     }
     
     AAF Temp=(*this)*inv(P);
     Temp.offset_min=Temp.offset_max=0;
     
     unsigned ltemp=Temp.length;
     
     unsigned *idtemp=Temp.indexes;
     
     double *vatempg=Temp.deviations;
     
     unsigned * id1 = indexes;
     unsigned * id2 = P.indexes;

     double * var1 = deviations;
     double * var2 = P.deviations;

     unsigned * pu1 = id1;
     unsigned * pu2 = id2;
     
     fa1=fb1=cvalue;
     
     fa2=fb2=P.cvalue;
     
    for (unsigned i = 0; i < ltemp; i++)
    {
    unsigned a = pu1 - id1;
    unsigned b = pu2 - id2;
    
    if (a == l1 || id1[a] != idtemp[i])
    {
      if (vatempg[i]>0)
      {
        fa2+=var2[b];
        fb2-=var2[b];
      
      }
      else
      {
         fa2-=var2[b];
         fb2+=var2[b];
      }
      
      pu2++;
      continue;
    }
    
    if (b == l2 || id2[b] != idtemp[i])
    {
      if (vatempg[i]>0)
      {
        fa1+=var1[a];
        fb1-=var1[a];
      
      }
      else
      {
         fa1-=var1[a];
         fb1+=var1[a];
      }
      
      pu1++;
      continue;
    }
    
    if (vatempg[i]>0)
    {

        fa2+=var2[b];
        fb2-=var2[b];
        
        fa1+=var1[a];
        fb1-=var1[a];
        
        
      
      }
    else
    {
    
        fa2-=var2[b];
        fb2+=var2[b];
         
        fa1-=var1[a];
        fb1+=var1[a];
          
        
    }
    
    pu1++;
    pu2++;
   
   }
   
    if (fb1>fa1)
    {
        fb1+=offset_max;
        fa1+=offset_min;
    }
    else
    {
        fb1+=offset_min;
        fa1+=offset_max;
    }
        
    if (fb2>fa2)
    {
        fb2+=P.offset_max;
        fa2+=P.offset_min;
    }
    else
    {
        fb2+=P.offset_min;
        fa2+=P.offset_max;
    }
        
       
       
    fa=fa1/fa2;
    fb=fb1/fb2;
        
    
    #ifdef FAST_RAD
    Temp.radius = 0.0;
    for (unsigned i = 0; i < Temp.length; i++)
    Temp.radius += fabs(Temp.deviations[i]);
    #endif
        
   if (AAF::approximationType == INTERVAL_EXACT){
    Temp.offset_min=fb-Temp.getMin();
    Temp.offset_max=fa-Temp.getMax();}
   else{
    Temp.offset_min=minimum(fb, Temp.getMin())-Temp.getMin();
    Temp.offset_max=maximum(fa,Temp.getMax())-Temp.getMax();}
        
   
    return Temp;
     
  }
  
}


/************************************************************
 * Operator:      /
 * Author & Date: Darius Grabowski - 10.06.2005
 * Description:   
 *   computes the division <double> / <AAF>
 *
 *   Input  : double    : divisor
 *            AAF       : dividend
 *   Output : AAF       : AAF result
 ************************************************************/
AAF operator / (double cst, const AAF P)
{

  AAF Temp=inv(P)*cst;
  return Temp;
 
    
}

/************************************************************
 * Operator:      ^
 * Author & Date: Darius Grabowski - 10.06.2005
 * Description:   
 *   computes the power function
 *
 *   Input  : const int : exponent
 *   Output : AAF       : AAF result
 ************************************************************/
AAF AAF::operator ^ (const int exponent) const
{
  double a, b;
  double fa, fb;
  double r;
  double alpha = 0.0;
  double dzeta = 0.0;
  double delta = 0.0;

  if (length == 0)
  {
    AAF Temp(pow((double)cvalue, double(exponent)));
    return Temp;
  }

  if (exponent == 0)
  {
    return AAF(1.0);
  }
  
  if (exponent == 1)
  {
    return *this;
  }

  if (exponent == -1)
  {
    return inv(*this);
  }

  r = rad();

  a = cvalue - r;
  b = cvalue + r;

  fa = pow(a, (double)exponent);
  fb = pow(b, (double)exponent);

  if ( (b*a < EPSILON) && (exponent < 0) )
  {
    throw(AAF_Exception(AAF_DIVZERO_EXCEPTION, 
			std::string("Error <AAF::^>: computing zero inversion").c_str(), 
			a, b));
  }

  if (AAF::approximationType == CHEBYSHEV)
  {
    // use Chebyshev approximation
    if (r > AAF_MINRAD)
    {
      alpha = (fb-fa)/(b-a);
    }
    else
    {
      // avoid division by zero
      alpha = exponent*fa/(a+EPSILON);
    }
    
    // we have two points having the slope alpha
    double x_1 = -pow(fabs((double)alpha/exponent), (double)1.0/(exponent-1.0));

    double x_2 = -x_1;

    double fx_1, fx_2;
    
    // check valid points
    if (x_1 > a)
    {
      // x_1 is valid
      fx_1 = pow(x_1, (double)exponent);
    }
    else
    {
      x_1 = a;
      fx_1 = fa;
    }
    
    if (x_2 < b)
    {
      // x_2 is valid
      fx_2 = pow(x_2, (double)exponent);
    }
    else
    {
      x_2 = b;
      fx_2 = fb;
    }
    
    double y_2 = fx_2 - alpha*x_2;
    double y_1 = fx_1 - alpha*x_1;
    
    delta = 0.5*(y_1 - y_2);
    
    dzeta = 0.5*(y_1 + y_2);
#ifdef AAF_DEBUG
    fprintf(stdout, "Function: x^%d\n", exponent);
    fprintf(stdout, "Interval: [%f, %f]\n", a, b);
    fprintf(stdout, "f(Interval): [%f, %f]\n", fa, fb);
    fprintf(stdout, "x_1: %f, f(x_1): %f\n", x_1, fx_1);
    fprintf(stdout, "x_2: %f, f(x_2): %f\n", x_2, fx_2);
    fprintf(stdout, "alpha: %f, delta: %f, dzeta: %f\n", alpha, delta, dzeta);
#endif
  }
  else if (AAF::approximationType == MINRANGE)
  {
    // use MinRange approximation

    // special case: 0.0 in [a,b] : alpha = f'(0.0)
    // exp > 0 and exp even and [a,b] > 0 : MINRANGE: alpha = f'(a), y0_b > y0_a : e1 = 1 e2 = 1 e3 = 1
    // exp > 0 and exp odd  and [a,b] > 0 : MINRANGE: alpha = f'(a), y0_b > y0_a : e1 = 1 e2 = 0 e3 = 1
    // exp > 0 and exp even and [a,b] < 0 : MINRANGE: alpha = f'(b), y0_a > y0_b : e1 = 1 e2 = 1 e3 = 0
    // exp > 0 and exp odd  and [a,b] < 0 : MINRANGE: alpha = f'(b), y0_b > y0_a : e1 = 1 e2 = 0 e3 = 0
    // exp < 0 and exp even and [a,b] > 0 : MINRANGE: alpha = f'(b), y0_a > y0_b : e1 = 0 e2 = 1 e3 = 1
    // exp < 0 and exp odd  and [a,b] > 0 : MINRANGE: alpha = f'(b), y0_a > y0_b : e1 = 0 e2 = 0 e3 = 1
    // exp < 0 and exp even and [a,b] < 0 : MINRANGE: alpha = f'(a), y0_b > y0_a : e1 = 0 e2 = 1 e3 = 0
    // exp < 0 and exp odd  and [a,b] < 0 : MINRANGE: alpha = f'(a), y0_a > y0_b : e1 = 0 e2 = 0 e3 = 0
    
    if (b*a < 0.0)
    {
      alpha = 0.0;
      if (exponent % 2 == 0)
      {
	delta = 0.5*((fa>fb)?fa:fb);
	dzeta = delta;
      }
      else
      {
	delta = 0.5*(fb - fa);
	dzeta = 0.5*(fb + fa);
      }
    }
    else
    {
      if ( ( (exponent > 0) && (a > 0.0) ) || ( (exponent < 0) && (a < 0.0) ) )
      {
	alpha = exponent*fa/(a+EPSILON);
      }
      else
      {
	alpha = exponent*fb/(b+EPSILON);
      }
      
      double y_a = fa-alpha*a;
      double y_b = fb-alpha*b;

      delta = 0.5*fabs(y_a - y_b);
      
      dzeta = 0.5*(y_a + y_b);

#ifdef AAF_DEBUG
      fprintf(stdout, "Function: x^%d\n", exponent);
      fprintf(stdout, "Interval: [%f, %f]\n", a, b);
      fprintf(stdout, "f(Interval): [%f, %f]\n", fa, fb);
      fprintf(stdout, "alpha: %f, delta: %f, dzeta: %f\n", alpha, delta, dzeta);
#endif
    }
  }
  else // (AAF::approximationType == SECANT)
  {
    if (r > AAF_MINRAD)
    {
      alpha = (fb-fa)/(b-a);
    }
    else
    {
      // avoid division by zero
      alpha = exponent*fa/(a+EPSILON);
    }
    dzeta = fa - alpha*a;
    delta = 0.;
  }

  // z0 = alpha*x0 + dzeta

  AAF Temp(alpha*(cvalue) + dzeta);

  Temp.length = length + 1;
  Temp.size = Temp.length;
  Temp.deviations = new double [Temp.size];
  Temp.indexes = new unsigned [Temp.size];

  // zi = alpha*xi

  for (unsigned i = 0; i < length; i++)
  {
    Temp.indexes[i] = indexes[i];
    Temp.deviations[i] = alpha*(deviations[i]);
  }
  
  
  // Compute the error in a new deviation symbol
  // zk = delta
  Temp.indexes[length] = Temp.inclast();
  Temp.deviations[length] = delta;

#ifdef FAST_RAD
  Temp.radius = fabs(alpha) * radius + fabs(delta);
#endif

  return Temp;
}


/************************************************************
 * Method:        sqrt
 * Author & Date: ??? - ???
 * Description:   
 *   This function computes the square root
 *
 *   Input  : const AAF & : AAF argument
 *   Output : AAF         : AAF result
 ************************************************************/
AAF sqrt(const AAF & P)
{
  double a, b;
  double fa, fb;
  double r;
  double alpha, dzeta, delta;

  if (P.length == 0)
  {
    AAF Temp(sqrt(P.cvalue));
    return Temp;
  }

  r = P.rad();

  a = P.cvalue - r;
  b = P.cvalue + r;

  // check lower bound
  if (a < EPSILON)
  {
    throw(AAF_Exception(AAF_NEGROOT_EXCEPTION, 
			std::string("Error <AAF::sqrt>: computing negative root").c_str(), 
			a, b));
  }

  double dNeg = 0.0;

  if (a < 0.)
  {
    dNeg = -a/2.;
    a = 0.;
    fa = 0.;
  }
  else
  {
    fa = sqrt(a);
  }
  fb = sqrt(b);


  if (AAF::approximationType == CHEBYSHEV)
  {
    // temporary vars
    
    double t, u;

    alpha = 1.0/(fa + fb);
    
    t = 0.25*fa*fb*alpha;
    
    u = 0.125*(a + b)*alpha;
    
    // dzeta calculation:
    dzeta = u + 3.0*t;
    
    // Calculation of the error
    delta = u - t;
  }
  else if (AAF::approximationType == MINRANGE)
  {       
    alpha = 1.0/(2.0*fb);

    delta = 0.5*alpha*(a-2*fb*fa+b);
    
    dzeta = 0.5*fb-delta;
    
  }
  else // (AAF::approximationType == SECANT)
  {
    if (r > AAF_MINRAD)
    {
      alpha = (fb-fa)/(b-a);
    }
    else
    {
      alpha = 1.0/(2.0*fb);
    }
    dzeta = fa - alpha*a;
    delta = 0.;
  }

  // z0 = alpha*x0 + dzeta

  AAF Temp(alpha*(P.cvalue + dNeg) + dzeta);

  Temp.length = P.length + 1;
  Temp.size = Temp.length;
  Temp.deviations = new double [Temp.size];
  Temp.indexes = new unsigned [Temp.size];

  alpha -= dNeg/P.rad();
  // zi = alpha*xi
  for (unsigned i = 0; i < P.length; i++)
  {
    Temp.indexes[i] = P.indexes[i];
    Temp.deviations[i] = alpha*(P.deviations[i]);
  }
  
  // Compute the error --> delta in a new deviation symbol 
  Temp.indexes[P.length] = Temp.inclast();
  Temp.deviations[P.length] = delta;

#ifdef FAST_RAD
  Temp.radius = fabs(alpha) * P.radius + fabs(delta);
#endif

  return Temp;
}


/************************************************************
 * Method:        isqrt
 * Author & Date: Darius Grabowski - 03.02.2006
 * Description:   
 *   This function computes the inverted square root
 *
 *   Input  : const AAF & : AAF argument
 *   Output : AAF         : AAF result
 ************************************************************/
AAF isqrt(const AAF & P)
{
  double a, b;
  double fa, fb;
  double r;
  double alpha, dzeta, delta;

  if (P.length == 0)
  {
    AAF Temp(1.0/sqrt(P.cvalue));
    return Temp;
  }

  r = P.rad();

  a = P.cvalue - r;
  b = P.cvalue + r;

  // check bounds
  if ( (a < EPSILON) || (b < EPSILON) )
  {
    throw(AAF_Exception(AAF_NEGROOT_EXCEPTION, 
			std::string("Error <AAF::isqrt>: computing negative root").c_str(), 
			a, b));
  }

  fa = 1.0/sqrt(a);
  fb = 1.0/sqrt(b);

  if (AAF::approximationType == CHEBYSHEV)
  {
    double x_;  // temporary var

    // compute the slope
    if (r > AAF_MINRAD)
    {
      alpha = (fb - fa)/(b - a);
    }
    else
    {
      alpha = -0.5*fb*fb*fb;
    }
    
    // compute the point having the slope alpha
    x_ = pow((double)0.5*(a/fb + b/fa), (double)2/3);
    
    delta = 0.5*(fa + alpha*(x_ - a) - 1.0/sqrt(x_));
   
    // dzeta calculation:
    dzeta = fa - alpha*a - delta;
  }
  else if (AAF::approximationType == MINRANGE)
  {
    alpha = -0.5*fb*fb*fb;

    delta = 0.5*(fa - fb + alpha*(b - a));
    
    dzeta = fa - alpha*a - delta;
  }
  else // (AAF::approximationType == SECANT)
  {
    if (r > AAF_MINRAD)
    {
      alpha = (fb-fa)/(b-a);
    }
    else
    {
      alpha = -0.5*fb*fb*fb;
    }
    dzeta = fa - alpha*a;
    delta = 0.;
  }

  // z0 = alpha*x0 + dzeta

  AAF Temp(alpha*(P.cvalue) + dzeta);

  Temp.length = P.length + 1;
  Temp.size = Temp.length;
  Temp.deviations = new double [Temp.size];
  Temp.indexes = new unsigned [Temp.size];

  // zi = alpha*xi

  for (unsigned i = 0; i < P.length; i++)
  {
    Temp.indexes[i] = P.indexes[i];
    Temp.deviations[i] = alpha*(P.deviations[i]);
  }
  
  // Compute the error --> delta in a new deviation symbol 

  Temp.indexes[P.length] = Temp.inclast();
  Temp.deviations[P.length] = delta;

#ifdef FAST_RAD
  Temp.radius = fabs(alpha) * P.radius + fabs(delta);
#endif

  return Temp;
}


/************************************************************
 * Method:        inv
 * Author & Date: ??? - ???
 * Description:   
 *   This function computes the inverse 
 *
 *   Input  : const AAF & : AAF argument
 *   Output : AAF         : AAF result
 ************************************************************/
AAF inv(const AAF & P)
{
  double a, b;
  double fa, fb;
  double alpha;

  if (P.length == 0)
  {
    AAF Temp(1.0/(P.cvalue));
    return Temp;
  }

  a = P.getMin();
  b =P.getMax();
    
  if (a==0 or b==0)
  {
    throw(AAF_Exception(AAF_DIVZERO_EXCEPTION, 
    			std::string("Error <AAF::inv>: computing zero inversion").c_str(), 
			a, b));
  }

  fa = 1/a; // max
  fb = 1/b; // min
   
  // Derivative of 1/x is -1/x*x
  
  alpha = -1/(P.cvalue*P.cvalue);

  
  AAF Temp(1./(P.cvalue));
  
  Temp.length = P.length;
  Temp.size = Temp.length;
  Temp.deviations = new double [Temp.size];
  Temp.indexes = new unsigned [Temp.size];
  
  for (unsigned i = 0; i < Temp.length; i++)
  {
    Temp.indexes[i] = P.indexes[i];
    Temp.deviations[i] = alpha*(P.deviations[i]);
  }
    
 #ifdef FAST_RAD
  Temp.radius = fabs(alpha) * P.radius;
 #endif
    
 if (AAF::approximationType == INTERVAL_EXACT){
   Temp.offset_min=fb-Temp.getMin();
   Temp.offset_max=fa-Temp.getMax();
 }
 else{
  Temp.offset_min=minimum(fb, Temp.getMin())-Temp.getMin();
  Temp.offset_max=maximum(fa, Temp.getMax())-Temp.getMax();}
   
  return Temp;
}


/************************************************************
 * Method:        exp
 * Author & Date: ??? - ???
 * Description:   
 *   This function computes the exponential fct.
 *
 *   Input  : const AAF & : AAF argument
 *   Output : AAF         : AAF result
 ************************************************************/
AAF exp(const AAF &P)
{
  double a, b;
  double fa, fb;
  double r;
  double alpha, dzeta, delta;

  bool chebyshevFailed = false;

  if (P.length == 0)
  {
    AAF Temp(exp(P.cvalue));
    return Temp;
  }

  r = P.rad();

  a = P.cvalue - r;
  b = P.cvalue + r;

  // if upper bound < -50 --> return value is smaller than 1E-20
  if (b < -50.0)
  {
    AAF Temp(exp(P.cvalue));
    return Temp;
  }

  // Derivative of exp(x) is exp(x)
  fa = exp(a); 
  fb = exp(b);  
  
  if (AAF::approximationType == CHEBYSHEV)
  {
    // use Chebyshev approximation
    if (r > AAF_MINRAD)
    {
      alpha = (fb-fa)/(b-a);
    }
    else
    {
      alpha = fa;
    }
    
    double u = log(alpha);
    
    delta = 0.5*(fa+(u-a-1.0)*alpha);
    
    dzeta = fa-a*alpha-delta;
    
    if ( (fa - 2.0*delta) < 0.0)
      chebyshevFailed = true;

#ifdef AAF_DEBUG
    fprintf(stdout, "Function: exp(x)\n");
    fprintf(stdout, "Interval: [%f, %f]\n", a, b);
    fprintf(stdout, "f(Interval): [%f, %f]\n", fa, fb);
    fprintf(stdout, "x_1: %f, f(x_1): %f\n", u, exp(u));
    fprintf(stdout, "alpha: %g, delta: %g, dzeta: %g\n", alpha, delta, dzeta);
#endif
  }
  else if (AAF::approximationType == MINRANGE)
  {
    // use MinRange approximation
    alpha = fa;
  
    dzeta = 0.5*(fa+fb-alpha*(a+b));

    delta = 0.5*(fb-fa-alpha*(b-a));
  }
  else // (AAF::approximationType == SECANT)
  {
    if (r/fa > AAF_MINRAD)
    {
      alpha = (fb-fa)/(b-a);
    }
    else
    {
      alpha = fa;
    }
    dzeta = fa - alpha*a;
    delta = 0.;
  }

  // z0 = alpha*x0 + dzeta

  AAF Temp(alpha*(P.cvalue) + dzeta);

  Temp.length = P.length + 1;
  Temp.size = Temp.length;
  Temp.deviations = new double [Temp.size];
  Temp.indexes = new unsigned [Temp.size];

  // zi = alpha*xi

  for (unsigned i = 0; i < P.length; i++)
  {
    Temp.indexes[i] = P.indexes[i];
    Temp.deviations[i] = alpha*(P.deviations[i]);
  }
    
  // Compute the error in a new deviation symbol 
  // zk = delta
  Temp.indexes[P.length] = Temp.inclast();
  Temp.deviations[P.length] = delta;

#ifdef FAST_RAD
  Temp.radius = fabs(alpha) * P.radius + fabs(delta);
#endif

  return Temp;
}


/************************************************************
 * Method:        log
 * Author & Date: Darius Grabowski - 10.10.2005
 * Description:   
 *   computes the natural logarithm function
 *
 *   Input  : const AAF & : AAF argument
 *   Output : AAF         : AAF result
 ************************************************************/
AAF log(const AAF &P)
{
  double a, b;
  double fa, fb;
  double r;
  double alpha, dzeta, delta;

  if (P.length == 0)
  {
    AAF Temp(log(P.cvalue));
    return Temp;
  }
  
  r = P.rad();

  a = P.cvalue - r;
  b = P.cvalue + r;

  if ( (a < EPSILON) || (b < EPSILON) )
  {
    throw(AAF_Exception(AAF_NEGLOG_EXCEPTION, 
			std::string("Error <log(AAF)>: computing negative log").c_str(), 
			a, b));
  }

  // Derivative of log(x) is 1.0/x  
  
  fa = log(a);
  fb = log(b);

  if (AAF::approximationType == CHEBYSHEV)
  {
    // use Chebyshev approximation
    if (r > AAF_MINRAD)
    {
      alpha = (fb-fa)/(b-a);
    }
    else
    {
      alpha = 1.0/a;
    }
   
    double u = 1.0/alpha;
    double fu = log(u);

    double y_1 = fu - alpha*u;
    double y_2 = fb - alpha*b;

    delta = 0.5*(y_1 - y_2);
    
    // dzeta = 0.5*(y_1 + y_2); 
    dzeta = y_1 - delta;
    
    
#ifdef AAF_DEBUG
    fprintf(stdout, "Function: log(x)\n");
    fprintf(stdout, "Interval: [%f, %f]\n", a, b);
    fprintf(stdout, "f(Interval): [%f, %f]\n", fa, fb);
    fprintf(stdout, "x_: %f, f(x_): %f\n", u, fu);
    fprintf(stdout, "alpha: %f, delta: %f, dzeta: %f\n", alpha, delta, dzeta);
#endif
  }
  else if (AAF::approximationType == MINRANGE)
  {
    // use MinRange approximation    
    alpha = 1.0/b;
  
    delta = 0.5*(fb-fa-alpha*(b-a));
    
    dzeta = fb-1.0-delta;    
  }
  else // (AAF::approximationType == SECANT)
  {
    if (r > AAF_MINRAD)
    {
      alpha = (fb-fa)/(b-a);
    }
    else
    {
      alpha = 1./a;
    }
    dzeta = fa - alpha*a;
    delta = 0.;
  }

  // z0 = alpha*x0 + dzeta

  AAF Temp(alpha*(P.cvalue) + dzeta);

  Temp.length = P.length + 1;
  Temp.size = Temp.length;
  Temp.deviations = new double [Temp.size];
  Temp.indexes = new unsigned [Temp.size];

  // zi = alpha*xi
  for (unsigned i = 0; i < P.length; i++)
  {
    Temp.indexes[i] = P.indexes[i];
    Temp.deviations[i] = alpha*(P.deviations[i]);
  }
  
  // Compute the error in a new deviation symbol  
  // zk = delta
  Temp.indexes[P.length] = Temp.inclast();
  Temp.deviations[P.length] = delta;
  
#ifdef FAST_RAD
  Temp.radius = fabs(alpha) * P.radius + fabs(delta);
#endif

  return Temp;
}


/************************************************************
 * Method:        logexp
 * Author & Date: Darius Grabowski - 10.10.2005
 * Description:   
 *   computes the chained function log(1.0 + exp(x))
 *
 *   Input  : const AAF & : AAF argument
 *   Output : AAF         : AAF result
 ************************************************************/
AAF logexp(const AAF &P)
{
  double a, b;
  double fa, fb;
  double r;
  double alpha, dzeta, delta;

  if (P.length == 0)
  {
    AAF Temp(log(1.0 + exp(P.cvalue)));
    return Temp;
  }
  
  r = P.rad();

  a = P.cvalue - r;
  b = P.cvalue + r;

  if (b < 0)
  {
    fa = log(1.0 + exp(a));
    fb = log(1.0 + exp(b));
  }
  else
  {
    fa = a + log(1.0+exp(-a));
    fb = b + log(1.0+exp(-b));
  }

  // Derivative of log(1+exp(x)) is exp(x)/(1+exp(x))

  if (AAF::approximationType == CHEBYSHEV)
  {
    // use Chebyshev approximation
    if (r/fa > AAF_MINRAD)
    {
      alpha = (fb-fa)/(b-a);
    }
    else
    {
      alpha = exp(a)/(1.0 + exp(a));
    }
   
    if (alpha < 1E-15)
      alpha = 1E-15;
    
    double u, fu;

    if (alpha > 1.0-1E-15)
    {
      u = 0.5*(a+b);
      fu = log(1.0 + exp(u));
    } 
    else
    {
      u = log(alpha/(1.0 - alpha));
      fu = -log(1.0 - alpha);
    }

    delta = 0.5*(fa - fu - alpha*(a - u));
    
    dzeta = fa - alpha*a - delta;

#ifdef AAF_DEBUG
    fprintf(stdout, "Function: log(1.0+exp(x))\n");
    fprintf(stdout, "Interval: [%f, %f]\n", a, b);
    fprintf(stdout, "f(Interval): [%f, %f]\n", fa, fb);
    fprintf(stdout, "x_: %f, f(x_): %f\n", u, fu);
    fprintf(stdout, "alpha: %f, delta: %f, dzeta: %f\n", alpha, delta, dzeta);
#endif
  }
  else if (AAF::approximationType == MINRANGE)
  {
    // use MinRange approximation
    alpha = exp(a)/(1.0 + exp(a));

    double y_a = fa - alpha*a;
    double y_b = fb - alpha*b;
  
    delta = 0.5*(y_b - y_a);
    
    dzeta = 0.5*(y_b + y_a);    
  }
  else // (AAF::approximationType == SECANT)
  {
    if (r/fa > AAF_MINRAD)
    {
      alpha = (fb-fa)/(b-a);
    }
    else
    {
      alpha = exp(a)/(1.0 + exp(a));
    }
    dzeta = fa - alpha*a;
    delta = 0.;
  }

  // z0 = alpha*x0 + dzeta

  AAF Temp(alpha*(P.cvalue) + dzeta);

  Temp.length = P.length + 1;
  Temp.size = Temp.length;
  Temp.deviations = new double [Temp.size];
  Temp.indexes = new unsigned [Temp.size];

  // zi = alpha*xi
  for (unsigned i = 0; i < P.length; i++)
  {
    Temp.indexes[i] = P.indexes[i];
    Temp.deviations[i] = alpha*(P.deviations[i]);
  }
  
  // Compute the error in a new deviation symbol  
  // zk = delta
  Temp.indexes[P.length] = Temp.inclast();
  Temp.deviations[P.length] = delta;

#ifdef FAST_RAD
  Temp.radius = fabs(alpha) * P.radius + fabs(delta);
#endif
  
  return Temp;
}

/************************************************************
 * Method:        atan
 * Author & Date: Darius Grabowski - 10.06.2005
 * Description:   
 *   Computes the arcus tangens function
 *   The atan function is strictly monotonic increasing but it 
 *   has two possible turning points which has to be considered
 *   for the Chebyshev approxiamtion
 *
 *   Input  : const AAF & : AAF argument
 *   Output : AAF         : AAF result
 ************************************************************/
AAF atan(const AAF &P)
{
  double a, b;
  double fa, fb;
  double r;
  double alpha, dzeta, delta;  

  if (P.length == 0)
  {
    AAF Temp(atan(P.cvalue));
    return Temp;
  }

  r = P.rad();

  a = P.cvalue - r;
  b = P.cvalue + r;

  // Derivative of atan(x) is 1.0/(1.0+x^2)
  fa = atan(a);
  fb = atan(b);

  if (AAF::approximationType == CHEBYSHEV)
  {
    // use Chebyshev approximation
    if (r > AAF_MINRAD)
    {
      alpha = (fb-fa)/(b-a);
    }
    else
    {
      alpha = 1.0/(1.0+a*b);
    }
    
    // we have two points having the slope alpha
    double x_1 = -sqrt((1-alpha)/alpha);
    double x_2 = -x_1;

    double fx_1, fx_2;

    // check valid points
    if (x_1 > a)
    {
      // x_1 is valid
      fx_1 = atan(x_1);
    }
    else
    {
      x_1 = a;
      fx_1 = fa;
    }

    if (x_2 < b)
    {
      // x_2 is valid
      fx_2 = atan(x_2);
    }
    else
    {
      x_2 = b;
      fx_2 = fb;
    }

    double y_2 = fx_2 - alpha*x_2;
    double y_1 = fx_1 - alpha*x_1;
    
    delta = 0.5*(y_2 - y_1);
    
    dzeta = 0.5*(y_2 + y_1);

#ifdef AAF_DEBUG
    fprintf(stdout, "Function: atan(x)\n");
    fprintf(stdout, "Interval: [%f, %f]\n", a, b);
    fprintf(stdout, "f(Interval): [%f, %f]\n", fa, fb);
    fprintf(stdout, "x_1: %f, f(x_1): %f\n", x_1, fx_1);
    fprintf(stdout, "x_2: %f, f(x_2): %f\n", x_2, fx_2);
    fprintf(stdout, "alpha: %f, delta: %f, dzeta: %f\n", alpha, delta, dzeta);
#endif
  }
  else if (AAF::approximationType == MINRANGE)
  {
    // use MinRange approximation    
    if (fabs(b) > fabs(a))
    {
      alpha = 1.0/(1.0+b*b);
    }
    else
    {
      alpha = 1.0/(1.0+a*a);
    }
  
    double y0_a = fa-alpha*a;
    double y0_b = fb-alpha*b;
    
    dzeta = 0.5*(y0_b + y0_a);

    delta = 0.5*(y0_b - y0_a);
  }
  else // (AAF::approximationType == SECANT)
  {
    if (r > AAF_MINRAD)
    {
      alpha = (fb-fa)/(b-a);
    }
    else
    {
      alpha = 1.0/(1.0+a*a);
    }
    dzeta = fa - alpha*a;
    delta = 0.;
  }

  // z0 = alpha*x0 + dzeta
  AAF Temp(alpha*(P.cvalue) + dzeta);

  Temp.length = P.length + 1;
  Temp.size = Temp.length;
  Temp.deviations = new double [Temp.size];
  Temp.indexes = new unsigned [Temp.size];

  // zi = alpha*xi
  for (unsigned i = 0; i < P.length; i++)
  {
    Temp.indexes[i] = P.indexes[i];
    Temp.deviations[i] = alpha*(P.deviations[i]);
  }  
  
  // Compute the error in a new deviation symbol
  // zk = delta
  Temp.indexes[P.length] = Temp.inclast();
  Temp.deviations[P.length] = delta;

#ifdef FAST_RAD
  Temp.radius = fabs(alpha) * P.radius + fabs(delta);
#endif

  return Temp;
}

/************************************************************
 * Method:        tanh
 * Author & Date: Darius Grabowski - 10.10.2005
 * Description:   
 *   computes the tangens hyperbolicus function
 *   Definition: tanh(x) := (exp(x)-exp(-x))/(exp(x)+exp(-x))
 *   The tanh function is strictly monotonic increasing but it 
 *   has two possible turning points which has to be considered
 *   for the Chebyshev approximation
 *
 *   Input  : const AAF & : AAF argument
 *   Output : AAF         : AAF result
 ************************************************************/
AAF tanh(const AAF &P)
{
  double a, b;
  double fa, fb;
  double r;
  double alpha, dzeta, delta;  

  if (P.length == 0)
  {
    AAF Temp(tanh(P.cvalue));
    return Temp;
  }

  r = P.rad();

  a = P.cvalue - r;
  b = P.cvalue + r;

  // Derivative of tanh(x) is 1.0-tanh(x)^2

  fa = tanh(a);
  fb = tanh(b);

  if (AAF::approximationType == CHEBYSHEV)
  {
    // use Chebyshev approximation
    if (r > AAF_MINRAD)
    {
      alpha = (fb-fa)/(b-a);
    }
    else
    {
      alpha = 1.0-fa*fb;
    }
    
    // we have two points having the slope alpha
    double dSqrtAlpha = sqrt(1-alpha);
    double x_1 = -atanh(dSqrtAlpha);
    double x_2 = -x_1;

    double fx_1, fx_2;

    // check valid points
    if (x_1 > a)
    {
      // x_1 is valid
      fx_1 = -dSqrtAlpha;
    }
    else
    {
      x_1 = a;
      fx_1 = fa;
    }

    if (x_2 < b)
    {
      // x_2 is valid
      fx_2 = dSqrtAlpha;
    }
    else
    {
      x_2 = b;
      fx_2 = fb;
    }

    double y_2 = fx_2 - alpha*x_2;
    double y_1 = fx_1 - alpha*x_1;
    
    delta = 0.5*(y_2 - y_1);
    
    dzeta = 0.5*(y_2 + y_1);

#ifdef AAF_DEBUG
    fprintf(stdout, "Function: tanh(x)\n");
    fprintf(stdout, "Interval: [%f, %f]\n", a, b);
    fprintf(stdout, "f(Interval): [%f, %f]\n", fa, fb);
    fprintf(stdout, "x_1: %f, f(x_1): %f\n", x_1, fx_1);
    fprintf(stdout, "x_2: %f, f(x_2): %f\n", x_2, fx_2);
    fprintf(stdout, "alpha: %f, delta: %f, dzeta: %f\n", alpha, delta, dzeta);
#endif
  }
  else if (AAF::approximationType == MINRANGE)
  {
    // use MinRange approximation    
    if (fabs(fb) > fabs(fa))
    {
      alpha = 1.0-fb*fb;
    }
    else
    {
      alpha = 1.0-fa*fa;
    }
  
    double y0_a = fa-alpha*a;
    double y0_b = fb-alpha*b;
    
    dzeta = 0.5*(y0_b + y0_a);

    delta = 0.5*(y0_b - y0_a);
  }
  else // (AAF::approximationType == SECANT)
  {
    if (r > AAF_MINRAD)
    {
      alpha = (fb-fa)/(b-a);
    }
    else
    {
      alpha = 1.0-fa*fb;
    }
    dzeta = fa - alpha*a;
    delta = 0.;
  }

  // z0 = alpha*x0 + dzeta

  AAF Temp(alpha*(P.cvalue) + dzeta);

  Temp.length = P.length + 1;
  Temp.size = Temp.length;
  Temp.deviations = new double [Temp.size];
  Temp.indexes = new unsigned [Temp.size];

  // zi = alpha*xi

  for (unsigned i = 0; i < P.length; i++)
  {
    Temp.indexes[i] = P.indexes[i];
    Temp.deviations[i] = alpha*(P.deviations[i]);
  }
  
  
  // Compute the error in a new deviation symbol  
  // zk = delta
  Temp.indexes[P.length] = Temp.inclast();
  Temp.deviations[P.length] = delta;

#ifdef FAST_RAD
  Temp.radius = fabs(alpha) * P.radius + fabs(delta);
#endif

  return Temp;
}


/************************************************************
 * Method:        heaviside
 * Author & Date: Darius Grabowski - 10.12.2005
 * Description:   
 *   computes a simplified heaviside function
 *
 *   Input  : const AAF & : AAF argument
 *   Output : AAF         : AAF result
 ************************************************************/
AAF heaviside(const AAF &P)
{
  AAF Temp(0.0);

  if (P.getcenter() > 0.0)
    Temp += 1.0;

  return Temp;
}

/************************************************************
 * Method:        mag
 * Author & Date: Darius Grabowski - 06/2007
 * Description:
 *   computes the magnitude of (re|im)
 *
 *   Input  : const AAF & : AAF real part
 *            const AAF & : AAF imaginary part
 *   Output : AAF         : AAF result
 ************************************************************/
AAF mag(const AAF & re, const AAF & im)
{


    double dRe = re.getcenter();
    double dIm = im.getcenter();

    
    double dMag = sqrt(dRe*dRe + dIm*dIm);
    double dxx = dRe / dMag;
    double dyy = dIm / dMag;
    
    double min_real, max_real;
    double min_imag, max_imag;
    
    min_real=max_real=dRe;
    max_real=max_imag=dIm;
    
    unsigned l1 = re.length;
    unsigned l2 = im.length;
    
   
    if (l1+l2 == 0)
    {
        AAF Temp(dMag);
        
        return (Temp);
    }

    if (l1 == 0)
    {
        // todo
    }

    if (l2 == 0)
    {
        // todo
    }

    // Create the resulting AAF
    AAF Temp(dMag);

    unsigned * id1 = re.indexes;
    unsigned * id2 = im.indexes;

    double * va1 = re.deviations;
    double * va2 = im.deviations;

    unsigned * pu1 = id1;
    unsigned * pu2 = id2;
    
    if (l1+l2)
        Temp.indexes = new unsigned [l1+l2 + 1]; // the indexes of the result
    unsigned * idtemp = Temp.indexes;
    
    // Fill the resulting indexes array
    // by merging the 2 input indexes array
    
    unsigned * fin = std::set_union(id1,id1+l1,id2,id2+l2,idtemp);
    unsigned ltemp = fin - idtemp + 1;
    
    if (ltemp)
        Temp.deviations = new double [ltemp];
    double * vatempg = Temp.deviations;
    
    Temp.length = ltemp;
    Temp.size = ltemp;
    
    // Fill the deviations array
    // of the resulting AAF
    
    for (unsigned i = 0; i < ltemp-1; i++)
    {
        unsigned a = pu1-id1;
        unsigned b = pu2-id2;
        
        
        if (a == l1 || id1[a] != idtemp[i])
        {
            vatempg[i] = dyy*va2[b];  // va2[b]+0
            
            
            if (abs(dIm)<im.rad())
            {
                min_imag=0;
            }
            else
            {
                if ( vatempg[i]>0)
                {
                    
                   min_imag-=va2[b];
                  
                    
                }
                else
                {
                    
                    min_imag+=va2[b];
                   
  
                }
                
            }
            
            
            if ( vatempg[i]>0)
            {
                
                max_imag+=va2[b];
                
                
            }
            else
            {
                
                max_imag-=va2[b];
                
                
            }
            
            pu2++;
            continue;
        }
        
        if (b == l2 || id2[b] != idtemp[i])
        {
            vatempg[i] = dxx*va1[a];  // va1[a]+0
            
            
            if (abs(dRe)<re.rad())
            {
                min_real=0;
            }
            else
            {
                if ( vatempg[i]>0)
                {
                    
                    min_real-=va2[b];
                    
                    
                }
                else
                {
                    
                    min_real+=va2[b];
                    
                    
                }
            }
            
            if ( vatempg[i]>0)
            {
                
                max_real+=va2[b];
                
                
            }
            else
            {
                
                max_real-=va2[b];
                
                
            }
            pu1++;
            continue;
        }
        
        vatempg[i] = dxx*va1[a] + dyy*va2[b];
        
        
        if (abs(dRe)<re.rad())
        {
            min_real=0;
        }
        else
        {
            
            if ( vatempg[i]>0)
            {
                
                min_real-=va2[b];
                
                
            }
            else
            {
                
                min_real+=va2[b];
                
                
            }
            
        }
        
        if ( vatempg[i]>0)
        {
            
            max_real+=va2[b];
            
            
        }
        else
        {
            
            max_real-=va2[b];
            
            
        }

        
    
        if (abs(dIm)<im.rad())
        {
            min_imag=0;
        }
        else
        {
            
            
            if ( vatempg[i]>0)
            {
                
                min_imag-=va2[b];
                
                
            }
            else
            {
                
                min_imag+=va2[b];
                
                
            }
        }
        
        if ( vatempg[i]>0)
        {
            
            max_imag+=va2[b];
            
            
        }
        else
        {
            
            max_imag-=va2[b];
            
            
        }
        
       
        
        pu1++;
        pu2++;
    }
    
    double ampl_min=sqrt(pow((min_real),2)+pow((min_imag),2));
    double ampl_max=sqrt(pow((max_real),2)+pow((max_imag),2));

    

    
#ifdef FAST_RAD
    Temp.radius = 0.0;
    for (unsigned i = 0; i < ltemp-1; i++)
    {
        Temp.radius += fabs(vatempg[i]);
    }
#endif
    
    
    double lin_error= ((ampl_max-Temp.getMax())-(ampl_min-Temp.getMin()))/2.0;
    double offset=((ampl_max-Temp.getMax())+(ampl_min-Temp.getMin()))/2.0;
    
    
    Temp.cvalue+=offset;
    
    Temp.indexes[ltemp-1] = Temp.inclast();
    Temp.deviations[ltemp-1] =lin_error;
    
#ifdef FAST_RAD
    Temp.radius+=lin_error;
#endif
    
  
    //AAF res=(ampl_min+ampl_max)/2.0+AAInterval(-(ampl_max-ampl_min)/2.0, (ampl_max-ampl_min)/2.0);
    
    
    return Temp;
}

AAF round(const AAF& P)
{
  AAF Temp(P);
  
  double min=P.getMin();
  
  double max=P.getMax();
  
  int lit=lround(min);
  int rit=lround(max);

  vector<double> steps;
  vector<double> out_values;

  out_values.push_back(lit);
  
  for (int i=lit; i<=rit; i++)
  {
     steps.push_back(i+0.5);
     out_values.push_back(i+1);
     
  }
  
  if (max<steps[0] and !(abs(max-steps[0])<1e-10))
  {
     
    return out_values[0];
  }
  
  for (unsigned i=0; i<steps.size()-1; i++)
  {
  
    if ((min>steps[i] or abs(min-steps[i])<1e-10) and (max<steps[i+1] and !(abs(max-steps[i+1])<1e-10))) 
    {

       return out_values[i+1];
       
    }
    
  }
  
  if (min>steps[steps.size()-1] or abs(min-steps[steps.size()-1])<1e-10)
  {
      
     return out_values.back();
  }
  
  
  Temp.offset_min=-0.5;
  Temp.offset_max=0.5;
  
  return Temp;
  
}
