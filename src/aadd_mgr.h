/**
 
 @file aadd_mgr.h
 
 @ingroup AADD
 
 @brief Affine Arithmetic Desion Diagrams manager of block and path conditions.
 @details The file defines the static class instance that manages conditional stmts and loops.
 @details We destinguish path conditions and block conditions.
 @details  Path conditions are all conditions between the program start and the current point of execution
 that cannot be evaluated to true or false at runtime.
 @details Block conditions are the conditions that are a condition to the current assignment statements.
 Block conditions stem from conditional or iteration statements.
 @details Furthermore, we implement here other static stuff such error messages etc.
 
 @author Carna Zivkovic, Christoph Grimm
 
 @copyright@parblock
 Copyright (c) 2017
 Carna Zivkovic, Christoph Grimm
 Design of Cyber-Physical Systems
 TU Kaiserslautern Postfach 3049
 67663 Kaiserslautern zivkovic@cs.uni-kl.de
 
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
    void printConditions();
    
    condMgrC();
}; // Not yet fully in use.

condMgrC& condMgr();


/**
 @brief The class blockMgr holds static data structures for AADD.
 @details This manager handles the block conditions.
 Block conditions are the conditions that are a condition to the current assignment statements.
 Block conditions stem from conditional or iteration statements.
 */
class blockMgrC
{
private:
    vector<BDD* > conditions;                  //@short holds a stack of block conditions.
    bool in_if;                                //@short true if in then-part of if statement.
    std::clock_t startcputime;
    
public:
    blockMgrC();
    ~blockMgrC();
    
    void thenBlock(const BDD& c);              //@short starts a conditional block, e.g. in if(c) ...
    void elseBlock(unsigned line, const string& filename); //@short adds else part.
    void endBlock(unsigned line, const string& filename); //@short pops a block condition.
    void whileBlock(const BDD& c);             //@short pushes condition of a while iteration.
    void endBlock();                           //@short ends a conditional block, e.g. end of if stmt. or while.
    
    inline bool inCond()                       { return !conditions.empty(); };
    const BDD& blockCondition();               //@short computes the current block condition as conjunction of all conditions on stack.
    
    void printError(string error_message,
                    const int line=0,
                    string file_name="") const;
};

blockMgrC& bCond();

#endif
