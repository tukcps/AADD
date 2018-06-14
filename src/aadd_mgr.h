/**
 
 @file aadd_if.h
 
 @ingroup AADD
 
 @brief Affine Arithmetic Desion Diagrams, manager of conditions and control-flow statements.
 @details The file defines the static class instance that manages conditional stmts and loops.
 @details We destinguish paht conditions and block conditions. 
 @details  Path conditions are all conditions between the program start and the current point of execution
 that cannot be evaluated to true or false at runtime.
 @details Block conditions are the conditions that are a condition to the current assignment statements.
 Block conditions stem from conditional or iteration statements.
 @details Furthermore, we implement here other static stuff such error messages etc.
 
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


#ifndef aadd_if_h
#define aadd_if_h

#include <vector>
#include <time.h>

using namespace std;

class AAF;
class BDD;
class AADD;

/**
 @brief The class condMgr manages the path conditions in a program run.
 @detail Path conditions are all conditions between the program start and the current point of execution
 that cannot be evaluated to true or false at runtime. 
 Block conditions are the conditions that are a condition to the current assignment statements.
 Block conditions stem from conditional or iteration statements.
 */
class condMgrC
{
private:
    vector<AAF*> path_conditions; // Vector of path conditions. Index is the index of AADD/BDD.
    unsigned long last_index; 

public:
    unsigned long addCond(const AAF&c);        // returns index of new condition
    AAF& getCond(unsigned long index) const;
    
private:
    vector<BDD*> block_conditions;

public:
    void printConditions();
   
    condMgrC();  
}; // Not yet fully in use.

condMgrC& condMgr();

/**
 @brief The class BlockMgr holds static data structures for AADD.
 @details This manager handles the block conditions. 
 Block conditions are the conditions that are a condition to the current assignment statements.
 Block conditions stem from conditional or iteration statements.
 */
class blockMgrC
{
    vector<BDD* > block_conds; 
    void thenBlock(BDD* c, unsigned line=0);   //@short starts a conditional block, e.g. in if(c) ...
    void elseBlock(unsigned line=0);           //@short adds else part.
    void whileBlock(BDD* c, unsigned line=0);  //@short condition of a while iteration.
    void endBlock();                           //@short ends a conditional block, e.g. end of if stmt. or while.
};


/**
 @brief The class AADDMgr holds static data structures for AADD. 
 @detail This includes first of all a vector of conditions for conditional and iteration statements. 
     This is basically the currently active path conditions. 
     A condition is removed once a conditional stmt or iteration statement is left.
 
     Furthermore, we handle central user messages via this class.
 */
class AADDMgr
{
public:
    
    vector<BDD* >  conditions; // In AADD, the block condition is a conjunction of the BDD of this vector
    
    void ifAADD(const BDD &c);  // pushes pointer to cond c on the stack.
                                // We now save cond and t parts if any.
                                // … in overloaded assignment operators.
    bool inCond()               { return !conditions.empty(); };
    const BDD& blockCondition();
    bool inIf();
    bool inWhile();
    void elseAADD(const int line, const char* file_name);     
    void endifAADD(const int line, const char* file_name);
    void endActivation();
    
    void whileAADD(const BDD& c); // pushes pointer to condition c on the stack.
                             // loop is repeated until c is false.
    
    void printError(const char* error_message, const int line=0, const char* file_name="") const;

    
    AADDMgr();
    ~AADDMgr();
    
private:
    bool in_if;
    bool in_while;
    clock_t startcputime;
   
};

AADDMgr& scopes();

#endif
