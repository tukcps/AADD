//
//  amp.h
//
//  (c) TU Kaiserslautern,
//  AG Design of Cyber-Physical Systems
//  Christoph Grimm, Carna Radojicic, Javier Moreno and Xiao Pan
//  Contains template class for modeling of an amplifier


#ifndef _AMP_H
#define _AMP_H

#include <queue>
#include <systemc-ams.h>


using namespace std;


    // Generic amplifier class
    template <class T>
    class amp: public sca_tdf::sca_module
    {
    public:
        sca_tdf::sca_in<T> in;               // input port
        sca_tdf::sca_out<T> out;             // output port
        
    private:
        T result, gain, offset, spec_value_;
        int delay_;
                
        
        void set_attributes();
        void processing();
        
    public:
        // The constructor of the module.
        amp(sc_core::sc_module_name n, T _gain=1.0, T _offset=0.0, int delay=0, T spec_value=0.0);
    };
    
    

    template <class T>
    void amp<T>::set_attributes()
    {
       if (delay_>0)
           out.set_delay(delay_);
        
         accept_attribute_changes();
        
    }


    template <class T>
    void amp<T>::processing()
    {
        result=gain*in.read() + offset;
        
       
        out.write(result);
        
    }

    template <class T>
    amp<T>::amp(sc_core::sc_module_name n, T _gain, T _offset, int delay, T spec_value)
    {gain=_gain; offset=_offset; delay_=delay; spec_value_=spec_value;}
    


#endif

