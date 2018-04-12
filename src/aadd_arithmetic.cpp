/**
 
 @file aadd_arithmetic.cpp
 
 @ingroup AADD
 
 @brief Arithmetic operations on AADD; implementation. 
 
 @author Carna Radojicic
 
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

#include "aadd.h"
#include <math.h>

inline double max(const double val1, const double val2)
{
    if (val1>val2)
        return val1;
    else
        return val2;
}

inline double min(const double val1, const double val2)
{
    if (val1<val2)
        return val1;
    else
        return val2;
}

/**
 
 @brief Function that performs floor C++ function on AADD
 
 @details Function called by AADD public method floor
 applicable only on real values of terminal vertices
 
 @author Carna Radojicic
 
 @return the root of AADD
 
 @see floor
 
 */

AADDNode* Floor(AADDNode* f)
{
    AADDNode *res;
    
    AAF value;
    
    /* Check terminal cases. */
    if ( f->isLeaf() ) {
        
        value=f->getValue();
        
        if (value.getlength()==0)
            value=floor(f->getValue().getcenter());
        else
        {
            cout << "Operation on ranges not defined!" << endl;
            exit(1);
        }
        
        res = new AADDNode(value);
        return(res);
    }
    return nullptr;
}

/**
  @brief Function that computes inversion of value of a terminal vertex in AADD (1./Value(v))
  @details Function called by AADD public method inv
  @author Carna Radojicic
  @return NULL if not a terminal case, otherivise terminal vertex with (1./Value(v))
  @see inv
 */
AADDNode* Inv(AADDNode* f)
{
    AADDNode *res;
    
    AAF value;
    
    /* Check terminal cases. */
    if (f->isLeaf() ) {
        value=inv(f->getValue());
        res = new AADDNode(value);
        return(res);
    }
    return NULL;
}

/**
 @brief Public method that performs 1./g
 @author Carna Radojicic
 @return AADD
 */
AADD& inv(const AADD& g)
{
    AADD* Temp=new AADD;
    Temp->root=g.ApplyUnaryOp(Inv, g.getRoot());
    return (*Temp);
}

/**
 @brief Public method that performs floor(g)
 @author Carna Radojicic
 @return AADD
 */
AADD& floor(const AADD& g)
{
    AADD* Temp=new AADD;
    Temp->root=g.ApplyUnaryOp(Floor, g.getRoot());
    return (*Temp);
}

/**
 @brief Public method that performs |g|
 @author Carna Radojicic
 @return AADD
 */
AADD& abs(const AADD& g)
{
    BDD cond=(g<0.);
    AADD* Temp=new AADD;
    
    if (cond==true)
    {
        (*Temp)=(-g);
        return (*Temp);
    }
    else if (cond==false)
    {
        (*Temp)=g;
        return (*Temp);
    }
    else
    {
        (*Temp)=Temp->ITE(cond,-g,g);
        return (*Temp);
    }
}

/**
 
 @brief Functions called by private method ApplyBinOp
 @details Perform binary operation * on terminal vertices
 @author Carna Radojicic
 @return NULL if not a terminal case, otherwise a terminal vertex
 
 @see ApplyBinOp
 
 */
AADDNode* Times(AADDNode* f, AADDNode* g)
{
    AADDNode *res;
    AAF value;
    
    if (f->isLeaf() && g->isLeaf()) {
        value= f->getValue()*g->getValue();
        res = new AADDNode(value);
        return(res);
    }
    return(NULL);
}

AADDNode* TimesC(AADDNode* f, const AAF& g)
{
    AADDNode *res;
    AAF value;
    
    if (f->isLeaf()) {
        value=f->getValue()*g;
        res = new AADDNode(value);
        return(res);
    }
    return(NULL);
}


/**
 @brief Functions called by private method ApplyBinOp
 @details Perform binary operation / on terminal vertices
 @author Carna Radojicic
 @return NULL if not a terminal case, otherwise a terminal vertex
 @see ApplyBinOp
 */
