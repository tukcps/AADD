/**
 
 @file aadd.h
 
 @ingroup AADD
 
 @brief Affine Arithmetic Desion Diagrams, main include file.
 
 @details The file defines the visible data structure of AADD.
 @details Furthermore, the user-visible definitiona for code instrumentation are here.
 
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
#ifndef aadd_h
#define aadd_h

#include "aadd_config.h"
#include "aadd_ddbase.h"
#include "aadd_bdd.h"
#include "aadd_lp_glpk.h"

/*
 Function calls that take operation as parameter.
 */
class AADDNode;
typedef AADDNode* (*AADD_AOP)(AADDNode *,AADDNode *);
typedef AADDNode* (*AADD_AOPC)(AADDNode*, const AAF &);
typedef AADDNode* (*AADD_UOP)(AADDNode*);


/**
 @brief Nodes of an AADD with defintion of operations on AADD
 @authors Christoph Grimm
 */
class AADDNode: public DDNode<AAF> {
public: 
    // AADDNode();
    AADDNode(const AAF&);
    AADDNode(const AADDNode& source); // copy constructor
    AADDNode(const unsigned long index, AADDNode* T, AADDNode* F);

    // some casts that are always true.
    AADDNode* getT() const { return (AADDNode*) T; };
    AADDNode* getF() const { return (AADDNode*) F; };
    
    // print node to stream s with indent of string. 
    void print(const string&, std::ostream &s = std::cout) const;
};




/**
 @brief The AADD class implements a decision diagram that takes AAF as leaf nodes and AAF as conditions.
 @details The AADD class uses a tree of AADDNode objects to set up a decision diagram that has AAF objects as leafs. 
The AADD class mostly handles the integration in C++ semantics and memory management. 
 @authors Carna Radojicic, Christoph Grimm
 */
class AADD: public DDBase<AAF>
{
  public:
    // constructors
    AADD(double cst=0.);
    AADD(double lb, double ub); 
    AADD(int cst);
    AADD(const AAF&);
    
    //copy constructors
    AADD(const AADD& from);
    AADD(const AADDNode& from);
    
    // destructor
    ~AADD();
    
    // Assigment operators
    AADD& assign(const AADD& right);
    AADD& operator=(const AADD& right);
    
    // Relational operators    
    BDD& operator<=(const AADD&) const;
    BDD& operator>=(const AADD&) const;
    BDD& operator<(const AADD&) const;
    BDD& operator>(const AADD&) const;
    BDD& operator==(const AADD&) const;
    BDD& operator!=(const AADD&) const;
    
    // Arithmetic operators
    AADD& operator-() const;
    AADD& operator*(const AADD&) const;
    AADD& operator*=(const AADD&);
    AADD& operator/(const AADD&) const;
    AADD& operator/=(const AADD&);
    AADD& operator+(const AADD&) const;
    AADD& operator+=(const AADD&);
    AADD& operator-(const AADD&) const;
    AADD& operator-=(const AADD&);

// folloing will be removed: 
// Is handled by C++ constructor of AADD from double, AAF, int anyhow. 
    AADD& operator*(double)const;
    AADD& operator*=(double);
    AADD& operator/(double)const;
    AADD& operator/=(double);
    AADD& operator+=(double);
    AADD& operator-(double) const;
    AADD& operator-=(double);
    AADD& operator*(int)const;
    AADD& operator*=(int);
    AADD& operator/(int)const;
    AADD& operator/=(int);
    AADD& operator+=(int);
    AADD& operator-(int) const;
    AADD& operator-=(int);
    AADD& operator*(const AAF&)const;
    AADD& operator*=(const AAF&);
    AADD& operator/(const AAF&)const;
    AADD& operator/=(const AAF&);
    AADD& operator+=(const AAF&);
    AADD& operator-(const AAF&) const;
    AADD& operator-=(const AAF&);

    AADD& operator%(int)const;
 
   
    // Unary operations
    friend AADD& inv(const AADD&);
    friend AADD& abs(const AADD&);
    friend AADD& floor(const AADD&);
    
    // ITE function on two AADD with a BDD as condition
    AADD& ITE(const BDD&,const AADD&,const AADD&);
    AADD& ITE(const BDD&,const AAF&, const AAF&);

    // For reducion of AADD
    // AADD& join(AADDnode
    
    // Helpers for implementation of operations.
    AADDNode* ApplyBinOp(AADD_AOP op, AADDNode* f, AADDNode* g) const;
    AADDNode* ApplyBinOpC(AADD_AOPC op, AADDNode* f, const AAF& g) const;
    AADDNode* ApplyUnaryOp(AADD_UOP op, AADDNode* f) const;
    AADDNode* BTimesA(BDDNode* f, AADDNode* g) const; 
    AADDNode* Join(AADDNode* f, vector<constraint<AAF> >) const;
    
    AADDNode* getRoot() const { return (AADDNode*) root; };

    // find bounds of ranges of tree leafs
    double GetMin() const;
    double GetMax() const;
    opt_sol GetBothBounds() const;
    
    // printing AADD to stream s, default is cout
    void print(std::ostream & s=std::cout) const;

    // writing AADD to the file format recognised by graph drawing program Graphviz
    int printaadd(string file_name) const;
    
  protected:
    // Called by relational operators
    BDDNode* Compare(AADDNode*, double, vector<constraint<AAF> >, string) const;
    
    // Recursive functions for Arithmetic operators
    AADDNode* Modulo(AADDNode*, int) const;
    
    // Method that computes tight bounds of leafs of AAF by setting up an LP problem. Calls GLPK for solving it. 
    opt_sol FindBounds(AADDNode*,  vector<constraint<AAF> >) const;
};


// Operators that take lvalue of non-AADD type.
AADD& operator * (double, const AADD&);
AADD& operator / (double, const AADD&);
AADD& operator + (double, const AADD&);
AADD& operator - (double, const AADD&);

AADD& operator * (int, const AADD&);
AADD& operator / (int, const AADD&);
AADD& operator + (int, const AADD&);
AADD& operator - (int, const AADD&);

AADD& operator * (const AAF&, const AADD&);
AADD& operator / (const AAF&, const AADD&);
AADD& operator + (const AAF&, const AADD&);
AADD& operator - (const AAF&, const AADD&);


// Operations on AADDNodes for internal use:
AADDNode* Times(AADDNode*, AADDNode*);
AADDNode* Divide(AADDNode*, AADDNode*);
AADDNode* Plus(AADDNode*, AADDNode*);
AADDNode* Minus(AADDNode*, AADDNode*);

AADDNode* TimesC(AADDNode*, const AAF&);
AADDNode* DivideC(AADDNode*, const AAF&);
AADDNode* PlusC(AADDNode*, const AAF&);
AADDNode* MinusC(AADDNode*, const AAF&);


// For IO
std::ostream & operator << (std::ostream & s, const AADD& f);

// For use in C++:
// Can be switched off by including aadd_disabled instead.
// Then, symbolic execution is turned totally off.
typedef class AADD doubleS;
typedef class AADD floatS;
typedef class AADD intS;
typedef class AADD realS;
typedef class BDD  boolS;

#include "aadd_mgr.h"
#include "aadd_macros.h"

#endif /* aadd_h */
