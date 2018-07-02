This directory contains examples for symbolic simulation with SystemC. 

SystemC can be downloaded from www.accellera.org. 
It is a simulator for hardware/software systems that is implemented as an extension library for C++. 
Its extension to AMS systems (SystemC AMS) can be downloaded from https://www.coseda-tech.com/index.php.
It can easily be combined with libAADD. 

To do so follow these steps:

1. Install SystemC and SystemC AMS e.g. into /usr/local.

2. Set an environment variable SYSTEMC and SYSTEMCAMS to the installation directory

   > export SYSTEMC=PathToInstallationDirectory (e.g. /usr/local)
   > export SYSTEMCAMS=PathToInstallationDirectory (e.g. /usr/local)

3. Open README.md in AADD repository and follow the steps to install and set an environement variable AADDLIB. 

When all three steps are completed successfully you can compile and run examples.
Check README.md in the corresponding folders that explains how.

The following examples are included:

* waterlevel-monitor, the famous water level monitor. 
* sigma_delta_modulator, a 3rd order delta-sigma-modulator in SystemC AMS. 

