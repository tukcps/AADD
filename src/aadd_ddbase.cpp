/**

 @file aadd_ddbase.cpp

 @ingroup AADD

 @brief Functions and methods to manipulate the DD Base class used in AADD.

 @author Carna Radojicic, Christoph Grimm

 @copyright@parblock
 Copyright (c) 2017  Carna Radojicic, Christoph Grimm
 Design of Cyber-Physical Systems
 TU Kaiserslautern 
 Postfach 3049 
 67663 Kaiserslautern 
 radojicic@cs.uni-kl.de

 This file is part of AADD package.

 AADD is free software: you can redistribute it and/or modify it
 under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 AADD is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public
 License for more details.

 You should have received a copy of the GNU General Public License
 along with AADD package. If not, see <http://www.gnu.org/licenses/>.
 @endparblock
 */


#include <stdio.h>
#include <math.h>
#include <algorithm>    // std::set_union, std::sort
#include <vector>       // std::vector

#include "aadd_ddbase.h"


// Destructor for bool value type.
template <>
DDNode<bool>::~DDNode()
{
    // There are shared nodes, e.g. ONE and ZERO.
    // They may never be deleted. If, this is an error.
    assert( this->isNotShared() );
    
    // ZERO and ONE can be shared
    if ( (T!=nullptr) and (T->isNotShared() ) ) delete T;
    if ( (F!=nullptr) and (F->isNotShared() ) ) delete F;
};


// Destructor for AAF value type
template<>
DDNode<AAF>::~DDNode()
{
    // There are shared nodes, e.g. ONE and ZERO.
    // They may never be deleted. If, this is an error.
    assert( this->isNotShared() );
    
    // recursion
    if ( (T!=nullptr) ) delete T;
    if ( (F!=nullptr) ) delete F;
};

// destroy tree for bool data type
template <>
void DDNode<bool>::delete_tree()
{
    // There are shared nodes, e.g. ONE and ZERO.
    // They may never be deleted. If, this is an error.
   
    if (this->isNotShared())
        delete this;
    
}


// destroy tree for AAF data type
template <>
void DDNode<AAF>::delete_tree()
{
    // There are shared nodes, e.g. ONE and ZERO.
    // They may never be deleted. If, this is an error.
    
    if (this->isNotShared())
        delete this;
}


template<>
void DDBase<bool>::setRoot(DDNode<bool>* source)
{
   if (root != nullptr && root -> isNotShared() )
        delete root; // for BDD.
    
    root = source;
}


template<>
void DDBase<AAF>::setRoot(DDNode<AAF>* source)
{
    if (root != nullptr && root -> isNotShared() )
        delete root;
    
    root = source;
}


template<>
int DDBase<AAF>::printFile(FILE* fp, const DDNode<AAF>* node) const
{
  int retval;
    
  if (node!=NULL)
  {
      if (node->isLeaf() )
      {
          AAF val=node->getValue();
          
          if (val.getlength()==0)
          {
              retval=fprintf(fp, "\"%p\" [shape=box] [label = \"%g\"];\n",  (void *) node, val.getcenter());
              
              if (retval == EOF)
              {
                  cout << "END of FILE!" << endl;
                  return(1);
              }
          }
          else
          {
              retval=fprintf(fp,  "\"%p\" [shape=box] [label = \"%g", (void *) node, val.getcenter());
              
              if (retval == EOF)
              {
                  cout << "END of FILE!" << endl;
                  return(1);
              }
              
              for (unsigned i=0; i<val.getlength(); i++)
              {
                  
                  if (val[i+1]>=0)
                  {
                      retval=fprintf(fp, "+");
                      
                      if (retval == EOF)
                      {
                          cout << "END of FILE!" << endl;
                          return(1);
                      }
                  }
                  
                  retval=fprintf(fp, "%g*e%u", val[i+1], val.getIndexes()[i]);
                  
                  if (retval == EOF)
                  {
                      cout << "END of FILE!" << endl;
                      return(1);
                  }
              }
              
              if (val.offset_min!=0 or val.offset_max!=0)
              {
                  retval=fprintf(fp, "+[%g, %g]", val.offset_min, val.offset_max);
                  
              }
              retval=fprintf(fp,"\"];\n");
              

          }
      }
      else
      {
          
          retval=fprintf(fp, "\"%p\" [label= \"v%lu\"];\n", (void *) node, node->getIndex());
          
          if (retval == EOF)
          {
              cout << "END of FILE!" << endl;
              return(1);
          }
          

          retval = fprintf(fp,
                           "\"%p\" -> \"%p\" [style = solid];\n",
                           (void *) node, (void *) node->getT());
          
          if (retval == EOF)
          {
              cout << "END of FILE!" << endl;
              return(1);
          }
          
          retval = fprintf(fp,
                           "\"%p\" -> \"%p\" [style = dashed];\n",
                           (void *) node, (void *) node->getF());
          
          
          if (retval == EOF)
          {
              cout << "END of FILE!" << endl;
              return(1);
          }
          
          printFile(fp, node->getT());
          printFile(fp, node->getF());
 
      }
  }
    
    
  return 0;
    

}

