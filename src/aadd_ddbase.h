/**
 
 @file aadd_ddbase.h
 
 @ingroup AADD
 
 @brief Decision diagram structure base class.

 @details Defines the data structure of ROBDD that is common to both AADD and BDD. The classes take the leaf type as a template parameter.
 @details Everything here should not be visible to a user.
 
 @author Carna Radojicic, Christoph Grimm
 
 @copyright@parblock
 Copyright (c) 2017  
 Carna Radojicic, Christoph Grimm 
 Design of Cyber-Physical Systems
 TU Kaiserslautern Postfach 3049 67663 Kaiserslautern radojicic@cs.uni-kl.de
 
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

#ifndef aadd_ddbase_h
#define aadd_ddbase_h

#include "aadd_config.h"
#include <assert.h>
#include <iostream>
#include <fstream>
#include <limits>

#include "aa.h"
#include "aadd_mgr.h"

using namespace std;


/*
 @brief MAXINDEX is the maximum index of the tree nodes. 
 @brief roots are numbered from 0 on, with increasing indices. 
 @brief leaves have index MAXINDEX
 */
const unsigned long MAXINDEX=numeric_limits<unsigned long>::max();

/**
 @brief Node of a binary decision diagram with leaf nodes of LeafType.
 */
template <class ValueT>
class DDNode
{
public:
    bool isLeaf() const;
    bool isInternal() const;
    
    bool isShared() const;
    bool isNotShared() const;
    
    ValueT getValue() const;
    void setValue(const ValueT& v)  { value = v; }; 
    
    AAF& getCond() const             { return condMgr().getCond(index); };
    
    unsigned long getIndex()        const { return index; };
    void setIndex(unsigned long i)  { index = i;};
    
    
    DDNode<ValueT>* getT()            const { return T; };
    DDNode<ValueT>* getF()            const { return F; };
    void setT(DDNode<ValueT>* t)       { T=t; };
    void setF(DDNode<ValueT>* f)       { F=f; };
    
    void print(std::ostream &s = std::cout) const;
    DDNode<ValueT>& copy(const DDNode<ValueT>& source); // Creates a recursive copy of source.
    void delete_tree();
    
protected:
    unsigned long index;  /** index */
    DDNode<ValueT> *T;    /** 1(node); NULL if leaf-node */
    DDNode<ValueT> *F;    /** 0(node); NULL if leaf-node */
    ValueT value;         /** Value if leaf-node */
    
public:
    DDNode(unsigned long index, DDNode<ValueT> *T, DDNode<ValueT> *F);
    DDNode(const DDNode<ValueT>& from); // copy constructor
    ~DDNode();
};


/* 
 @brief The base class for decision diagrams that have leaves of type ValT. 
 */
template<class ValT>
class DDBase
{
public: 
    
    // IO methods
    int printFile(FILE*, const DDNode<ValT>*) const;
    std::ostream & printConsole(std::ostream &, const DDNode<ValT>&) const;
    friend std::ostream & operator << (std::ostream &, const DDBase<ValT> &);
    void printGraphViz(const string file_name) const;
       
    unsigned numLeaves() const;
    unsigned numLeaves(const DDNode<ValT>& ) const;
    unsigned numNodes() const;
    unsigned numNodes(const DDNode<ValT>* ) const;
 
    // The decision diagrams root. 
    void setRoot(DDNode<ValT>* source); // deletes root and sets root to source.
    DDNode<ValT>* getRoot() const;      // gets root; no copy.
    DDNode<ValT>* stealRoot();          // returns root; no copy and sets root to nullptr.
    DDNode<ValT>* getCopy() const;      // gets a copy of the root tree.
    
    DDNode<ValT>* root;                 // return the root of BDD or AADD.
    static unsigned long last;
};


/*
 * Implementations must be in header, at least for clang.
 * Is in aadd_base_impl.h to hide details. 
 */

#include "aadd_ddbase_impl.h"


#endif 
