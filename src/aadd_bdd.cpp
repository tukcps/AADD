/**
 
 @file aadd_bdd.cpp
 @ingroup AADD
 @brief BDD Part of of Affine Arithmetic Desion Diagrams, implementation.
 @details The header file defines data structure of ROBDD and helper functions for GLPK acces etc.
 @details Everything here should not be visible to a user.
 
 @author Carna Radojicic, Christoph Grimm

 @copyright@parblock
 Copyright (c) 2017  
 Carna Radojicic, Christoph Grimm, 
 Design of Cyber-Physical Systems
 TU Kaiserslautern Postfach 3049 
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

#include "aadd_bdd.h"

/************************************************************
 * Description:
 *   Function that creates terminal vertex v with value(v)=1
 ************************************************************/
BDDNode* ONE(void)
{
    static BDDNode* one=nullptr;
    
    if (one==nullptr)
    {
        one=new BDDNode(MAXINDEX, nullptr, nullptr);
        one->setValue(true);
    }
    
    return one;
}

/**
 @brief  Function that creates terminal vertex v with value(v)=0
 */
BDDNode* ZERO(void)
{
    static BDDNode* zero=nullptr;
    if (zero==nullptr)
    {
        zero=new BDDNode(MAXINDEX, nullptr, nullptr);
        zero->setValue(false);
    }
    return zero;
}


/**
 @brief Constructor that creates an internal node of a BDD.
 @details replaces deprecated insert method.
 @author Carna Radojicic, Christoph Grimm
 @return new vertex that is internal with two childs T,F,index. 
 */
BDDNode::BDDNode(const unsigned long index,
                 BDDNode* T,
                 BDDNode* F,
                 bool value): DDNode<bool>(index,T,F)
{
    if (T!=nullptr and T->isShared()) this->T=T;
    if (F!=nullptr and F->isShared()) this->F=F;
    this->value = value;
}


/** 
 @brief Calls the copy constructor of the generic DDNode<bool> class.
 */
BDDNode::BDDNode(const BDDNode &source): DDNode<bool>(source)
{
    // everyghing done in base class. 
}


/**
 @brief BDD Destructor
 @author Carna Radojicic, Christoph Grimm
 */
BDD::~BDD()
{
    if ( (root!=nullptr) and ( !root->isLeaf()) )
    {
        delete root;
    }
}


/**
 @brief Copy constructor
 @author Carna Radojicic, Christoph Grimm
 */
BDD::BDD(const BDD &from)
{
    if (from.root->isLeaf())
    {
        // we have in BDD only two leaves, ONE and ZERO.
        root=from.root;
    }
    else // recursion
    {
        root=new BDDNode(*from.getRoot());
    }
    assert(root != nullptr);
}


/**
 @brief Constructor creating a BDD with one terminal vertex with value cst
 @details The value is a Boolean number
 @author Carna Radojicic
 @return AADD with a single leaf node.
 */
BDD::BDD(bool cst)
{
    if (cst==true)
    {
        root=ONE();
    }
    else
    {
        root=ZERO();
    }
    assert(root != nullptr);
}


/**
 @brief Constructor creating AADD with the root equal to node and copy of the rest.
        It creates a coppy of the BDD tree.
 @author Carna Radojicic, Christoph Grimm
 */
BDD::BDD(BDDNode* from)
{
    if (from->isLeaf())
        root = from;
    else
        root=new BDDNode(*from);
    assert(root != nullptr);
}


/**
 @brief Assigment operator
 @details Assigns AADD to AADD
 @author Carna Radojicic, Christoph Grimm
 @return AADD to be assigned
 */
