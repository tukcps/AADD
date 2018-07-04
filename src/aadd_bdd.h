
/**
 
 @file aadd_bdd.h
 @ingroup AADD
 @brief BDD Part of of Affine Arithmetic Desion Diagrams.
 @details Defines data structure of ROBDD and helper functions for GLPK acces etc.
 @author Carna Radojicic, Christoph Grimm
 @copyright@parblock
 Copyright (c) 2017  Carna Radojicic, Christoph Grimm, 
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

#ifndef aadd_bdd_h
#define aadd_bdd_h

#include "aadd_ddbase.h"



class BDDNode: public DDNode<bool> {
public: 
    BDDNode(const BDDNode& source);        // copy constructor
    BDDNode(const unsigned long index,
            BDDNode* T, BDDNode* F,
            const bool value = false);
    
    // some casts that are safe.
    BDDNode* getT() const { return (BDDNode*) T; };
    BDDNode* getF() const { return (BDDNode*) F; };
    bool getValue() const { return value; };
    

    void print(const string&, std::ostream &) const;
    

};


/*
 Function calls that take operation as parameter.
 */
typedef BDDNode* (*BDD_AOP)(BDDNode *,BDDNode *);
typedef BDDNode* (*BDD_AOPC)(BDDNode*, const bool &);
typedef BDDNode* (*BDD_UOP)(BDDNode*);
BDDNode* ITE(BDDNode*, BDDNode*, BDDNode*);


/**
 @brief BDD class
 @author Carna Radojicic, Christoph Grimm
 */
class BDD: public DDBase<bool>
{
public:

    BDD& ITE(const BDD&, const BDD&, const BDD&);
   
    // Comparison of BDD with other BDD.
    // Comparison of BDD with bool.
    BDD& operator==(const BDD&) const;
    BDD& operator!=(const BDD&) const;
    
    // Logical operators on BDD
    BDD& operator!() const;
    BDD& operator and(const BDD&) const;
    BDD& operator &=(const BDD&);
    BDD& operator |=(const BDD&);
    
    BDD& operator or(const BDD&) const;
    BDD& operator xor(const BDD& g) const;
    
    // assignment method -- called by op =
    BDD& assign(const BDD&);
    
    // assignment operators
    BDD& operator=(const BDD&);

  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wc++11-extensions"
    explicit operator bool() const;
  #pragma clang diagnostic pop
    
    BDD& nand(const BDD& g) const;
    BDD& nor(const BDD& g) const;
    BDD& xnor(const BDD& g) const;
    

    // Applies operators to the corresponding nodes f and g of binary trees
    BDDNode* Complement(BDDNode* ) const;
    BDDNode* ApplyBinOp(BDD_AOP op, BDDNode* f, BDDNode* g) const;
    BDDNode* ApplyBinOpC(BDD_AOPC op, BDDNode* f, const bool g) const;
    BDDNode* ApplyUnaryOp(BDD_UOP op, BDDNode* f) const;
    BDDNode* getRoot() const {return (BDDNode*) root;}; // safe cast.
    
    // Prints BDD to stream s
    void print(std::ostream & s) const;

    BDD(bool = false);
    BDD(int);
    BDD(const BDD& from);
    BDD(const AADD& from);
    BDD(BDDNode* root);
    ~BDD();

    
protected:
};

/**
 @brief Creates a unique terminal node with the value 1
 */
BDDNode* ONE();

/**
 @brief Creates a unique terminal node with the value 0
 */
BDDNode* ZERO();

BDDNode* And(BDDNode*, BDDNode*);
BDDNode* Or(BDDNode*, BDDNode*);
BDDNode* Xor(BDDNode*, BDDNode*);
BDDNode* Inv(BDDNode*);
BDDNode* Floor(BDDNode*);
BDDNode* Negate(BDDNode*);


std::ostream & operator << (std::ostream & s, const BDD& f);

#endif /* aadd_bdd_h */
