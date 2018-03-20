/*
 * aa_aaftrigo.cpp -- Trigonometric operations (all non-affine)
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
// $Log: aa_aaftrigo.c,v $
//
// }}} 

#include "aa.h"
#include <algorithm>
#include <cmath>

#ifndef PI
#define PI (4*atan(1.0))
#endif

// number of points for the linear regression approximation
#define NPTS 8 


/************************************************************
 * Method:        sin
 * Author & Date: ??? - ???
 * Description:   
 *   Sine function
 *   sine isn't montonic and the second derivative change its 
 *   sign inside the defined interval so we use the 
 *   approximation by least squares
 *
 *   Input  : double : corresponding center value
 *   Output : -
 ************************************************************/
AAF sin(const AAF & P)
{

  double a, b;
  double alpha, dzeta, delta;

  if (P.length == 0)
  {
    AAF Temp(sin(P.cvalue));
    return Temp;
  }

  AAInterval i = P.convert();

  double w = i.width();

  //i = mintrigo(i); // no more needed

  a = i.getlo();
  b = i.gethi();


  // y' = alpha*x+dzeta , the regression line
  // approximate y = sin(x)

  double x[NPTS];
  double y[NPTS];
  double r[NPTS]; // residues, r[i] = y[i]-y'[i]

  double xm = 0;
  double ym = 0;

  // the trivial case, the interval is larger than 2*PI
  if (w >= 2*PI ) 
  {
    // y' = 0 , delta = 1 cause -1 <= sin(x) <= +1
    alpha = 0.0;
    dzeta = 0.0;
    delta = 1.0;
  }
  else // case of the least squares
  {
    x[0]=a;
    y[0]=sin(a);
    x[NPTS-1]=b;
    y[NPTS-1]=sin(b);
    
    double pas=w/(NPTS-1);
    
    for (unsigned j=1; j< NPTS-1; j++)
    {
      x[j]=x[j-1]+pas;
      y[j]=sin(x[j]);
    }
    
    
    // Calculation of xm and ym , averages of x and y
    
    for (unsigned j=0; j<NPTS; j++)
    {
      xm=xm+x[j];
      ym=ym+y[j];
    }
    
    xm=xm/NPTS;
    ym=ym/NPTS;
    
    // Calculation of alpha and dzeta
    
    double temp1;
    double temp2=0;
    alpha = 0;
    
    for (unsigned j=0; j<NPTS; j++)
    {
      temp1=x[j]-xm;
      alpha+=y[j]*temp1;
      temp2+=temp1*temp1;
    }    
    
    alpha=alpha/temp2;  // final alpha
    dzeta=ym-alpha*xm; // final dzeta
    
    
    // Calculation of the residues
    // We use the absolute value of the residues!
    
    for (unsigned j=0; j<NPTS; j++)
    {
      r[j]=fabs(y[j]-(dzeta+alpha*x[j]));
    }
    
    
    // The error delta is the maximum
    // of the residues (in absolute values)
    
    double *ptr;
    
    ptr = std::max_element(r, r+NPTS);
    
    delta = *ptr;
    
  }
    
  // z0 = alpha*x0 + dzeta
  
  AAF Temp(alpha*(P.cvalue)+dzeta);
  
  Temp.length=(P.length)+1;
  Temp.size = Temp.length;
  Temp.deviations = new double [Temp.size];
  Temp.indexes = new unsigned [Temp.size];
  
  // zi = alpha*xi
  
  for (unsigned j=0; j < P.length; j++)
  {
    Temp.indexes[j]=P.indexes[j];
    Temp.deviations[j]=alpha*(P.deviations[j]);
  }
  
  // zk = delta
  
  Temp.indexes[P.length]=Temp.inclast();   // the error indx
  Temp.deviations[P.length]=delta;
  
  return Temp;
}


/************************************************************
 * Method:        cos
 * Author & Date: ??? - ???
 * Description:   
 *   Cosine function
 *   we use the identity cos(x)=sin(x+PI/2)
 *
 *   Input  : double : corresponding center value
 *   Output : -
 ************************************************************/
AAF cos(const AAF & P)
{
  AAF Temp = P;
  return sin(Temp+PI/2);
}


/************************************************************
 * Method:        tan
 * Author & Date: ??? - ???
 * Description:   
 *   Tangent function
 *   we use the identity tan(x)=sin(x)/cos(x)
 *   Due to the nature of the tan fct remember that
 *   we can have infinite value with small intervals
 *
 *   Input  : double : corresponding center value
 *   Output : -
 ************************************************************/
AAF tan(const AAF & P)
{
  return sin(P)/cos(P);
}


/************************************************************
 * Method:        tan
 * Author & Date: ??? - ???
 * Description:   
 *   Cotangent function
 *   we use the identity cotan(x)=cos(x)/sin(x)
 *   Due to the nature of the cotan fct remember that
 *   we can have infinite value with small intervals
 *
 *   Input  : double : corresponding center value
 *   Output : -
 ************************************************************/
AAF cotan(const AAF & P)
{
  return cos(P)/sin(P);
}
