/**
 
 @file aadd_common.cpp
 
 @ingroup AADD
 
 @brief Common functions and methods to manipulate AADD
 
 @author Carna Radojicic, Christoph Grimm
 
 @copyright@parblock
 Copyright (c) 2017  Carna Radojicic, Christoph Grimm, Design of Cyber-Physical Systems
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


#include <stdio.h>
#include <math.h>
#include <algorithm>    // std::set_union, std::sort
#include <vector>       // std::vector

#include "aadd.h"


/**
 @brief Calls the copy constructor of the generic DDNode<bool> class.
 */
AADDNode::AADDNode(const AADDNode &source): 
	DDNode<AAF>(source)
{
	// nothing else; everything done by base class.
}

/**
 @brief Constructor af AADD Node
 @details Inserts a terminal vertex to AAADD
 @author Carna Radojicic
 @return terminal vertex inserted
 */
AADDNode::AADDNode(const AAF& val): 
	DDNode(MAXINDEX, nullptr, nullptr)
{
    this->value=val;
}

AADDNode::AADDNode(const unsigned long index, AADDNode*T, AADDNode*F): 
	DDNode(index, T, F)
{
    value = 0.0;
}

/**
 @brief Constructor creating AADD with one terminal vertex with value cst
 @details The value is a real number
 @author Carna Radojicic, Christoph Grimm
 @return AADD with a signle leaf node.
 */
AADD::AADD(double cst)
{
    root=new AADDNode(cst);
}


/**
 @brief Constructor creating AADD with one terminal vertex with affine form with range lb,ub
 @author Christoph Grimm
 @return AADD with a signle leaf node and range lb, ub. A new noise symbol is used. 
 */
AADD::AADD(double lb, double ub)
{
    AAF af(lb, ub);
    root=new AADDNode(af);
}



/**
 @brief Constructor creating AADD with one terminal vertex with value cst
 @details The value is an integer number
 @author Carna Radojicic, Christoph Grimm
 @return AADD with a single leaf node.
 */
AADD::AADD(int cst)
{
    root=new AADDNode(cst);
}


/**
 @brief Constructor creating AADD with one terminal vertex with value cst
 @details The value is an interval represented by AAF
 @author Carna Radojicic
 @return AADD
 */
AADD::AADD(const AAF& cst)
{
    root=new AADDNode(cst);
}


/**
 @brief Copy constructor
 @author Carna Radojicic, Christoph Grimm
 */
AADD::AADD(const AADD &from)
{
    if (from.root->isLeaf())
    {
        root=new AADDNode(from.root->getValue());
    }
    else // no leaf, recursion
    {
        root=new AADDNode(from.root->getIndex(),
                          new AADDNode(*from.getRoot()->getT()),
                          new AADDNode(*from.getRoot()->getF()));
    }
}


/**
 @brief Constructor creating AADD with the root equal to node and copy of rest.
 @author Carna Radojicic, Christoph Grimm
 */
AADD::AADD(const AADDNode& node)
{
    root=new AADDNode(node);
}


/**
 @brief AADD Destructor
 @author Carna Radojicic, Christoph Grimm
 */
AADD::~AADD()
{
    if (root!=nullptr) delete root;
    root=nullptr;
}


/**
 @brief Assigment method. Copies an AADD parameter while considering block conditions. 
 @details Assigns an AADD right to this AADD
 @author Carna Radojicic, Christoph Grimm
 @return AADD this with result. 
 */
AADD& AADD::assign(const AADD& right)
{
    if (bCond().inCond())  {   // in conditional stmt.
        ITE(bCond().blockCondition(), *new AADD(right), *this);
    } else {                    // not in any conditional statement.
        if ( this!=&right ) {
            // first free memory of (*this)
            root->delete_tree();
            root = new AADDNode(*right.getRoot());
        }
        // otherwise root stays the same...
    }
    return (*this);
}



/**
 @brief Assigment operator of C++ AADD <- AADD
 @details Assigns an AADD right to AADD and returns reference. 
          It also considers block conditions in doing this. 
 @author Carna Radojicic, Christoph Grimm
 @return AADD this with result.
 */
AADD& AADD::operator=(const AADD& right)
{
    return assign(right);
}



/**
 @brief Builds an AADD from ifcase and thencase with condition as conditions.
 @details ITE function that takes two AADD as if and then arguments and has a BDD as a condition.
 @details to mix both AADD into one, arithmetic operations are used that do the copying. 
 @author Christoph Grimm, Carna Radojicic
 @return AADD that has additional levels.
 */
AADD& AADD::ITE(const BDD& c, const AADD& t, const AADD& f)
{
    // if condition is true or false, we just need to return t or f
    if (c.getRoot()->isLeaf()) {
        if ( c.getRoot()->getValue() == true ) {
            setRoot(new AADDNode(*t.getRoot()));
            return *this;
        }
        else {
            setRoot(new AADDNode(*f.getRoot()));
            return *this;
        }
    }

    // otherwise, we need to create a new AADD.
    // specific form of Shannon expansion is here: aadd = c*t + ic*f. 
    AADDNode *Temp, *Temp2;
    Temp  = BTimesA(c.getRoot(), t.getRoot() );
    Temp2 = BTimesA((!c).getRoot(), f.getRoot());
    setRoot(ApplyBinOp(Plus, Temp, Temp2));

    return (*this);
}


AADD& ITE(const BDD& c, const AAF& t, const AAF& f)
{
    if (c.getRoot()->isLeaf()) {
        if ( c.getRoot()->getValue() == true ) {
            AADD* result=new AADD(t);
            return *result;
        }
        else {
            AADD* result=new AADD(f);
            return *result;
        }
    }
    
    AADD Temp, Temp2;
    AADD * result = new AADD; 
    
    Temp.setRoot(Temp.BTimesA(c.getRoot(), new AADDNode(t) ) );
    Temp2.setRoot(Temp.BTimesA((!c).getRoot(), new AADDNode(f)) );
    result->setRoot(Temp.ApplyBinOp(Plus, Temp.getRoot(), Temp2.getRoot()));
    
    return (*result);
}


template<>
void DDNode<AAF>::print(std::ostream &s) const
{
    getValue().print(s);
}


/**
 @brief Private method that prints AADD to standard output stream (std:cout)
 @details Method is called by output stream operator <<
 @author Carna Radojicic
 @return stream output with AADD printed
 @see operator <<
 */
void AADD::print(std::ostream & s) const
{
    cout << "AADD is ";
    getRoot()->print("", s);
}


std::ostream & operator << (std::ostream & s, const AADD& f)
{
    f.print(s);
    return s;
}



/**
 @brief Print method that recursively prints an AADD to an output stream s
        with indenting of the first parameter. The first parameter is increased
        recursively. 
 */
void AADDNode::print(const string& indent, std::ostream & s) const
{
    if ( isLeaf() )
    {
        s << "leaf with value "; DDNode<AAF>::print(s); s << endl;
    }
    else
    {   // node is internal node.
        s << indent << "internal node; cond: "; getCond().print(s); s<<", index: "<<index<<endl;
        s << indent << "---------";
        getT()->print(indent, s);
        s << indent << " - - - - ";
        getF()->print(indent, s);
        s << endl;
    }
}

