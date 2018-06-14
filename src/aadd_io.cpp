/**
 
 @file aadd_common.cpp
 
 @ingroup AADD
 
 @brief Method to print AADD to file and screen. 
 
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


#include <stdio.h>
#include <math.h>
#include <algorithm>    // std::set_union, std::sort
#include <vector>       // std::vector

#include "aadd.h"


/**
 
 @brief Private Method that prints AADD to file fp
 @details File has extension .dot that allows AADD to be visualised
 by the package Graphviz. Method is called by public method printaadd
 @author Carna Radojicic
 @return 0 if no error or 1 in case of error
 @see printaadd
 */
int DDBase::PrintToFile(FILE* fp, Node* node) const
{
    
    int retval;
    
    if (node!=NULL)
    {
        if ( node->isLeaf() )
        {
            VALUE_TYPE tmp=Value(node);
            
            if (tmp.getlength()==0)
            {
                retval=fprintf(fp, "\"%p\" [shape=box] [label = \"%g\"];\n",  (void *) node, tmp.getcenter());
                
                if (retval == EOF)
                {
                    cout << "END of FILE!" << endl;
                    return(1);
                }
            }
            else
            {
                retval=fprintf(fp,  "\"%p\" [shape=box] [label = \"%g", (void *) node, tmp.getcenter());
                
                if (retval == EOF)
                {
                    cout << "END of FILE!" << endl;
                    return(1);
                }
                
                for (unsigned i=0; i<tmp.getlength(); i++)
                {
                    
                    if (tmp[i+1]>=0)
                    {
                        retval=fprintf(fp, "+");
                        
                        if (retval == EOF)
                        {
                            cout << "END of FILE!" << endl;
                            return(1);
                        }
                    }
                    
                    retval=fprintf(fp, "%g*e%u", tmp[i+1], tmp.getIndexes()[i]);
                    
                    if (retval == EOF)
                    {
                        cout << "END of FILE!" << endl;
                        return(1);
                    }
                    
                    if (i==(tmp.getlength()-1))
                    {
                        if (tmp.offset_min!=0 or tmp.offset_max!=0)
                        {
                            retval=fprintf(fp, "+[%g, %g]", tmp.offset_min, tmp.offset_max);
                            
                        }
                        retval=fprintf(fp,"\"];\n");
                    }
                    
                    
                    if (retval == EOF)
                    {
                        cout << "END of FILE!" << endl;
                        return(1);
                    }
                    
                    
                }
            }
            
        }
        else // pointers
        {
            // if (! IsConstant(ThenChild(node)))
            if (! node->T->isLeaf() )
            {
                
                
                retval = fprintf(fp,
                                 "\"%p\" -> \"%p\" [style = solid];\n",
                                 (void *) node, (void *) ThenChild(node));
                
                if (retval == EOF)
                {
                    cout << "END of FILE!" << endl;
                    return(1);
                }
                
                retval=fprintf(fp, "\"%p\" [label= \"x%lu\"];\n", (void *) node, node->index);
                
                if (retval == EOF)
                {
                    cout << "END of FILE!" << endl;
                    return(1);
                }
                
                retval=fprintf(fp, "\"%p\" [label= \"x%lu\"];\n", (void *) ThenChild(node), ThenChild(node)->index);
                
                if (retval == EOF)
                {
                    cout << "END of FILE!" << endl;
                    return(1);
                }
                
                
                
            }
            else
            {
                retval = fprintf(fp,
                                 "\"%p\" -> \"%p\" [style = solid];\n",
                                 (void *) node, (void *) ThenChild(node));
                
                if (retval == EOF)
                {
                    cout << "END of FILE!" << endl;
                    return(1);
                }
                
                retval=fprintf(fp, "\"%p\" [label= \"x%lu\"];\n", (void *) node, node->index);
                
                if (retval == EOF)
                {
                    cout << "END of FILE!" << endl;
                    return(1);
                }
                
                
            }
            
            // if (!IsConstant(ElseChild(node)))
            if ( ! node->F->isLeaf() )
            {
                
                retval = fprintf(fp,
                                 "\"%p\" -> \"%p\" [style = dashed];\n",
                                 (void *) node, (void *) ElseChild(node));
                
                if (retval == EOF)
                {
                    cout << "END of FILE!" << endl;
                    return(1);
                }
                
                retval=fprintf(fp, "\"%p\" [label= \"x%lu\"];\n", (void *) node, node->index);
                
                if (retval == EOF)
                {
                    cout << "END of FILE!" << endl;
                    return(1);
                }
                
                retval=fprintf(fp, "\"%p\" [label= \"x%lu\"];\n", (void *) ElseChild(node), ElseChild(node)->index);
                
                if (retval == EOF)
                {
                    cout << "END of FILE!" << endl;
                    return(1);
                }
                
            }
            else
            {
                retval = fprintf(fp,
                                 "\"%p\" -> \"%p\" [style = dashed];\n",
                                 (void *) node, (void *) ElseChild(node));
                
                if (retval == EOF)
                {
                    cout << "END of FILE!" << endl;
                    return(1);
                }
                
                retval=fprintf(fp, "\"%p\" [label= \"x%lu\"];\n", (void *) node, node->index);
                
                if (retval == EOF)
                {
                    cout << "END of FILE!" << endl;
                    return(1);
                }
                
                
            }
            
            PrintToFile(fp, ThenChild(node));
            PrintToFile(fp, ElseChild(node));
            
            
        }
        
        
    }
    return 0;
}

