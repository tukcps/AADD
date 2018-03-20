/**

 @file aadd_disabled.h

 @ingroup AADD

 @brief Include file that disables all symbolic simulation features.

 @details To make a numeric execution or simulatin run, just include this file. 
 @details It replaces all types and function with numeric types from C++ standard. 

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


/*
 Only include this file if you don't want to do a symbolic simulation ! 
 */ 

#ifndef aadd_disabled_h
#define aadd_disabled_h

typedef double doubleS;
typedef float  floatS;
typedef float  intAS;
typedef double realS;
typedef bool   boolS;

#define ifS if
#define elseS else
#define endS
#define whileS while

#endif
