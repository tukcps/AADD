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

template<class T>
SC_MODULE(reference)
{
    
    sc_out<T> out;
    
    
    SC_HAS_PROCESS(reference);
    reference(sc_module_name);
    
    void processing();
    sc_clock clk1, clk2;
    doubleS x;
    doubleS res;
    
    
};

template<class T>
reference<T>::reference(sc_module_name n):clk1("clk1", sc_time(1, SC_MS), 0.5, SC_ZERO_TIME, true), clk2("clk2", sc_time(2, SC_MS), 0.5, SC_ZERO_TIME, true)
{
    
    SC_METHOD(processing);
    sensitive << clk1;
    dont_initialize();
    x=doubleS(1, 5);
    
};

template<class T>
void reference<T>::processing()
{

   /* ifS(x>2)
    {
        res=res+1;
    }
    endS
    
    cout << res << endl; */
    
    boolS cond=(x>2);
    
    out.write(cond);
    

};

template<class T>
SC_MODULE(test)
{
    sc_in<T> in;
    
    SC_HAS_PROCESS(test);
    test(sc_module_name)
    {
       SC_METHOD(processing);
       sensitive << in.pos();
       dont_initialize();
    }
    
    void processing();
    doubleS x;
    
    
};

template<class T>
void test<T>::processing()
{
    cout << sc_time_stamp() << endl;
    cout << in.read() << endl;
    
    x=x+1;

}




#endif
