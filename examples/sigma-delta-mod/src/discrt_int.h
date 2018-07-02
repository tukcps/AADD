//
//  discrt_int.h
//  
//
//  Created by Administrator on 7/30/13.
//
//

#ifndef _discrt_int_h
#define _discrt_int_h

#include <queue>
#include "systemc-ams.h"


using namespace std;

   // Generic integrator class
    template <class T>
    class discrt_int: public sca_tdf::sca_module
    {
    public:
        sca_tdf::sca_in<T> in;                // input port
        sca_tdf::sca_out<T> out;              // output port
        
        
    private:
        
        int delay_;
        
        T result, spec_value_;
        
        
        void initialize();
        void set_attributes();
        void processing();
        
    public:
        // The constructor of the module.
        discrt_int(sc_core::sc_module_name n, T spec_value=0.0, int delay=0);
        
        
     };
    
template <class T>
void discrt_int<T>::initialize()
{
    result=spec_value_;
        
    if (delay_)
      out.initialize(spec_value_);
        
        
        
}
    

    
    template <class T>
    void discrt_int<T>::set_attributes()
    {
        if (delay_>0)
            out.set_delay(delay_);
        
    }
    
    template <class T>
    void discrt_int<T>::processing()
    {
        result=result+in.read();
        
        out.write(result);
    }
    

    template <class T>
    discrt_int<T>::discrt_int(sc_module_name n, T spec_value, int delay)
    {
        delay_=delay;
        spec_value_=spec_value;
        
    }


#endif
