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
 @brief Implements an assignment to the AADD.
 @details Handles assignment to an AADD and is used by all overaloaded assignment operators
 @author Christoph Grimm, Carna Zivkovic
 @return AADD that was assigned.
 */
/**
 @brief Assigment operator
 @details Assigns AADD of right to this AADD
 @author Carna Radojicic, Christoph Grimm
 @return AADD to be assigned
 */
AADD& AADD::handle_assignment(const AADD& right)
{
    // If in if-part of conditional statemen, or in while statement.
    if (scopes().inCond() && ((scopes().inIf()) or (scopes().inWhile()) ) )
    {
#ifdef DEBUG_IF
        cout << "Path condition:" << *scopes().conditions.back() << endl;
#endif
        // and now scopes().t gets value of right
        if ((this != &right) != false)
        {
            // collect conditions in if-part
            BDD cond=(*scopes().conditions.back()); // cond in if(cond) or while(cond)
            
            //  a while loop....
            if (scopes().inWhile())
            {
                AADD temp((*this));
                
                // first free memory of (*this)
                root->delete_tree();
                root = nullptr;
                root=new AADDNode(*((ITE(cond, right, temp)).getRoot()));
                
                return (*this);
            }
            
            // else if(cond) statement: executed for false values of previous conditions and cond
            // cond and !cond1 and !cond2 ....
            // for(auto c: scopes().conditions)
            for (unsigned i=0; i<scopes().conditions.size()-1; i++) // CARNA: -1???? Sure?????
            {
                cond = (cond and !(*scopes().conditions[i]));
                // cond = (cond and !(*c) );
            }
            
            AADD* Temp=new AADD(ITE(cond, right, (*this)));
            
            // push Temp (scopes.t)
            scopes().t.push_back(Temp);
            
            // first free memory of (*this)
            root->delete_tree();
            root = nullptr;
            
            root=new AADDNode(*Temp->getRoot());
            
        }
        else // assigned to itself
        {
            // in a while loop... there is no else part.
            if (scopes().inWhile())
            {
                return (*this);
            }
            
            // collect conditions in if-part
            BDD cond=(*scopes().conditions.back()); // cond in if(cond) or else if(cond)
            
            //  else if(cond) statement: executed for false values of previous conditions and cond
            // cond and !cond1 and !cond2 ....
            for (auto c: scopes().conditions)
            // for (unsigned i=0; i<scopes().conditions.size()-1; i++)
            {
                // cond = (cond and !(*scopes().conditions[i]));
                cond = (cond and !(*c) );
            }
            
            if (cond==true or cond==false)
            {
                return (*this);
            }
            
            // Now, (symbolic) condition is uncertain BDD ...
            // (*this) stays the same and (*this) is pushed in scopes().t
            
            AADD* Temp=new AADD(*this);
            
            scopes().t.push_back(Temp);
        }
        return (*this);
    }
    
    // for else part of conditional statement.
    if (scopes().inCond() && !scopes().inIf() )
    {
#ifdef DEBUG_IF
        cout<< "In else-part" << endl;
#endif
        
        // collect conditions from if parts
        BDD cond=*scopes().conditions.back();
        
        // else statement: executed for false values of all conditions
        // (*this) gets right for !cond1*!cond2*....
        // and scopes().t for cond1 or cond2 or ...
        
        // root=new AADDNode(*Temp.getRoot());
        
        for (auto c: scopes().conditions)
        // for (unsigned i=0; i<scopes().conditions.size()-1; i++)
        {
            // cond=cond or (*scopes().conditions[i]);
            cond = cond or *c;
        }
        
        bool found=false;
        
        AADD *t, *Temp;
        
        
        // finds assigment in if part in which (*this) appeared
        for (unsigned i=0; i<scopes().t.size(); i++)
        {
            if (((*this)==*scopes().t[i])==true)
            {
                t=scopes().t[i];
                found=true;
                break;
            }
        }
        
        if (found) // variable assigned also in if part
        {
            Temp=new AADD(ITE(cond, *t, right));
        }
        else // otherwise
        {
            Temp=new AADD(ITE(cond, (*this), right));
        }
        
        // free memory of (*this)
        root->delete_tree();
        root = nullptr;
        
        // now (*this) gets value of Temp
        if (Temp->root->isLeaf())
            root=new AADDNode(Temp->getRoot()->getValue());
        else
            root=new AADDNode(*Temp->getRoot());
        
        return (*this);
    }
    
    
    // if not in scope of conditional statement....
    
    // check if object (*this) is not assigned to itself
    // otherwise root stays the same ...
    if (this!=&right)
    {
        // first free memory of (*this)
        root->delete_tree();
        root = nullptr;
        
        /*  assign new value (right) to (*this)
         here we recursively create new pointers of root and its children
         this is to avoid sharing nodes between (*this) and right
         otherwise (*this) will point to nothing if
         a destructor for right is called */
        root=new AADDNode(*right.getRoot());
    }
    // otherwise root stays the same...
    return (*this);
}



