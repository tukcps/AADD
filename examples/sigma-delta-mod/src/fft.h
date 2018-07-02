//
//  fft.h

//
//  Created by Radojicic Carna on 7/31/13.
//
//


#include <iostream>
#include <fstream>

#include "systemc-ams.h"

#include "../xaaf/xaaf.h"

struct Complex
{
    AAF real;
    AAF imag;
};

template <int N>
SCA_TDF_MODULE(FFT) // calculates Fourier Transform of XAAF signal
{
public:
    sca_tdf::sca_in<XAAF> in;
    sca_tdf::sca_out<XAAF* > out_real;
    sca_tdf::sca_out<XAAF* > out_imag;

    sca_tdf::sca_out<XAAF* > ampl; // for sensitivity analysis
    
    
    unsigned i, n;
    
 private:
    vector<Complex> calc_fft(AAF);
    ofstream data;
    
    void initialize()
    {
        i=1;
        n=0;
        data.open("data.txt");
    }
    
    void set_attributes()
    {
        accept_attribute_changes();
        
    }
    
    void processing()
    {
        XAAF input=in.read();
        
        unsigned length=input.getlength();
        
        AAF mean=input.getmean();
        
        AAF* deviations;
        
        if (i<N)
        {
            if (length==0)
            {
                calc_fft(mean);
                
            }
            else
            {
                calc_fft(mean);
                for (unsigned i=0; i<length; i++)
                {
                    calc_fft(input[i+1]);
                }
            }
            
        }
        else
        {
            XAAF* real, *imag;
            
            XAAF* amplitude;
            
           if (length==0)
           {
               vector<Complex> fft_res;
               
               fft_res=calc_fft(mean);
               
               real=new XAAF[fft_res.size()];
               imag=new XAAF[fft_res.size()];
               
               amplitude=new XAAF[fft_res.size()];
               
               for (unsigned i=0; i<fft_res.size(); i++)
               {
                   real[i]=fft_res[i].real;
                   imag[i]=fft_res[i].imag;
                   
                   XAAF temp=magnitude(real[i],imag[i]);
                   
                   if (i>0)
                   {
                       temp*=2;
                       temp/=N;
                   }
                   else
                   {
                       temp/=N;
                   }
                   
                   amplitude[i]=temp;
                   
                   data << i << " " << temp.getMin() << " " << temp.getMax() << std::endl;
                
                   
               }
               
               out_imag.write(imag);
               out_real.write(real);
               ampl.write(amplitude);
               
               delete [] real;
               delete [] imag;
               
               delete [] amplitude;
               
               

           }
           else
           {
               vector<Complex> m_value;
               
               vector<Complex>* dev=new vector<Complex>[length];
               
               unsigned *indexes=new unsigned[length];
               AAF* dev_real=new AAF[length];
               AAF* dev_imag=new AAF[length];
               
               real=new XAAF[N];
               imag=new XAAF[N];
               amplitude=new XAAF[N];
            
               m_value=calc_fft(mean);
               for (unsigned i=0; i<length; i++)
               {
                   indexes[i]=input.getIndexes()[i];
                   dev[i]=calc_fft(input[i+1]);
                   
                   
               }
               
               for (unsigned k=0; k<N; k++)
               {
                   
                   for (unsigned i=0; i<length; i++)
                   {
                       dev_real[i]=dev[i][k].real;
                       dev_imag[i]=dev[i][k].imag;
            
                   }
                   
                   
                   XAAF pom(m_value[k].real,dev_real,indexes,length);
                   
                   real[k]=pom;
                   
                   pom=XAAF(m_value[k].imag,dev_imag,indexes,length);
                   
                   imag[k]=pom;
                   
                   pom=magnitude(real[k],imag[k]);
                   
                   if (i>0)
                   {
                       pom*=2;
                       pom/=N;
                   }
                   else
                   {
                       pom/=N;
                   }
                   
                   amplitude[k]=pom;
                   
                    data << k << " " << pom.getMin() << " " << pom.getMax() << std::endl;
                   
               } // end for
               
               delete [] indexes;
               delete [] dev_real;
               delete [] dev_imag;

    
               out_real.write(real);
               out_imag.write(imag);
               ampl.write(amplitude);
               
               delete [] real;
               delete [] imag;
               
               delete [] amplitude;
               
           }
            
            
            i=0;
            n=-1;
            
            
        }
        
    
        ++i;
        ++n;
        
    }
    
public:
    FFT(sc_module_name n){}

    
};

