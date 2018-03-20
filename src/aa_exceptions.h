/** 
 * Class: AAF_Exception
 * This class includes exceptions for operations on affine forms 
 *
 * @author Darius Grabowski <darius.grabowski@ims.uni-hannover.de>
 * @date 2009-04-01
 * @version 1
 *
 * @see http://www.ims.uni-hannover.de/
 *
 * (c) 2008-2009 
 * Institute of Microelectronic Systems (IMS)
 * Leibniz Universitaet Hannover 
 *
 * This program comes without any warranty; 
 * without even the implied warranty of
 * merchantability or fitness for a particular purpose.
 * Proprietary and confidential. 
 * Distribution only by express authority of the IMS.
 */

// {{{ Revisions
//
// $Log: aa_exceptions.h,v $
//
// }}} 

#ifndef AAF_EXCEPTIONS_H
#define AAF_EXCEPTIONS_H

#define AAF_ERROR_EXCEPTION     0
#define AAF_DIVZERO_EXCEPTION   1
#define AAF_NEGROOT_EXCEPTION   2
#define AAF_NEGLOG_EXCEPTION    3

#include <string>
#include <exception>

class AAF_Exception: public std::exception
{
public:
  std::string message;

  int errorCode;

  double lValue;

  double hValue;

public:

  AAF_Exception(const int, const char *, const double);

  AAF_Exception(const int, const char *, const double, const double);

  AAF_Exception(const int, const char *);

  ~AAF_Exception() throw();

  const char* what() const throw ();

  void report() const throw ();
};

#endif
