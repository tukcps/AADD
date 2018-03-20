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
// $Log: aa_exceptions.c,v $
//
// }}} 

#include "aa_exceptions.h"
#include <iostream>

using namespace std;

/************************************************************
 * Method:        
 * Author & Date: Darius Grabowski - 10/2006
 * Description:   
 *
 *   Input  : 
 *   Output : 
 ************************************************************/
AAF_Exception::AAF_Exception(const int errorCode_, const char *m, const double lValue_, const double hValue_):
  message(m),
  errorCode(errorCode_),
  lValue(lValue_),
  hValue(hValue_)
{
}

/************************************************************
 * Method:        
 * Author & Date: Darius Grabowski - 10/2006
 * Description:   
 *
 *   Input  : 
 *   Output : 
 ************************************************************/
AAF_Exception::AAF_Exception(const int errorCode_, const char *m, const double lValue_):
  message(m),
  errorCode(errorCode_),
  lValue(lValue_),
  hValue(0)
{
}

/************************************************************
 * Method:        
 * Author & Date: Darius Grabowski - 10/2006
 * Description:   
 *
 *   Input  : 
 *   Output : 
 ************************************************************/
AAF_Exception::AAF_Exception(const int errorCode_, const char *m):
  message(m),
  errorCode(errorCode_),
  lValue(0),
  hValue(0)
{
}

/************************************************************
 * Method:        
 * Author & Date: Darius Grabowski - 10/2006
 * Description:   
 *
 *   Input  : 
 *   Output : 
 ************************************************************/
AAF_Exception::~AAF_Exception() throw ()
{
}

/************************************************************
 * Method:        
 * Author & Date: Darius Grabowski - 10/2006
 * Description:   
 *
 *   Input  : 
 *   Output : 
 ************************************************************/
const char* AAF_Exception::what() const throw ()
{
  return (message.c_str());
}

/************************************************************
 * Method:        
 * Author & Date: Darius Grabowski - 10/2006
 * Description:   
 *
 *   Input  : 
 *   Output : 
 ************************************************************/
void AAF_Exception::report() const throw ()
{
  cout << "Errorcode: " << errorCode << " " << message 
       << " [" << lValue << "," << hValue << "]" << endl;
}