AADDNode* Divide(AADDNode* f, AADDNode* g)
{
    AADDNode *res;
    AAF value;
    
    if (f-> isLeaf() && g->isLeaf() ) {
        value = f->getValue()/g->getValue();
        res = new AADDNode(value);
        return(res);
    }
    return(NULL);
}

AADDNode* DivideC(AADDNode* f, const AAF& g)
{
    AAF value;

    if (f->isLeaf()) {
        value = (f->getValue())/g;
        return(new AADDNode(value));
    }
    return(NULL);
}

/**
 @brief Functions called by private method ApplyBinOp
 @details Perform binary operation + on terminal vertices
 @author Carna Radojicic
 @return NULL if not a terminal case, otherwise a terminal vertex
 @see ApplyBinOp
 */
AADDNode* Plus(AADDNode* f, AADDNode* g)
{
    if (f->isLeaf() && g->isLeaf() ) {
        AAF value=f->getValue()+g->getValue();
        return(new AADDNode(value));
    }
    return(NULL);
}

AADDNode* PlusC(AADDNode* f, const AAF& g)
{
    if (f->isLeaf() ) {
        AAF value=f->getValue()+g;
        return(new AADDNode(value));
    }
    return(NULL);
}

/**
 @brief Functions called by private method ApplyBinOp
 @details Perform binary operation - on terminal vertices
 @author Carna Radojicic
 @return NULL if not a terminal case, otherwise a terminal vertex
 @see ApplyBinOp
 */
AADDNode* Minus(AADDNode* f, AADDNode* g)
{
    if ( f->isLeaf() && g->isLeaf() ) {
        AAF value= f->getValue()- g->getValue();
        return(new AADDNode(value));
    }
    return(NULL);
    
}

AADDNode* MinusC(AADDNode* f, const AAF& g)
{
    AAF value;
    
    if (f->isLeaf() ) {
        value= f->getValue()-g;
        return(new AADDNode(value));
    }
    return(nullptr);
}

/**
 @brief Function called by private method ApplyUnaryOp
 @details Performs - on value of terminal vertices v (-Value(v))
 @author Carna Radojicic
 @return NULL if not a terminal case, otherwise a terminal vertex
 @see ApplyUnaryOp
 */
AADDNode* Negate(AADDNode* f)
{
    /* Check terminal cases. */
    if (f->isLeaf() ) {
        return(new AADDNode(-(f->getValue())));
    }
    return nullptr;
}

/**
 @brief Private method called by unary operator -
 @details Called recursively until all terminal vertices are reached
 @author Carna Radojicic
 @return the root of AADD
 @see operator -()
 */
AADDNode* AADD::ApplyUnaryOp(AADD_UOP op, AADDNode* f) const
{
    AADDNode *res,
	   *fv, *fvn,
	   *T, *E;
    
    unsigned long index;
    
    res = (*op)(f);
    assert(res != nullptr);
    if (res != nullptr) return(res);
    
    index = f->getIndex();
    fv=(AADDNode*)f->getT();
    fvn=(AADDNode*)f->getF();
    
    T = ApplyUnaryOp(op,fv);
    if (T == nullptr) return(nullptr);
    
    E = ApplyUnaryOp(op,fvn);
    if (E == nullptr) {
        delete T;
        return(nullptr);
    }
    
    if (T->isLeaf() and E->isLeaf())
    {
        if (T->getValue()==E->getValue())
        {
            res=T;
        }
        else
        {
            res=new AADDNode(index,T,E);
        }
    }
    else if (T==E)
    {
        res=T;
    }
    else
    {
        res=new AADDNode(index,T,E);
    }
    
    return(res);
    
}



/**
 @brief Private method called by binary operators (*, +, -, /)
 @details Called recursively until all terminal vertices are reached
 @author Carna Radojicic
 @return the root of AADD
 @see binary operators
 */
