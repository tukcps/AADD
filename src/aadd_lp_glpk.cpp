//
//  aadd_lp_glpk.cpp
//  AADD
//
//  Created by Christoph Grimm on 19.11.17.
//
//

/**
 
 @file aadd_common.cpp
 
 @ingroup AADD
 
 @brief Common functions and methods to manipulate AADD
 
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


#include <stdio.h>
#include <math.h>
#include <algorithm>    // std::set_union, std::sort
#include <vector>       // std::vector

#include "aadd.h"



/**
 @brief Private Method of AADD that finds total lower and upper bounds
 @details Method is called by GetMin(), GetMax() and GetBothBounds()
 @author Carna Radojicic
 @return optimal solution including lower and upper bounds
 @see GetMin(), GetMax() and GetBothBounds()
 */
opt_sol AADD::FindBounds(AADDNode* f, vector<constraint<AAF> > constraints) const
{
    opt_sol res;
    opt_sol resT, resE;
    
    constraint<AAF> cons;
    
    AADDNode *fv, *fvn;
    
    if (f->isLeaf() ) {
        
        opt_sol bounds=solve_lp(f->getValue(), constraints);
        
        res.min=bounds.min;
        res.max=bounds.max;
        return res;
    }
    
    /* Recursive step */
    
    fv=(f->getT());
    fvn=(f->getF());
    
    cons.con=f->getCond();
    cons.sign='+';
    
    constraints.push_back(cons);
    
    resT=FindBounds(fv,constraints);
    
    constraints.back().sign='-';
    
    resE=FindBounds(fvn,constraints);
    
    if (resT.min<resE.min)
    {
        res.min=resT.min;
    }
    else
    {
        res.min=resE.min;
    }
    
    
    if (resT.max>resE.max)
    {
        res.max=resT.max;
    }
    else
    {
        res.max=resE.max;
    }
    
    return res;
    
}


/**
 
 @brief Method that finds total lower bound
 
 @author Carna Radojicic
 
 @return double:lower bound found
 
 */
double AADD::GetMin() const
{
    vector<constraint<AAF> > constraints;
    opt_sol bounds;
    bounds = FindBounds(getRoot(), constraints);
    
#ifdef AADD_DEBUG
    cout << "Total lower bound is: " << bounds.min << endl;
#endif
    
    return bounds.min;
    
    
} // AADD::GetMin

/**
 @brief Method that finds total upper bound
 @author Carna Radojicic
 @return double:upper bound computed by GLPK
 */
double AADD::GetMax() const
{
    vector<constraint<AAF> > constraints;
    opt_sol bounds;
    bounds = FindBounds(getRoot(), constraints);
    
#ifdef AADD_DEBUG
    cout << "Total upper bound is: " << bounds.max << endl;
#endif
    return bounds.max;
} // AADD::GetMax

/**
 @brief Method that finds total lower and upper bounds
 @author Carna Radojicic
 @return optimal solution with lower & upper bounds found
 */
opt_sol AADD::GetBothBounds() const
{
    vector<constraint<AAF> > constraints;
    opt_sol bounds;
    bounds =  FindBounds(getRoot(), constraints);
    
#ifdef AADD_DEBUG
    cout << "Total lower bound is: " << bounds.min << endl;
    cout << "Total upper bound is: " << bounds.max << endl;
#endif
    
    return bounds;
} // AADD::GetBothBounds




/**
 
 @brief Function that calls LP solver
 
 @details Input: AAF whose bounds should be computed and constraints
 subject to which LP problem should be solved
 Function is called by relational operators and AADD methods
 GetBothBounds(), GetMin() & GetMax()
 
 @author Carna Radojicic
 
 @return optimal solution including total lower and upper bounds of AAF values
 of terminal vertices
 
 @see relational operators and GetBothBounds(), GetMin() & GetMax()
 
 */