/**
 @brief Assigment operator
 @details Assigns AADD of right to this AADD
 @author Carna Radojicic, Christoph Grimm
 @return AADD to be assigned
 */
AADD& AADD::operator=(const AADD& right)
{
    handle_assignment(right);
    return (*this);
}


/**
 @brief Assigment operator
 @details Assigns a real value to AADD
 @author Carna Radojicic, Christoph Grimm
 @return AADD to be assigned
 */
AADD& AADD::operator=(double right)
{
    // get condition as BDD
/*
    BDD cond = true;
    for (auto c: scopes().conditions)
    {
        cond = cond and *c;
    }
   
    ITE(cond, *new AADD(right), *this);

    return (*this);
 */
    AADD temp(right);
    handle_assignment(right);
    return (*this);
}


/**
 @brief Assigment operator
 @details Assigns an interval represented by AAF to AADD
 @author Carna Radojicic, Christoph Grimm
 @return AADD to be assigned
 */
AADD& AADD::operator=(const AAF& right)
{
/*
    // does at the moment only handle if statements. 
    // get condition as BDD
    BDD cond = true;
    for (auto c: scopes().conditions)
        cond = cond and *c;

    // Add condition.     
    ITE(cond, *new AADD(right), *this);

    return (*this);
 */
    AADD temp(right);
    handle_assignment(temp);
    return (*this);
}


/**
 @brief Builds an AADD from ifcase and thencase with condition as conditions.
 @details ITE function that takes two AADD as if and then arguments and has a BDD as a condition.
 @author Christoph Grimm, Carna Radojicic
 @return AADD that has additional levels.
 */
AADD& AADD::ITE(const BDD& c, const AADD& t, const AADD& f)
{
    if (c.getRoot()->isLeaf())
    {
        if ( c.getRoot()->getValue() == true )
        {
            AADD* result=new AADD(t);
            return *result;
        }
        else
        {
            AADD* result=new AADD(f);
            return *result;
        }
    }
    AADD Temp, Temp2;

    Temp.setRoot(Temp.BTimesA(c.getRoot(), t.getRoot() ) );
    BDD ic = !c;
   
    Temp2.setRoot(Temp.BTimesA(ic.getRoot(), f.getRoot()) );

    setRoot(Temp.ApplyBinOp(Plus, Temp.getRoot(), Temp2.getRoot()));

    return (*this);
}


AADD& ITE(const BDD& c, const AAF& t, const AAF& f)
{
    if (c.getRoot()->isLeaf())
    {
        if ( c.getRoot()->getValue() == true )
        {
            AADD* result=new AADD(t);
            return *result;
        }
        else
        {
            AADD* result=new AADD(f);
            return *result;
        }
    }
    
    AADD Temp, Temp2;
    AADD* result=new AADD;
    
    Temp.setRoot(Temp.BTimesA(c.getRoot(), new AADDNode(t) ) );
    BDD ic = !c;
    
    Temp2.setRoot(Temp.BTimesA(ic.getRoot(), new AADDNode(f)) );
    
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