AADDNode* AADD::ApplyBinOp(AADD_AOP op, AADDNode*f, AADDNode* g) const
{
    AADDNode *res,
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
        index = f->getIndex();        // taks smaller index of leaves
        fv =  (AADDNode*)f->getT();   // and its leaves T,F
        fvn = (AADDNode*)f->getF();
    } else {
        index = g->getIndex();        // else take other index
        fv = fvn = f;            // and skip; keep both leaves
    }
    if (gord <= ford) {
        gv = (AADDNode*)g->getT();    // Take leaves of other op if index valid
        gvn = (AADDNode*)g->getF();
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
            res=new AADDNode(index,T,E);
        }
    }
    else if (T==E)
    {
        res=T;
    }
    else
    {
        res=new AADDNode(index,T,E);
    }
    return res;
}



AADDNode* AADD::ApplyBinOpC(AADD_AOPC op, AADDNode*f, const AAF& cst) const
{
    AADDNode *res,
	   *fv, *fvn,
	   *T, *E;
    
    
    res = (*op)(f, cst);
    
    if (res != NULL) return(res);
    
    unsigned long index = f->getIndex();
    fv=f->getT();
    fvn=f->getF();
    
    T = ApplyBinOpC(op,fv,cst);
    if (T == NULL) return(NULL);
    
    E = ApplyBinOpC(op,fvn, cst);
    if (E == NULL) {
        delete T;
        return(NULL);
    }
    
    if (T->isLeaf() and E-> isLeaf() )
    {
        if (T->getValue()==E->getValue())
        {
            res=T;
        }
        else
        {
            res=new AADDNode(index,T,E);
        }
    }
    else if (T==E)
    {
        res=T;
    }
    else
    {
        res=new AADDNode(index,T,E);
    }
    
    return(res);
}

/**
 @brief Private method called by binary operators (*, +, -, /)
 @details Called recursively until all terminal vertices are reached
 @author Carna Radojicic
 @return the root of AADD
 @see binary operators
 */
AADDNode* AADD::BTimesA(BDDNode*f, AADDNode* g) const
{
    

    AADDNode *res;
    BDDNode  *fv, *fvn;
    AADDNode *gv, *gvn, *T, *E;
    unsigned long ford, gord;
    unsigned long index;
    
    /* Check terminal cases */
    if (f->isLeaf() && g->isLeaf() ) {
        if (f->getValue() == false )
            res=new AADDNode(0.0);
        else
            res = new AADDNode(g->getValue());
        return(res);
    }
    
    /* Recursive step. */
    ford = f->getIndex();
    gord = g->getIndex();
    
    if (ford <= gord) {
        index = f->getIndex();        // taks smaller index of leaves
        fv =  f->getT();   // and its leaves T,F
        fvn = f->getF();
    } else {
        index = g->getIndex();        // else take other index
        fv = fvn = f;            // and skip; keep both leaves
    }
    if (gord <= ford) {
        gv = g->getT();    // Take leaves of other op if index valid
        gvn = g->getF();
    } else {                     // skip if invalid.
        gv = gvn = g;
    }
    
    T = BTimesA(fv, gv);    // Call OP on T leaf
    if (T == nullptr) return(nullptr);
    E = BTimesA(fvn, gvn);  // Call OP on F leaf
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
            res=new AADDNode(index,T,E);
        }
    }
    else if (T==E)
    {
        res=T;
    }
    else
    {
        res=new AADDNode(index,T,E);
    }
    return res;
}


/**
 @brief Unary operator -
 @author Carna Radojicic
 @return AADD multiplied by (-1)
 */
AADD& AADD::operator-() const
{
    AADD* Temp=new AADD;
    Temp->root=ApplyUnaryOp(Negate, getRoot());
    return (*Temp);
} // AADD::operator-

// Binary arithmetic operators


AADD& AADD::operator*(const AADD& other) const
{
    AADD* Temp=new AADD;
    Temp->root=ApplyBinOp(Times, getRoot(), other.getRoot());
    
    return (*Temp);
    
} // AADD::operator*