BDD& BDD::operator=(const BDD& right)
{
    // check if in scope of conditional statement
    if (scopes().inCond() && ((scopes().inIf()) or (scopes().inWhile()) ) )
    {
#ifdef DEBUG_IF
        cout << "In if-part, condition is:" << *scopes().conditions.back() << endl;
#endif
        // and now scopes().tbdd gets value of right
        if (this!=&right)
        {
            // collect conditions in if-part
            
            BDD cond=(*scopes().conditions.back()); // cond in if(cond) or while(cond)
            
            //  a while loop....
            
            if (scopes().inWhile())
            {
                BDD temp((*this));

                // first free memory of (*this), then create new node. 
                root->delete_tree();
                root = nullptr;
                root=new BDDNode(*((ITE(cond, right, temp)).getRoot()));
                
                return (*this);
            }
            
            //  else if(cond) statement: executed for false values of previous conditions and cond
            // cond and !cond1 and !cond2 ....
            for (unsigned i=0; i<scopes().conditions.size()-1; i++)
            {
                cond = (cond and !(*scopes().conditions[i]));
            }
            
            BDD* Temp=new BDD(ITE(cond, right, (*this)));
            
        
            // push Temp (scopes.t)
            scopes().tbdd.push_back(Temp);
            
            // first free memory of (*this)
            root->delete_tree();
            root = nullptr;

            root=new BDDNode(*Temp->getRoot());
            
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
            for (unsigned i=0; i<scopes().conditions.size()-1; i++)
            {
                cond = (cond and !(*scopes().conditions[i]));
            }
            
            if (cond==true or cond==false)
            {
                return (*this);
            }
            
            // Now, (symbolic) condition is uncertain BDD ...
            // (*this) stays the same and (*this) is pushed in scopes().tbdd
            
            BDD* Temp=new BDD(*this);
            
            scopes().tbdd.push_back(Temp);
        }
        
        return (*this);
    }
    
    
    if (scopes().inCond() && !scopes().inIf() )
    {
#ifdef DEBUG_IF
        cout<< "In else-part" << endl;
#endif
        
        // collect conditions from if parts
        
        BDD cond=*scopes().conditions.back();
        
        // else statement: executed for false values of all conditions
        // (*this) gets right for !cond1*!cond2*....
        // and scopes().tbdd for cond1 or cond2 or ...
        
        // root=new BDDNode(*Temp.getRoot());
        
        for (unsigned i=0; i<scopes().conditions.size()-1; i++)
        {
            cond=cond or (*scopes().conditions[i]);
        }
        
        bool found=false;
        
        BDD *t, *Temp;
        
        
        // finds assigment in if part in which (*this) appeared
        
        for (unsigned i=0; i<scopes().tbdd.size(); i++)
        {
            if (((*this)==*scopes().tbdd[i])==true)
            {
                t=scopes().tbdd[i];
                found=true;
                break;
            }
        }
        
        if (found) // variable assigned also in if part
        {
            Temp=new BDD(ITE(cond, *t, right));
        }
        else // otherwise
        {
            Temp=new BDD(ITE(cond, (*this), right));
        }
        
        // remove root
        root->delete_tree();
        root = nullptr;
        
        // now (*this) gets value of Temp
        if (Temp->root->isLeaf())
            // root=new BDDNode(Temp->getRoot()->getValue());
            if (Temp->root->getValue() == true) root=ONE(); 
            else root=ZERO(); 
        else
            root=new BDDNode(*Temp->getRoot());
       
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
        root=new BDDNode(*right.getRoot());
    }
    // otherwise root stays the same...
    return (*this);
}

// to allow use of BDD in conditional statements
// this further allows operator == to work in existing simulators (e.g. SystemC)

BDD::operator bool() const
{
    if ((*this)==true)
    {
        return true;
    }
    
    return false;
}


/**
 @brief Builds an BDD from ifcase and thencase with condition as conditions.
 @details ITE function that takes two BDD as if and then arguments and has a BDD as a condition.
 @author Christoph Grimm
 @return BDD that has additional levels from merging the two BDD+new one.
 */
BDD& ITE(const BDD& c, const BDD& t, const BDD& e)
{
    if (c.getRoot()->isLeaf() && (c.getRoot()->getValue() == true))
    {
        BDD* result=new BDD(t);
        return (*result);
        
    }
    else if (c.getRoot()->isLeaf() && (c.getRoot()->getValue() == false))
    {
        BDD* result=new BDD(e);
        return (*result);
    }
        
    BDD Temp, Temp2;
    BDD* result=new BDD;
    Temp.setRoot( Temp.ApplyBinOp(And, c.getRoot(), t.getRoot() ) );
    BDD ic = !c;
    Temp2.setRoot(Temp.ApplyBinOp(And, ic.getRoot(), e.getRoot()) );
    result->setRoot(Temp.ApplyBinOp(Or, Temp.getRoot(), Temp2.getRoot()));
    return (*result);
}


template<>
void DDNode<bool>::print(std::ostream &s) const
{
    if (getValue() == true) s << "true";
    else s << "false";
}


/**
 @brief Private method that prints AADD to standard output stream (std:cout)
 @details Method is called by output stream operator <<
 @author Carna Radojicic
 @return stream output with AADD printed
 @see operator <<
 */
void BDD::print(std::ostream & s) const
{
    cout << "BDD is ";
    getRoot()->print("", s); 
}

void BDDNode::print(const string& indent, std::ostream & s) const
{
    if ( isLeaf() )
    {
        s << "leaf with value: "; DDNode<bool>::print(s); s << endl;
    }
    else
    {   // node is internal node.
        s << indent << "internal node; cond: "; getCond().print(s); s<<", index: "<<index<<endl;
        s << indent << "--------->";
        getT()->print(indent + "        ", s);
        s << indent << "- - - - ->";
        getF()->print(indent + "        ", s);
        s << endl;
    }
}



/**
 @brief Output stream operator of BDD
 @author Carna Radojicic, Christoph Grimm
 @return stream output with AADD printed
 */
std::ostream & operator << (std::ostream & s, const BDD& f)
{
    f.print(s);
    return s; 
}