template <int N>
vector<Complex> FFT<N>::calc_fft(AAF value)
{
    vector<Complex> res;
    static vector<unsigned> ind;
    
    static unsigned i=1;
    static unsigned n=0;
    
    static double fourier_sum_nom_real[N];
    static double fourier_sum_nom_imag[N];
    
    static vector<double> fourier_sum_dev_real[N];
    static vector<double> fourier_sum_dev_imag[N];
    
    if (ind.size())
        ind.clear();

    
    for (unsigned j=0; j<value.getlength(); j++)
    {
        unsigned in_index=value.getIndexes()[j];
        ind.push_back(in_index);
    
        for (unsigned p=fourier_sum_dev_real[0].size(); p<in_index; p++)
        {

            fourier_sum_dev_real[0].push_back(0.0);
            fourier_sum_dev_imag[0].push_back(0.0);
        }
        
    }
    
    unsigned length=fourier_sum_dev_real[0].size();
    
    double nom=value.getcenter();
    
    for (unsigned k=0; k<N; k++)
    {
        
        if (k>0)
        {
            for (int p=fourier_sum_dev_real[k].size(); p<length; p++)
            {
                fourier_sum_dev_real[k].push_back(0.0);
                fourier_sum_dev_imag[k].push_back(0.0);
            }
            
        }
        
        
        fourier_sum_nom_real[k]=fourier_sum_nom_real[k]+(nom*cos(2*M_PI*k*n/N));
        fourier_sum_nom_imag[k]=fourier_sum_nom_imag[k]+(nom*sin(-2*M_PI*k*n/N));
        
       
        
        for (unsigned j=0; j<ind.size(); j++)
        {
                
            fourier_sum_dev_real[k][ind[j]-1]=fourier_sum_dev_real[k][ind[j]-1]+(value[j+1]*cos(2*M_PI*k*n/N));
            fourier_sum_dev_imag[k][ind[j]-1]=fourier_sum_dev_imag[k][ind[j]-1]+(value[j+1]*sin(-2*M_PI*k*n/N));
                
        }
            
        
        
    }
    
    if (i==N)
    {
        
        Complex temp;
        
        unsigned *indexes;
        
        double *dev_real;
        double *dev_imag;
        
        unsigned len=fourier_sum_dev_real[0].size();
        
        AAF re, im;
        
        if (len>0)
        {
        
         indexes=new unsigned[len];
         dev_real=new double[len];
         dev_imag=new double[len];
            
        }
        
        for (unsigned i=0; i<len; i++)
        {
            indexes[i]=i+1;
            
        }
        
        for (unsigned k=0; k<N; k++)
        {
            for (unsigned i=0; i<len; i++)
            {
                dev_real[i]=fourier_sum_dev_real[k][i];
                dev_imag[i]=fourier_sum_dev_imag[k][i];
                
            }
            
            if (len>0)
            {
            
             re=AAF(fourier_sum_nom_real[k],dev_real,indexes,len);
            
             im=AAF(fourier_sum_nom_imag[k],dev_imag,indexes,len);
                
                
            }
            else
            {
               re=fourier_sum_nom_real[k];
               im=fourier_sum_nom_imag[k];
            }
            
            
            temp.real=re;
            temp.imag=im;
            
            res.push_back(temp);
            
            fourier_sum_nom_real[k]=0;
            fourier_sum_nom_imag[k]=0;
            
            fourier_sum_dev_real[k].clear();
            fourier_sum_dev_imag[k].clear();
            
            
        
            
        } // end for
        
        i=0;
        n=-1;
        
        if (len>0)
        {
        
         delete []indexes;
         delete []dev_real;
         delete []dev_imag;
            
        }
    
        
    }
    
    ++i;
    ++n;
    return res;
}