AADD& AADD::operator*(const AAF& cst) const
{
    AADD* Temp=new AADD;
    Temp->root=ApplyBinOpC(TimesC, getRoot(), cst);
    return (*Temp);
    
}

AADD& AADD::operator*(double cst) const
{
    AAF tmp=cst;
    AADD* Temp=new AADD;
    Temp->root=ApplyBinOpC(TimesC, getRoot(), tmp);
    
    return (*Temp);
}

AADD& AADD::operator*(int cst) const
{
    AAF tmp=cst;
    AADD* Temp=new AADD;
    Temp->root=ApplyBinOpC(TimesC, getRoot(), tmp);
    return (*Temp);
}

AADD& AADD::operator/(const AADD& other) const
{
    AADD* Temp=new AADD;
    Temp->root=ApplyBinOp(Divide, getRoot(), other.getRoot());
    return (*Temp);
} // AADD::operator/


AADD& AADD::operator/(const AAF& cst) const
{
    AADD* Temp=new AADD;
    Temp->root=ApplyBinOpC(DivideC, getRoot(), cst);
    return (*Temp);
}

AADD& AADD::operator/(double cst) const
{
    AAF tmp(cst);
    AADD* Temp=new AADD;
    Temp->root=ApplyBinOpC(DivideC, getRoot(), tmp);
    
    return (*Temp);
}

AADD& AADD::operator/(int cst) const
{
    AAF tmp(cst);
    AADD* Temp=new AADD;
    
    Temp->root=ApplyBinOpC(DivideC, getRoot(), tmp);
    
    return (*Temp);
}

AADD& AADD::operator+(const AADD& other) const
{
    AADD* Temp=new AADD;
  #ifdef JOIN
    vector<constraint <AAF> > constraints;
    Temp->root=Join((ApplyBinOp(Plus, getRoot(), other.getRoot())), constraints);
  #else
     Temp->root=ApplyBinOp(Plus, getRoot(), other.getRoot());
  #endif

    return (*Temp);
} // AADD::operator+



AADD& AADD::operator+(const AAF& cst) const
{
    AADD* Temp=new AADD;
    Temp->root=ApplyBinOpC(PlusC, getRoot(), cst);
    return (*Temp);
}

AADD& AADD::operator+(double cst) const
{
    AAF tmp(cst);
    AADD* Temp=new AADD;
    Temp->root=ApplyBinOpC(PlusC, getRoot(), tmp);
    return (*Temp);
}

AADD& AADD::operator+(int cst) const
{
    AAF tmp(cst);
    AADD* Temp=new AADD;
    Temp->root=ApplyBinOpC(PlusC, getRoot(), tmp);
    return (*Temp);
}

AADD& AADD::operator-(const AADD& other) const
{
    AADD* Temp=new AADD;
    Temp->root=ApplyBinOp(Minus, getRoot(), other.getRoot());
    return (*Temp);
    
} // AADD::operator-


AADD& AADD::operator-(const AAF& cst) const
{
    AADD* Temp=new AADD;
    Temp->root=ApplyBinOpC(MinusC, getRoot(), cst);
    return (*Temp);
}

AADD& AADD::operator-( double cst) const
{
    AAF tmp=cst;
    AADD* Temp=new AADD;
    Temp->root=ApplyBinOpC(MinusC, getRoot(), tmp);
    return (*Temp);
}

AADD& AADD::operator-(int cst) const
{
    AAF tmp=cst;
    AADD* Temp=new AADD;
    Temp->root=ApplyBinOpC(MinusC, getRoot(), tmp);
    return (*Temp);
}

AADD& AADD::operator*=(const AADD& other)
{
    AADDNode *result = ApplyBinOp(Times, getRoot(), other.getRoot());
    delete root;

    root=result;
    return (*this);
} // AADD::operator*=

AADD& AADD::operator*=(double cst)
{
    AAF tmp=cst;
    AADDNode *result = ApplyBinOpC(TimesC, getRoot(), tmp);
    delete root;
    root=result;
    return (*this);
}

