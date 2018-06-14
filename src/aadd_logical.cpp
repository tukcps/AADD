/**
 
 @file aadd_logical.cpp
 
 @ingroup AADD
 
 @brief Logical operations with AADD.
 
 @author Carna Radojicic, Christoph Grimm
 
 @copyright@parblock
 Copyright (c) 2017  Carna Radojicic Design of Cyber-Physical Systems
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
#include <assert.h>
#include "aadd_bdd.h"



/**
 @brief Private method called by unary logical NOT operator !
 @return A copy of the BDD root, but with its complement function. 
 @details Reverses recursively the logical values of AADD terminal vertices:
          If a value is one, then the reversed value is 0 and vice versa.
 @author Carna Radojicic, Christoph Grimm
 @see operator !
 */
BDDNode* BDD::Complement(BDDNode* f) const
{
    // The leaves ...
    if (f->isLeaf() ) {
        if (f == ZERO()) {
            return(ONE());
        } else {
            return(ZERO());
        }
    }
    
    // The tree
    BDDNode* Tr = Complement(f->getT());
    assert(Tr != nullptr);
    
    BDDNode* Fa = Complement(f->getF());
    assert(Fa != nullptr);
    
    BDDNode* Tree=new BDDNode(f->getIndex(), Tr, Fa);
    assert(Tree != nullptr);
    return(Tree);
}

/**
 
 @brief Function called by the binary logical operator or
 @details Function performed on terminal vertices with Boolean values
 @author Carna Radojicic
 @return NULL if not a terminal case or vertices are not the same

*/
BDDNode* Or(BDDNode* f, BDDNode* g)
{
    BDDNode *F, *G;
    BDDNode* one=ONE();
    BDDNode* zero=ZERO();
    
    F = f; G = g;
    
    if (F == one || G == one) return(one);
    if (F->isLeaf() and G->isLeaf()) return(zero);
    
    
    return(NULL);
}

/**
 
 @brief Function called by the binary logical operator or
 @details Function performed on terminal vertices with Boolean values
 @author Carna Radojicic
 @return NULL if not a terminal case or vertices are not the same
 
 */
BDDNode* And(BDDNode* f, BDDNode* g)
{
    BDDNode *F, *G;
    BDDNode* one=ONE();
    BDDNode* zero=ZERO();

    F = f; G = g;
    
    if (F == one && G == one) return(one);
    if (F==zero || G==zero) return(zero);
    
   /* if (F->isLeaf() and ) return(G);
    if (G->isLeaf() ) return(F);
    if (F == G) return(F); */
    
    return(nullptr);
}


/**
 @brief Function called by the binary logical operator xor
 @details Function performed on terminal vertices with Boolean values
 @author Carna Radojicic
 @return NULL if not a terminal case or vertices are not the same
 */
BDDNode* Xor(BDDNode* f, BDDNode* g)
{
    BDDNode *F, *G;
    
    BDDNode* one=ONE();
    BDDNode* zero=ZERO();
    
    F = f; G = g;
    if (F == G) return(zero);
    if (F == one && G == zero)
    {
        return(one);
    }
    if (G == one && F == zero)
    {
        return(one);
    }
    return(nullptr);
}


BDD& BDD::operator!() const
{
    BDD* Temp=new BDD;
    Temp->root=Complement(getRoot());
    return (*Temp);
} // BDD::operator!



BDD& BDD::operator and(const BDD& other) const
{
    BDD* Temp=new BDD;
    Temp->root=ApplyBinOp(And, getRoot(), other.getRoot());
    return (*Temp);
} // BDD::operator and

BDD& BDD::operator &=(const BDD& other)
{
    BDDNode* result=ApplyBinOp(And, getRoot(), other.getRoot());
    delete root;
    root=result;
    return (*this);
} // BDD::operator &=


BDD& BDD::operator or(const BDD& other) const
{
    BDD* Temp=new BDD;
    Temp->root=ApplyBinOp(Or, getRoot(), other.getRoot());
    
    return (*Temp);
} // BDD::operator or

BDD& BDD::operator |=(const BDD& other)
{
    BDDNode* result=ApplyBinOp(Or, getRoot(), other.getRoot());
    delete root;
    root=result;
    return (*this);
} // BDD::operator |=


BDD& BDD::operator xor(const BDD& other) const
{
    BDD* Temp=new BDD;
    Temp->root=ApplyBinOp(Xor, getRoot(), other.getRoot());
    
    return (*Temp);
} // BDD::operator xor


BDD& BDD::nand(const BDD& other) const
{
    BDD* Temp=new BDD(!((*this) and other));
    return (*Temp);
} // BDD::operator nand


BDD& BDD::nor(const BDD& other) const
{
    BDD* Temp=new BDD(!((*this) or other));
    (*Temp)=(!((*this) or other));
    return (*Temp);
} // BDD::operator nor


BDD& BDD::xnor(const BDD& other) const
{
    BDD* Temp=new BDD(!((*this) xor other));
    return (*Temp);
    
} // BDD::operator xor

/**
 @brief Private method called by binary logical operators
 @details Called recursively until all terminal vertices are reached
 @author Carna Radojicic
 @return the root node of BDD
 @see binary operators
 */
BDDNode* BDD::ApplyBinOp(BDD_AOP op, BDDNode*f, BDDNode* g) const
{
    BDDNode *res,
	   *fv, *fvn,
    *gv, *gvn,
	   *T, *E;
    unsigned long ford, gord;
    unsigned long index;
    
    /* Check terminal cases */
    res = (*op)(f,g);
    if (res != nullptr) return(res);
    
    /* Recursive step. */
    ford = f->getIndex();
    gord = g->getIndex();
    
    if (ford <= gord) {
        index = f->getIndex();  // taks smaller index of leaves
        fv =  (BDDNode*)f->getT();   // and its leaves T,F
        fvn = (BDDNode*)f->getF();
    } else {
        index = g->getIndex();        // else take other index
        fv = fvn = f;            // and skip; keep both leaves
    }
    if (gord <= ford) {
        gv =  (BDDNode*)g->getT();    // Take leaves of other op if index valid
        gvn = (BDDNode*)g->getF();
    } else {                     // skip if invalid.
        gv = gvn = g;
    }
    
    T = ApplyBinOp(op,fv,gv);    // Call OP on T leaf
    if (T == nullptr) return(nullptr);
    E = ApplyBinOp(op,fvn,gvn);  // Call OP on F leaf
    if (E == nullptr) {
        delete T;
        return(nullptr);
    }
    
    // Maybe we can reduce?
    if (T->isLeaf() and E->isLeaf() )
    {
        if (T->getValue()==E->getValue())
        {
            res=T;
        }
        else
        {
            res=new BDDNode(index,T,E);
        }
    }
    else if (T==E)
    {
        res=T;
    }
    else
    {
        res=new BDDNode(index,T,E);
    }
    return res;
}

