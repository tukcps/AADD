/**

 @file aadd_ddbase_impl.h

 @ingroup AADD

 @brief Implementation of the base class DDBase. Due to templates, as include file.

 @details Constructors, destructors, copy, types of leafs, etc.

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

#ifndef aadd_ddbase_impl_h
#define aadd_ddbase_impl_h

#include "aadd_ddbase.h"


/**
 @brief Copy constructor. 
 @author Carna Radojicic, Christoph Grimm
 */
template<class ValT>
DDNode<ValT>::DDNode(const DDNode<ValT> &from)
{
    if (from.isLeaf())
    {
        assert(from.isNotShared()); // If assertion fails, shared node e.g. ONE would be copied
        index = MAXINDEX;
        value = from.getValue();
        T = F = NULL;
    }
    else // no leaf, recursion
    {
        index = from.index;
        // if nodes such as ONE, ZERO can be shared, use them. 
        if (from.T->isShared()) T = from.T;
        else T = new DDNode(*from.T);
        if (from.F->isShared()) F = from.F;
        else F = new DDNode(*from.F);
    }
}


/**
 @brief Initializing constructur that does not copy any data structures.
 */
template<class LeafT>
DDNode<LeafT>::DDNode(unsigned long index, DDNode<LeafT>* T, DDNode<LeafT>* F)
{
    this->index = index;
    this->T = T;
    this->F = F;
}


/**
 @brief Returns true if a node is not a shared node.
 */
template <class Tvalue>
bool DDNode<Tvalue>::isNotShared() const
{
    return !isShared();
}


class BDDNode* ONE();
class BDDNode* ZERO();


template <class Tvalue>
bool DDNode<Tvalue>::isShared() const
{
    if (this == (void*)ONE()) return true;
    if (this == (void*)ZERO()) return true;

    return false;
};


template<class T>
bool DDNode<T>::isLeaf() const
{
    return (index == MAXINDEX);
};


template<class LeafT>
bool DDNode<LeafT>::isInternal() const
{
    return !(index == MAXINDEX);
};

template<class LeafT>
LeafT DDNode<LeafT>::getValue() const
{
    return value;
};

template<class ValT>
DDNode<ValT>* DDBase<ValT>::getRoot() const
{
    return root;
};


template<class ValT>
DDNode<ValT>* DDBase<ValT>::stealRoot()
{
    DDNode<ValT>* temp = root;
    root = NULL;
    return temp;
};


template<class ValT>
DDNode<ValT>* DDBase<ValT>::getCopy() const
{
    assert(false); // this does not copy ... so it is wrong. 
    return root;
};

/**
 @brief returns the number of leaf nodes in the AADD
 @author Christoph Grimm, Carna Radojicic
 @return number of leaf nodes
 @see NumberofLeaves
 */
template <class ValT>
unsigned DDBase<ValT>::numLeaves(const DDNode<ValT>& f) const
{
    if ( f.isLeaf() )
    {
        return 1;
    }
    else
    {
        unsigned count=0;
        count+=numLeaves( *f.getT());
        count+=numLeaves( *f.getF());
        return count;
    }
}


/**
 @brief Public Method that returns number of leaf nodes in the AADD
 for which the method is called
 @author Carna Radojicic
 @return number of leaf nodes returned
 */
template<class ValT>
unsigned DDBase<ValT>::numLeaves() const
{
    return numLeaves(*root);
}

/**
 @brief returns the number of intermediate nodes in the AADD
 @author Christoph Grimm, Carna Radojicic
 @return number of intermediate nodes
 */
template <class ValT>
unsigned DDBase<ValT>::numNodes(const DDNode<ValT>* source) const
{
    if (source->isLeaf())
    {
        return 0;
    }
    else
    {
        unsigned count=1;
        count+=numNodes(source->getT());
        count+=numNodes(source->getF());
        return count;
    }
    
}

/**
 @brief Public Method that returns number of intermediate nodes in the AADD
 for which the method is called
 @author Carna Radojicic
 @return number of intermediate nodes returned
 */
template <class ValT>
unsigned DDBase<ValT>::numNodes() const
{
    return numNodes(root);
}


/**
 @brief Public Method that prints AADD to the file format .dot <br>
 .dot can be opened by graph drawing package Graphviz, used to visualize AADD
 @details name of the file (without extension!) is specified via file_name
 @author Carna Radojicic
 @return 0 if no error or 1 in case of error
 */
template<class ValT>
void DDBase<ValT>::printGraphViz(const string file_name) const
{
    int retval;
    
    FILE* fp;
    string tmp=file_name+".dot";
    const char *name=tmp.c_str();
    
    fp=fopen(name,"w"); // open file for writing
    
    /* Write the header and the global attributes. */
    retval = fprintf(fp,"digraph \"AADD\" {\n");
    if (retval == EOF)
    {
        cout << "END of FILE!" << endl;
        return;
    }
    
    
    retval = fprintf(fp,"size = \"20,20\"\ncenter = true;\nedge [dir = nONE];\n");
    if (retval == EOF)
    {
        cout << "END of FILE!" << endl;
        return;
    }
    
    retval=printFile(fp, root);
    
    if (retval==0)
    {
        retval=fprintf(fp,"}\n");
        if (retval == EOF)
        {
            cout << "END of FILE!" << endl;
            return;
        }
    }
    fclose(fp);
}


/**
 @brief Output stream operator of AADD
 @author Carna Radojicic, Christoph Grimm
 @return stream output with AADD printed
 */
template<class ValT>
std::ostream & operator << (std::ostream & s, const DDBase<ValT>& f)
{
    unsigned leaves=f.numLeaves();
    
    if (leaves==1)
    {
        s << "AADD has has 1 leaf:" << endl;
    }
    else
    {
        s << "AADD has " << leaves << " leaves:" << endl;
    }
    
    return f.print(s, *f.root);
}


#endif /* aadd_ddbase_imple_h */