AADD& AADD::operator*=(int cst)
{
    AAF tmp=cst;
    AADDNode *result = ApplyBinOpC(TimesC, getRoot(), tmp);
    delete root;
    root=result;
    return (*this);
}

AADD& AADD::operator*=( const AAF& cst)
{
    AADDNode *result = ApplyBinOpC(TimesC, getRoot(), cst);
    delete root;
    root=result;
    return (*this);
}


AADD& AADD::operator/=(const AADD& other)
{
    AADDNode *result = ApplyBinOp(Divide, getRoot(), other.getRoot());
    delete root;
    root=result;
    return (*this);
} // AADD::operator/=


AADD& AADD::operator/=(double cst)
{
    AAF tmp(cst);
    AADDNode *result = ApplyBinOpC(DivideC, getRoot(), tmp);
    delete root;
    root=result;
    return (*this);
}

AADD& AADD::operator/=(int cst)
{
    AAF tmp(cst);
    AADDNode *result = ApplyBinOpC(DivideC, getRoot(), tmp);
    delete root;
    root=result;
    return (*this);
}

AADD& AADD::operator/=(const AAF& cst)
{
    AADDNode *result = ApplyBinOpC(DivideC, getRoot(), cst);
    delete root;
    root=result;
    return (*this);
}

AADD& AADD::operator+=(const AADD& other)
{
    AADDNode *result = ApplyBinOp(Plus, getRoot(), other.getRoot());
    delete root;
    root=result;
    return (*this);
} // AADD::operator+=

AADD& AADD::operator+=(double cst)
{
    AAF tmp(cst);
    AADDNode *result = ApplyBinOpC(PlusC, getRoot(), tmp);
    delete root;
    root=result;
    return (*this);
}

AADD& AADD::operator+=(int cst)
{
    AAF tmp(cst);
    AADDNode *result = ApplyBinOpC(PlusC, getRoot(), tmp);
    delete root;
    root=result;
    return (*this);
}


AADD& AADD::operator+=(const AAF& cst)
{
    AADDNode *result = ApplyBinOpC(PlusC, getRoot(), cst);
    delete root;
    root=result;
    return (*this);
}

AADD& AADD::operator-=(const AADD& other)
{
    AADDNode *result = ApplyBinOp(Minus, getRoot(), other.getRoot());
    delete root;
    root=result;
    return (*this);
} // AADD::operator-=

AADD& AADD::operator-=(double cst)
{
    AAF tmp(cst);
    AADDNode *result = ApplyBinOpC(MinusC, getRoot(), tmp);
    delete root;
    root=result;
    return (*this);
}

AADD& AADD::operator-=(int cst)
{
    AAF tmp(cst);
    AADDNode *result = ApplyBinOpC(MinusC, getRoot(), tmp);
    delete root;
    root=result;
    return (*this);
    
}


AADD& AADD::operator-=(const AAF& cst)
{
    AADDNode *result = ApplyBinOpC(MinusC, getRoot(), cst);
    delete root;
    root=result;
    return (*this);
    
}

AADD& operator * (double cst, const AADD& P)
{
    AADD* Temp=new AADD(P*cst);
    return (*Temp);
}

AADD& operator * (int cst, const AADD& P)
{
    AADD* Temp=new AADD(P*cst);
    return (*Temp);
}


AADD& operator / (double cst, const AADD& P)
{
    
    AADD* Temp=new AADD(inv(P)*cst);
    return (*Temp);
}

AADD& operator / (int cst, const AADD& P)
{
    AADD* Temp=new AADD(inv(P)*cst);
    return (*Temp);
}

AADD& operator + (double cst, const AADD& P)
{
    
    AADD* Temp=new AADD(P+cst);
    return (*Temp);
}

AADD& operator + (int cst, const AADD& P)
{
    
    AADD* Temp=new AADD(P+cst);
    return (*Temp);
}


AADD& operator - (double cst, const AADD& P)
{
    AADD* Temp=new AADD(-P+cst);
    return (*Temp);
}

