/**
 
 @file aadd_if.cpp
 
 @ingroup AADD
 
 @brief Affine Arithmetic Desion Diagrams, manager of conditional statements.
 
 @details The file defines the static class instance that manages conditional stmts and loops. 
 @details Furthermore, we add her other static stuff such error messages etc.
 
 @author Carna Radojicic, Christoph Grimm
 
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


AADDMgr::AADDMgr()
{
    cout << "==============================================" << endl;
    cout << "  AADD lib -- Symbolic execution is enabled."   << endl;
    cout << "     AADD library (c) TU Kaiserslautern,"       << endl;
    cout << "          C. Radojicic, C. Grimm."              << endl;
    cout << "============================================="  << endl
    << endl;
    startcputime = std::clock();
    in_if=in_while=false;
    saved_cond=nullptr;
}


AADDMgr::~AADDMgr()
{
    double cpu_duration = (std::clock() - startcputime) / (double)CLOCKS_PER_SEC;
    cout << endl;
    cout << "AADD lib finished." << endl;
    cout << "CPU time used: " << cpu_duration << " sec."<< endl;
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
    // we need to make a copy of the temporary BDD c.
    saved_cond = new BDD(c);
    conditions.push_back(saved_cond);
    in_if = true;
    assert ( inIf()  );
}


void AADDMgr::elseAADD(const int line, const char* file_name)
{
    if (in_if)
    {
       in_if = false;
    }
    else
    {
        printError("Syntax Error: No valid conditional statement", line, file_name);
    }
}


void AADDMgr::endifAADD()
{
   if (!conditions.empty())
   {
     conditions.pop_back();
   }
   
   if (conditions.empty()) // we came to the final end of control flow statement
   {                       // now scopes.t can be cleaned
     if (!scopes().t.empty())
     {
         scopes().t.clear();
     }
   }
}


void AADDMgr::whileAADD(const BDD& c)
{
    // to have only one condition on stack
    
    if (!conditions.empty())
    {
        conditions.pop_back();
    }
    
    // we need to make a copy of the temporary BDD c.
    saved_cond= new BDD(c);
    conditions.push_back(saved_cond);
    assert ( !conditions.empty() );
    in_while = true;
    assert ( inWhile()  );
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
