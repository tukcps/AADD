
//
//  sca_comlib_math.h
// 
//  (c) TU Kaiserslautern,  
//  AG Design of Cyber-Physical Systems
//  Christoph Grimm, Carna Radojicic, Javier Moreno
//
//  Contains declarations of ideal mathematical operations.
//
//  mul, divi, add, sub, logarithm, tan
//

#ifndef _ADDER_H
#define _ADDER_H

#include <systemc-ams.h>

// Generic adder class
template <class T>
class adder: public sca_tdf::sca_module
{
    public:
        sca_tdf::sca_in<T> in_1;                // input port 1
        sca_tdf::sca_in<T> in_2;                // input port 2
        sca_tdf::sca_out<T> out;                // result
        
    private:
        T result, spec_value_;
        int rate, delay_;
    
    
        void initialize();
        void set_attributes();
        void processing();
        
        
    public:
        // The constructor of the module.
    adder(sc_core::sc_module_name n, int datarate=1, T spec_value=0, int delay=0);
};
    

template <class T>
void adder<T>::set_attributes()
{
 
    in_1.set_rate(rate);
    in_2.set_rate(rate);
    out.set_rate(rate);
      

    if (delay_>0)
        out.set_delay(delay_);
    
     accept_attribute_changes();
        
}
 
    
template <class T>
void adder<T>::processing()
{
        for(int i=0;i<rate;i++)
        {
    
            out.write(in_1.read(i) + in_2.read(i),i );
    
        }
}
  
    
template <class T>
void adder<T>::initialize()
{
        
        
    if (delay_>0)
      out.initialize(spec_value_);
        
}
    


    
template <class T>
adder<T>::adder(sc_core::sc_module_name n, int datarate, T spec_value, int delay)
{
  rate=datarate;
  delay_=delay;
  spec_value_=spec_value;

}
 
#endif