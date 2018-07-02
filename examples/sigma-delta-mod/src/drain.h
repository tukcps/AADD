//
//  drain.h
//  test
//
//  Created by Carna Radojicic on 3/13/14.
//
//

#include "systemc-ams.h"
#include <iostream>
#include <dirent.h>


template<class T>
SCA_TDF_MODULE(drain)
{
    sca_tdf::sca_in<T> in;
    
    ofstream results;
    
    drain(sc_module_name n)
    {
        results.open("plots.txt");
    }
    
    void processing()
    {
        double time=get_time().to_seconds();
        opt_sol min_max=in.read().GetBothBounds();
        results << time << " " << min_max.min << " " << min_max.max << endl;
    
    }
    
};



