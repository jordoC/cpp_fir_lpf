#include "../fir/FirFilter.hpp"

#include <iostream>
#include <stdint.h>
#include <unistd.h>
#include <cstdlib>


//#define DBG


using namespace fir_filter;
using namespace std;

void usage(void)
{

    cout << "FIR filter quantization comparison." <<endl;
    cout << "Usage:" <<endl;
    cout << "-h     Print this help message"<<endl;
    return;
}

int main(int argc, char* argv[])
{
    int c ;
    uint8_t num_frac_bits = 4;
    string coeffs_path = "filter_coefs.csv";
    while( ( c = getopt (argc, argv, "c:h") ) != -1 ) 
    {
        switch(c)
        {
            case 'h':
                usage();
                return 0;
            case 'c':
                if(optarg) coeffs_path = optarg ;
                break;
            //case 'e':
            //    if(optarg) orth_constr = stod(optarg) ;
            //    break;
            //case 'r':
            //    if(optarg) num_realizations = atoi(optarg) ;
            //    break;
            //case 's':
            //    if(optarg) num_samps = atoi(optarg) ;
            //    break;
        }
    }

    FirFilter filt(coeffs_path,"","",4);

    //Cleanup
    ////////////////////////////////////////////////////////////////////////////
    
    return 0;
}