opt_sol solve_lp(const AAF& var1, vector<constraint<AAF> > constraints)
{
    
    opt_sol res;
    
    unsigned l1, l2, ltemp;
    
    unsigned *id1, *id2;
    
    unsigned *pu1, *pu2, *idtemp;
    
    unsigned * fin;
    
    glp_prob *lp;
    
    int *ia, *ja;
    
    double *ar;
    
    AAF var2;
    
    //  Structure with parameters to set options.
    glp_smcp param;
    glp_init_smcp(&param);       // set options to defaults.
    
#ifdef AADD_DEBUG
    param.msg_lev = GLP_MSG_ERR; // error and warning messages
#else
    param.msg_lev = GLP_MSG_OFF; // no output
#endif
    
    
    if (var1.getlength() and constraints.size())
    {
        
        unsigned num_constraints=constraints.size();
        
        var2=constraints.back().con; // take last constraint
        
        l1=var1.getlength();
        l2=var2.getlength();
        
        id1=var1.getIndexes();
        id2=var2.getIndexes();
        
        pu1=id1;
        pu2=id2;
        
        
        idtemp=new unsigned[l1+l2];
        
        
        fin = std::set_union(id1,id1+l1,id2,id2+l2,idtemp);
        ltemp = fin-idtemp;
        
        ia=new int[(ltemp+1)*num_constraints+1];
        ja=new int[(ltemp+1)*num_constraints+1];
        ar=new double[(ltemp+1)*num_constraints+1];
        
        lp = glp_create_prob();
        glp_set_prob_name(lp, "LP_problem");
        
        glp_add_rows(lp, num_constraints);
        
        for (unsigned j=0; j<num_constraints; j++)
        {
            
            string name="constraint"+to_string(j+1);
            glp_set_row_name(lp,j+1,name.c_str());
            
            
            if (constraints[j].sign=='-')
            {
                
                glp_set_row_bnds(lp, j+1, GLP_UP, 0.0, 0.0); // constraint <= ths
            }
            else
            {
                
                glp_set_row_bnds(lp, j+1, GLP_LO, 0.0, 0.0); // constraint >= ths
            }
            
            AAF con=constraints[j].con;
            
            l2=con.getlength();
            id2=con.getIndexes();
            
            pu2=id2;
            
            
            for (unsigned k=0; k<ltemp+1; k++)
            {
                unsigned ind=j*(ltemp+1)+k+1;
                
                ia[ind]=j+1;
                ja[ind]=k+1;
                
                if (k==0)
                {
                    if (constraints[j].sign=='-')
                    {
                        ar[ind]=con.getcenter()+con.offset_min;
                        
                    }
                    else
                    {
                        
                        ar[ind]=con.getcenter()+con.offset_max;
                        
                    }
                    
                    
                }
                else
                {
                    unsigned b = pu2-id2;
                    
                    if (b < l2 and id2[b] == idtemp[k-1])
                    {
                        
                        ar[ind]=con[b+1];
                        
                        pu2++;
                        
                    }
                    else
                    {
                        ar[ind]=0.0;
                    }
                }
            }
        } // end for
        glp_add_cols(lp, ltemp+1);
        glp_set_col_name(lp,1,"nom_value");
        glp_set_col_bnds(lp, 1, GLP_FX, 1.0,1.0);
        
        // glp_set_obj_coef(lp,1,var1.getcenter()+var1.offset_max); // to ensure upper bound
        glp_set_obj_coef(lp,1,var1.getcenter());
        
        for (unsigned j=0; j<ltemp; ++j)
        {
            unsigned a = pu1-id1;
            
            string name="e"+to_string(idtemp[j]);
            glp_set_col_name(lp,j+2,name.c_str());
            glp_set_col_bnds(lp, j+2, GLP_DB, -1.0, 1.0);
            if (a<l1 and id1[a] == idtemp[j])
            {
                glp_set_obj_coef(lp,j+2,var1[a+1]);
                pu1++;
            }
        }
        
        glp_load_matrix(lp,(ltemp+1)*num_constraints,ia,ja,ar);
        
        glp_set_obj_dir(lp, GLP_MAX);
        
        glp_simplex(lp, &param);
        
        res.max=glp_get_obj_val(lp)+var1.offset_max;
        
        glp_set_obj_coef(lp,1,var1.getcenter()); // to ensure lower bound
        
        glp_set_obj_dir(lp, GLP_MIN);
        
        glp_simplex(lp, &param);
        
        res.min=glp_get_obj_val(lp)+var1.offset_min;
        
        glp_delete_prob(lp);
        
        delete [] ia;
        delete [] ja;
        delete [] ar;
    }
    else
    {
        res.min=var1.getMin();
        res.max=var1.getMax();
    }
    return res;
}

