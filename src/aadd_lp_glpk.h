
/**
 
 @file aadd_lp_glpk.h
 
 @ingroup AADD
 
 @brief Access of Affine Arithmetic Desion Diagrams to GLPK for LP solving.
 
 @details Defines data structure of ROBDD and helper functions for GLPK acces etc.
 @details Everything here should not be visible to a user.
 
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

#ifndef aadd_lp_glpk_h
#define aadd_lp_glpk_h

#include <assert.h>
#include <iostream>
#include <fstream>

#include "aa.h"
#include "glpk.h"


/**
 @brief Type of constraints in internal vertices
 */
template<class TYPE>
class constraint
{
public:
    TYPE con;
    char sign;
};

/**
 @brief Type of optimal solution returned by LP solver
 @details Contains lower "min" and upper bounds "max" of leaf nodes' intervals
 */
struct opt_sol
{
    double max, min;
};

opt_sol solve_lp(const AAF&, vector<constraint<AAF> >);


#endif
