//
//  onebitquant.h
//  
//
//  Created by Radojicic Carna on 11/26/13.
//
//

#ifndef __bitquant_h
#define __bitquant_h


template <class T>
class quant: public sca_tdf::sca_module
{
public:
    sca_tdf::sca_in<T> in;
    sca_tdf::sca_out<T> out;
    
    quant(sc_module_name, T high_vol_, T low_vol_, double th_vol_, T init_value_=0.0, int delay_=0)
    {
        th_vol=th_vol_;
        high_vol=high_vol_;
        low_vol=low_vol_;
        init_value=init_value_;
        delay=delay_;
    }
    
private:
    double th_vol;
    
    T high_vol, low_vol;
    
    T init_value, y, v;
    
    int delay;
    
    void set_attributes()
    {
        if (delay>0)
        {
            out.set_delay(delay);
            
            
        }
        
    }
    
    void initialize()
    {
        if (delay>0)
        {
            out.initialize(init_value);
            
        }
        
    }
    
    void processing()
    {
        y=in.read();
        
        ifS(y>=th_vol)
          v=high_vol;
        elseS
          v=low_vol;
        endS
        
        out.write(v);
        
        
    }

    

};
/*

    
    void set_attributes()
    {
        if (delay>0)
        {
            out.set_delay(delay);
            
            
        }
        
    }
    
    
    void initialize()
    {
        if (delay>0)
        {
            out.initialize(init_value);
            
        }
        
    }
    
    void processing()
    {
        y=in.read();
        
        ifS(y>=th_vol)
           v=high_vol;
        elseS
          v=low_vol;
        
        out.write(v);
        
        
    }

};


*/

#endif