AADD& operator - (int cst, const AADD& P)
{
    AADD* Temp=new AADD(-P+cst);
    return (*Temp);
}


AADD& operator * (const AAF& cst, const AADD& P)
{
    
    AADD* Temp=new AADD(P*cst);
    return (*Temp);
    
}

AADD& operator / (const AAF& cst, const AADD& P)
{
    AADD* Temp=new AADD(inv(P)*cst);
    return (*Temp);
    
    
}

AADD& operator + (const AAF& cst, const AADD& P)
{
    
    AADD* Temp=new AADD(P+cst);
    return (*Temp);
    
    
}

AADD& operator - (const AAF& cst, const AADD& P)
{
    
    AADD* Temp=new AADD(-P+cst);
    return (*Temp);
    
}

/**
 @brief Privated method called by binary operator %
 @details Performed recursively on integer values of terminal vertices
 until all of them are reached
 
 @author Carna Radojicic, Christoph Grimm
 
 @return terminates if value of a terminal vertex is a range, otherwise the root of AADD
 
 @see operator %
 
 */

AADDNode* AADD::Modulo(AADDNode* f, int cst) const
{
    AADDNode *res;
    AADDNode *F;
    
    AADDNode *T,*E;
    AADDNode *fv, *fvn;
    
    double value;
    
    unsigned long index;
    
    F = f;
    if (cst==0)
    {
        cout << "Error: Remainder by ZERO is undefined!" << endl;
        exit(1);
    }
    
    if (F->isLeaf() ) {
        
        if ((F->getValue()).getlength()==0)
        {
            value=((int)((F->getValue()).getcenter()))%cst;
        }
        else
        {
            cout << "Error: Remainder not defined on ranges!" << endl;
            exit(1);
        }
        
        res = new AADDNode(value);
        return(res);
    }
    
    index = f->getIndex();
    fv=(f->getT());
    fvn=(f->getF());
    
    T = Modulo(fv,cst);
    if (T == NULL) return(NULL);
    
    E = Modulo(fvn,cst);
    if (E == NULL) {
        delete T;
        return(NULL);
    }
    
    if (T->isLeaf() and E->isLeaf() )
    {
        if (T->getValue()==E->getValue() )
        {
            res=T;
        }
        else
        {
            res=new AADDNode(index,T,E);
        }
        
    }
    else if (T==E)
    {
        res=T;
    }
    else
    {
        res=new AADDNode(index,T,E);
    }
    return(res);
}

/**
 @brief Binary operator %
 @author Carna Radojicic
 @return AADD
 */
AADD& AADD::operator%(int cst) const
{
    AADD* Temp=new AADD;
    Temp->root=Modulo(getRoot(), cst);
    
    return (*Temp);
    
} // AADD::operator%