/**
 @brief Public Method that prints AADD to the file format .dot <br>
 .dot can be opened by graph drawing package Graphviz, used to visualize AADD
 @details name of the file (without extension!) is specified via file_name
 @author Carna Radojicic
 @return 0 if no error or 1 in case of error
 */
int AADD::printaadd(string file_name) const
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
        return(1);
    }
    
    
    retval = fprintf(fp,
                     "size = \"20,20\"\ncenter = true;\nedge [dir = nONE];\n");
    if (retval == EOF)
    {
        cout << "END of FILE!" << endl;
        return(1);
    }
    
    retval=PrintToFile(fp, root);
    
    if (retval==0)
    {
        
        retval=fprintf(fp,"}\n");
        if (retval == EOF)
        {
            cout << "END of FILE!" << endl;
            return(1);
        }
    }
    
    fclose(fp);
    return retval;
}

/**
 @brief Private method that prints AADD to standard output stream (std:cout)
 @details Method is called by output stream operator <<
 @author Carna Radojicic
 @return stream output with AADD printed
 @see operator <<
 */
std::ostream& DDBase::PrintToConsole(std::ostream & s, Node *node) const
{
    if (node!=NULL)
    {
        
        if (node->isLeaf() )
        {
            VALUE_TYPE tmp=Value(node);
            
            
            if (tmp.getlength()==0)
            {
                
                s << tmp.getcenter() << endl;
                
            }
            else
            {
                
                s << tmp.getcenter();
                
                
                for (unsigned i=0; i<tmp.getlength(); i++)
                {
                    
                    if (tmp[i+1]>=0)
                    {
                        s << "+";
                    }
                    
                    s << tmp[i+1] << "*e" << tmp.getIndexes()[i];
                    
                    
                    if (i==(tmp.getlength()-1))
                    {
                        if (tmp.offset_min!=0 or tmp.offset_max!=0)
                        {
                            
                            s << "+[" << tmp.offset_min << ", " << tmp.offset_max << "]";
                            
                        }
                        s << endl;
                    }
                    
                    
                }
            }
            
        }
        else
        {
            
            VALUE_TYPE tmp=node->cond;
            
            if (tmp.getlength()==0)
            {
                
                s << "Condition: " << tmp.getcenter() << endl;
                
            }
            else
            {
                
                s << "Condition " << tmp.getcenter();
                
                for (unsigned i=0; i<tmp.getlength(); i++)
                {
                    
                    if (tmp[i+1]>=0)
                    {
                        s << "+";
                        
                        
                    }
                    
                    s << tmp[i+1] << "*e" << tmp.getIndexes()[i];
                    
                    if (i==(tmp.getlength()-1))
                    {
                        
                        if (tmp.offset_min!=0 or tmp.offset_max!=0)
                        {
                            
                            s << "+[" << tmp.offset_min << ", " << tmp.offset_max << "]";
                            
                        }
                        s << endl;
                        
                    }
                }
            }
            
            // if (! IsConstant(ThenChild(node)))
            if ( ! node->T->isLeaf() )
            {
                s << "x" << node->index << "---->" << "x" << node->T->index << endl;
                PrintToConsole(s, node->T);
            }
            else
            {
                
                s << "x" << node->index << "------->";
                
                
                VALUE_TYPE tmp=Value(node->T);
                
                if (tmp.getlength()==0)
                {
                    
                    s << tmp.getcenter() << endl;
                    
                    
                }
                else
                {
                    
                    s << tmp.getcenter();
                    
                    for (unsigned i=0; i<tmp.getlength(); i++)
                    {
                        if (tmp[i+1]>=0)
                        {
                            s << "+";
                        }
                        
                        s << tmp[i+1] << "*e" << tmp.getIndexes()[i];
                        
                        if (i==(tmp.getlength()-1))
                        {
                            
                            if (tmp.offset_min!=0 or tmp.offset_max!=0)
                            {
                                
                                s << "+[" << tmp.offset_min << ", " << tmp.offset_max << "]";
                                
                            }
                            
                            s << endl;
                        }
                    }
                }
                
            }
            
            // if (!IsConstant(ElseChild(node)))
            if (! node->F->isLeaf() )
            {
                s << "x" << node->index << "- - - ->" << "x" << node->F->index << endl;
                PrintToConsole(s, node->F);
            }
            else
            {
                s << "x" << node->index << "- - - ->";
                
                VALUE_TYPE tmp=Value(node->F);
                
                if (tmp.getlength()==0)
                {
                    s << tmp.getcenter() << endl;
                }
                else
                {
                    s << tmp.getcenter();
                    
                    for (unsigned i=0; i<tmp.getlength(); i++)
                    {
                        
                        if (tmp[i+1]>=0)
                        {
                            s << "+";
                            
                            
                        }
                        
                        s << tmp[i+1] << "*e" << tmp.getIndexes()[i];
                        
                        if (i==(tmp.getlength()-1))
                        {
                            
                            if (tmp.offset_min!=0 or tmp.offset_max!=0)
                            {
                                
                                s << "+[" << tmp.offset_min << ", " << tmp.offset_max << "]";
                                
                            }
                            
                            s << endl;
                        }
                    }
                }
            }
        }
    }
    
    return s;
}

/**
 
 @brief Output stream operator of AADD
 @author Carna Radojicic
 @return stream output with AADD printed
 
 */

std::ostream & operator << (std::ostream & s, const DDBase& f)
{
    unsigned leafs=f.numLeafs();
    
    if (leafs==1)
    {
        s << "AADD has 1 leaf:" << endl;
    }
    else
    {
        
        s << "AADD has " << leafs << " leafs:" << endl;
    }
    return f.PrintToConsole(s, f.root);
    
}
