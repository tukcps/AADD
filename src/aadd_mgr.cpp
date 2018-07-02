/**
 
 @file aadd_mgr.cpp
 
 @ingroup AADD
 
 @brief Affine Arithmetic Desion Diagrams, manager of path and block conditions.
 
 @details The file defines the static class instance that manages conditions in AADD, BDD, and conditional statements and loops.
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


blockMgrC::blockMgrC()
{
    cout << "==============================================" << endl;
    cout << "  AADD lib -- Symbolic execution is enabled."   << endl;
    cout << "     AADD library (c) TU Kaiserslautern,"       << endl;
    cout << "          C. Zivkovic, C. Grimm."              << endl;
    cout << "============================================="  << endl
    << endl;
    startcputime = clock();
}


blockMgrC::~blockMgrC()
{
    double cpu_duration = (clock() - startcputime) / (double)CLOCKS_PER_SEC;
    cout << endl;
    cout << "AADD lib finished." << endl;
    cout << "CPU time used: " << cpu_duration << " sec."<< endl;
    // condMgr().printConditions();
}


void blockMgrC::printError(string error_message, const int line, string file_name) const
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


void blockMgrC::thenBlock(const BDD& c)
{
    in_if = true;
    conditions.push_back(new BDD(c) );
}


void blockMgrC::elseBlock(const unsigned line, const string& file_name)
{
    if (in_if)   // we negate last condition on stack
    {
        BDD * last = conditions.back();
        conditions.pop_back();
        conditions.push_back( & !(*last) );
    }
    else
    {
        printError("Syntax Error: No valid conditional statement", line, file_name);
    }
}


void blockMgrC::endBlock(const unsigned line, const string& file_name)
{
    if (!conditions.empty())
    {
        conditions.pop_back();
        
        // end of conditional statement
        if (conditions.empty()) in_if=false;
        
    }
    else
        printError("Syntax Error: endS too much");
}


void blockMgrC::whileBlock(const BDD& c)
{
    // to have only one condition on stack
    if (!conditions.empty())
    {
        conditions.pop_back();
    }
    
    // we need to make a copy of the temporary BDD c.
    conditions.push_back(new BDD(c) );
}

//@short static instance of blockMgrC - there is only this one instance.
static blockMgrC block_cond_manager;

blockMgrC& bCond()
{
    return block_cond_manager;
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
 Computes the overall block conditiona as a conjunction of all
 single conditions of nested conditional statements.
 */
const BDD& blockMgrC::blockCondition()
{
    BDD* cond = new BDD(true);
    for ( auto c: bCond().conditions)
        cond = new BDD(*cond and *c);
    return *cond;
}