AADDNode* AADD::Join(AADDNode* f, vector<constraint<AAF> > constraints) const
{
    
    AADDNode *T, *F;
    AADDNode *fv, *gv;
    constraint<AAF> cons;
    
    if (f->isLeaf() )
    {
        return f;
    }
 
    // Recursive step
    
    cons.con=f->getCond();
    
    cons.sign='+';
    
    constraints.push_back(cons);
    
    fv=(AADDNode*)f->getT();
    
    T=Join(fv, constraints); // Merge applied on T child
    
    constraints.back().sign='-';
    
    gv=(AADDNode*)f->getF();
    
    F=Join(gv, constraints); // Merge applied on F child
    
    if (T->isLeaf() and F->isLeaf() )
    {
        if (T->getValue()==F->getValue() )
        {
            f->delete_tree();
            f=T;
        }
        else
        {
            
            // maybe we can join?
            
            AAF val1, val2;
            
            val1=T->getValue();
            val2=F->getValue();
            
            double lb1, lb2;
            double ub1, ub2;
            
            opt_sol bounds2=solve_lp(val2, constraints);
            
            constraints.back().sign='+';
            opt_sol bounds1=solve_lp(val1, constraints);
            
            lb1=bounds1.min;
            lb2=bounds2.min;
            
            ub1=bounds1.max;
            ub2=bounds2.max;
            
            unsigned l1 = val1.getlength();
            unsigned l2 = val2.getlength();
            
          //  #ifdef AADD_DEBUG
            cout << "Can we join?"  << endl;
            cout << "Range of T child " << val1 << " is: [";
            cout << lb1 << " " << ub1 << "]." << endl;
            cout << "Range of F child " << val2 <<  " is: [";
            cout << lb2 << " " << ub2 << "]." << endl;
          //  #endif
            
            // check for similarity - if ranges overlap and share symbols they are similar
            
            if (lb2<ub1 and lb1<ub2)
            {
                // val1 and val2 do not share symbols
                // nothing to merge
                
                if (l1 == 0)
                {
                    return f;
                }
                
                if (l2 == 0)
                {
                    return f;
                }
                
                // check if two AAFs share symbols; if yes, join
                // if not, return original node f
                
                unsigned * id1 = val1.getIndexes();
                unsigned * id2 = val2.getIndexes();
                
                double * va1 = val1.getDeviations();
                double * va2 = val2.getDeviations();
                
                unsigned * pu1 = id1;
                unsigned * pu2 = id2;
                
                unsigned* idtemp = new unsigned [l1+l2];
                
                // Fill the resulting indexes array
                // by joining the 2 input indexes array
                
                unsigned * fin = std::set_union(id1,id1+l1,id2,id2+l2,idtemp);
                unsigned ltemp = fin-idtemp;
                
                vector<double> deviations;
                vector<unsigned> indexes;
                bool share=false;
                
                
                for (unsigned i = 0; i < ltemp; i++)
                {
                    unsigned a = pu1-id1;
                    unsigned b = pu2-id2;
                    
                    if (a == l1 || id1[a] != idtemp[i])
                    {
                        pu2++;
                        continue;
                    }
                    
                    if (b == l2 || id2[b] != idtemp[i])
                    {
                        pu1++;
                        continue;
                    }
                    
                    share=true;
                    
                    deviations.push_back(minimum(va1[a], va2[b]));
                    indexes.push_back(idtemp[i]);
                    
                    pu1++;
                    pu2++;
                    
                }
                
                if (share==false) // affine forms do not share symbols -- no join possible
                {
                    return f;
                }
                
                // otherwise affine forms share symbols and overlap
                // join
                
                AAF Temp;
                
                double min=minimum(lb1, lb2);
                double max=maximum(ub1, ub2);
                
                // center of joined range is center of [lb1, ub1] union [lb2, ub2]
                
                double center=(min+max)/2.;
                unsigned size=deviations.size();
                
                double *dev=new double[size];
                unsigned *ind=new unsigned[size];
                
                
                for (unsigned i=0; i<size; i++)
                {
                    dev[i]=deviations[i];
                    ind[i]=indexes[i];
                    
                }
                
                Temp=AAF(center, dev, ind, size);
                
                // update offest_min and offset_max to enclose interval [lb1, ub1] union [lb2, ub2]
                
                constraints.pop_back();
                
                if (constraints.empty())
                {
                  Temp.offset_min=-(max-min)/2.+Temp.rad();
                  Temp.offset_max=(max-min)/2.-Temp.rad();
                }
                else
                {
                   opt_sol bounds=solve_lp(Temp, constraints);
                   Temp.offset_min=min-bounds.min;
                   Temp.offset_max=max-bounds.max;
                }
                
                if (Temp.offset_min>Temp.offset_max)
                {
                    double temp=Temp.offset_min;
                    
                    Temp.offset_min=Temp.offset_max;
                    Temp.offset_max=temp;
                    
                }
                
                f->delete_tree();
                f=new AADDNode(Temp);
 
        }
        else // otherwise no join possible
        {
            return f;
        }
        }
    }
    else if (T==F)
    {
        f=T;
    }
    else
    {
       if (f->getT()!=T) f->setT(T);
       if (f->getF()!=F) f->setF(F);
    }
    
   return f;
}






