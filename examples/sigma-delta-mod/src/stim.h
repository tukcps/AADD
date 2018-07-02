//
//  stim.h

//
//  Created by Administrator on 7/31/13.
//
//

#ifndef stim_h
#define stim_h

#include <iostream>
#include <fstream>

#include "systemc-ams.h"

double randn(double min=0., double max=1.) {
    
    srand((unsigned)time(NULL));
    double ampl = max - min ;
   
    double rnd=ampl * ((double)rand())/((double)RAND_MAX)+min;        // calculate random numbers;
    return rnd;                                                      // output rnd
	
}

template <class T>
SCA_TDF_MODULE(stim)
{
public:
    sca_tdf::sca_out<T> out;
    
    T ampl;
    double Ts;
    
    stim(sc_core::sc_module_name nm, T ampl_, double Ts_)
    {
        Ts=Ts_;
        ampl=ampl_;
    }
    
    void set_attributes()
    {
        set_timestep(Ts, SC_SEC);
    }

    void processing()
    {
      out.write(ampl);
        
    }

    
};


#endif
