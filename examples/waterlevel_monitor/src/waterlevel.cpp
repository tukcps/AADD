#include "aadd.h"
#include "systemc-ams.h"

using namespace sca_tdf;

//
// Henzinger's water level monitor:
// - a tank with a pump.
// - if pump is off, water falls with rate of 2 in./time
//   if pump is on, water level rises with rate of 1 in./time
// - water level is initially 1, and must be between 1 and 12.
//   there are sensors that show if level < 5 or > 10.
//   the sensors have a delay of 2 seconds.
//



//
// The tank
//
SCA_TDF_MODULE(tank)
{
    sca_in<boolS>    pump; // rate is +1 if true, -2 if false.
    sca_out<boolS>   l5;   // true if level<5
    sca_out<boolS>   g10;  // true if level>10
    sca_out<doubleS> level;
    
    doubleS wlevel;  // must be between 1 in. and 12 in.
    doubleS uncertainty1;
    doubleS uncertainty2;
    
    void processing() {
        double T = pump.get_timestep().to_seconds();
        
        ifS(pump) 
           wlevel = wlevel+(1.+uncertainty1)*T;
        elseS 
           wlevel = wlevel+(-2.+uncertainty2)*T;
        endS;

        ifS(wlevel < 5.) l5=true;
        elseS l5 = false; endS;
        
        ifS(wlevel > 10) g10 = true;
        elseS g10 = false; endS;

        level.write(wlevel);
    }
    
    void initialize() {
        wlevel = 1.0;
        uncertainty2=doubleS(-0.1, 0.1); 
        uncertainty1=doubleS(-0.1, 0.1); 
    }
    
    void set_attributes() {
        set_timestep(sca_core::sca_time(0.1, sc_core::SC_SEC));
        l5.set_delay(20); // 20 time steps = 2 sec. delay. 
        g10.set_delay(20);
    }
    
    SCA_CTOR(tank) { }
};


//
// The controller
//
SCA_TDF_MODULE(controller)
{
    sca_in<boolS>  l5, g10;
    sca_out<boolS> pump;
    
    void processing() {
        ifS( l5 )  pump=true; endS;
        ifS( g10 ) pump=false; endS; 
     }
    
    void initialize() {
        pump.initialize(true);
    }
    
    
    controller(sc_module_name n){}
};


//
// Plotting the water level to a file plots.txt
//
SCA_TDF_MODULE(drain) 
{
    sca_tdf::sca_in<doubleS> in;
    ofstream plotfile;
    
    void initialize() {
        plotfile.open("plots.txt");
    }
    
    void processing() {
        double time=get_time().to_seconds();
        opt_sol input=in.read().GetBothBounds();
        plotfile << time << " " << input.min <<  " " << input.max << " " << 1 << " " << 12 << std::endl;
    }
    
    // Constructor: does nothing.
    drain(sc_module_name n){}
};


//
// Structure of tank and controller.
//
int sc_main(int argc, char* argv[])
{
    
    clock_t begin, end;
    
    begin=clock();
    
    sca_tdf::sca_signal<boolS> l5;
    sca_tdf::sca_signal<boolS> g10;
    sca_tdf::sca_signal<boolS> pump;
    sca_tdf::sca_signal<doubleS> level;
    
    tank wtank("wtank");
    wtank.pump(pump);
    wtank.level(level);
    wtank.l5(l5);
    wtank.g10(g10);
    
    controller wcontrol("wcontrol");
    wcontrol.l5(l5);
    wcontrol.g10(g10);
    wcontrol.pump(pump);
    
    drain drain_("drain_");
    drain_.in(level);
    
    sc_start(30, SC_SEC);
    
    end=clock();
    double time=(double)(end-begin)/CLOCKS_PER_SEC;
    
    return 0;
}
// No newline at end of file

