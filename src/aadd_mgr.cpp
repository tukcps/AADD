/**
 
 @file aadd_mgr.cpp
 
 @ingroup AADD
 
 @brief Affine Arithmetic Desion Diagrams, manager of conditional statements.
 
 @details The file defines the static class instance that manages conditional stmts and loops. 
 @details Furthermore, we add her other static stuff such error messages etc.
 
 @author Carna Zivkovic (born Radojicic), Christoph Grimm
 
 @copyright@parblock
 Copyright (c) 2017
 Carna Radojicic, Christoph Grimm
 Design of Cyber-Physical Systems
 TU Kaiserslautern Postfach 3049
 67663 Kaiserslautern radojicic@cs.uni-kl.de
 
 This file is part of the AADD package.
 
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
#include "aadd_mgr.h"

AADDMgr::AADDMgr()
{
    cout << "==============================================" << endl;
    cout << "  AADD lib -- Symbolic execution is enabled."   << endl;
    cout << "     AADD library (c) TU Kaiserslautern,"       << endl;
    cout << "          C. Zivkovic, C. Grimm."              << endl;
    cout << "============================================="  << endl
    << endl;
    startcputime = clock();

}


AADDMgr::~AADDMgr()
{
    double cpu_duration = (clock() - startcputime) / (double)CLOCKS_PER_SEC;
    cout << endl;
    cout << "Symbolic simulation took: " << cpu_duration << " seconds."<< endl;
    // condMgr().printConditions(); 
}


void AADDMgr::printError(const char* error_message, const int line, const char* file_name) const
{
   if (line>0)
   {
       cerr << error_message << " in line " << line << " in file " << file_name << endl;
   }
   else
   {
       cerr << error_message << endl;
   }
    
   exit(1);
}


void AADDMgr::ifAADD(const BDD& c)
{
    conditions.push_back(new BDD(c) );
    in_if = true;
}


void AADDMgr::elseAADD(const int line, const char* file_name)
{
    if (in_if)   // we negate last condition on stack
    {
        in_if = false;
        BDD * last = conditions.back();
        conditions.pop_back();
        conditions.push_back( & !(*last) );
    }
    else
    {
        printError("Syntax Error: No valid conditional statement", line, file_name);
    }
}


void AADDMgr::endifAADD(const int line, const char* file_name)
{
   if (!conditions.empty())
   {
       conditions.pop_back();
   }
   else 
       printError("Syntax Error: endS too much");
    
}

void AADDMgr::whileAADD(const BDD& c)
{
    // to have only one condition on stack
    
    if (!conditions.empty())
    {
        conditions.pop_back();
    }
    
    // we need to make a copy of the temporary BDD c.
    conditions.push_back(new BDD(c) );
    in_while = true;
}

bool AADDMgr::inIf()
{
    return in_if;
}


bool AADDMgr::inWhile()
{
    return in_while;
}


//@short static instance of AADDMgr - there is only this one instance.
static AADDMgr aadd_manager;

AADDMgr& scopes()
{
    return aadd_manager;
}

//@short static instance of condMgr - there is only this one instance.
static condMgrC condition_manager;
condMgrC& condMgr()
{
    return condition_manager;
};

unsigned long condMgrC::addCond(const AAF& c)
{
    AAF *condition = new AAF(c);
    path_conditions.push_back(condition);
    last_index++; 
    return last_index-1; 
};

AAF& condMgrC::getCond(unsigned long index) const
{
    AAF* c = path_conditions[index];
    assert(c!=nullptr);
    return *c;
};

condMgrC::condMgrC()
{
   last_index=0;
};


void condMgrC::printConditions()
{
    cout << "Conditions: " << last_index << endl; 
    for (int i=0; i < last_index; i++)
    {
       cout << "i: " << i << ": " << *path_conditions[i] << endl; 
    }
};

/*
 Computes the overall block conditions as a conjunction of all
 single conditions of nested conditional statements.
 */
const BDD& AADDMgr::blockCondition()
{
    BDD* cond = new BDD(true);
    if (inCond())
        for ( auto c: scopes().conditions)
            cond = new BDD(*cond and *c);
    return *cond;
}

