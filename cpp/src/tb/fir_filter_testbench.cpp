#include "../fir/FirFilter.hpp"

#include <iostream>
#include <stdint.h>
#include <unistd.h>
#include <cstdlib>
#include "../ac/include/ac_fixed.h"
#include "../ac/include/ac_float.h"


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
    static const uint8_t num_bits = 8;
    static const uint8_t num_int_bits = 4;
    string coeffs_path = "filter_coeffs.csv";
    string partition_path = "nonuniform_quantizer_partition.csv";
    string codebook_path = "nonuniform_quantizer_codebook.csv";
    string input_path = "input_data.csv";
    while( ( c = getopt (argc, argv, "c:p:q:i:h") ) != -1 ) 
    {
        switch(c)
        {
            case 'h':
                usage();
                return 0;
            case 'c':
                if(optarg) coeffs_path = optarg ;
                break;
            case 'p':
                if(optarg) partition_path = optarg ;
                break;
            case 'q':
                if(optarg) codebook_path = optarg ;
                break;
            case 'i':
                if(optarg) input_path = optarg ;
                break;
        }
    }

    FirFilter filt(coeffs_path, partition_path, codebook_path);
    vector<ac_float<num_bits,0,num_bits,AC_RND>> *output_float_vec =
        new vector<ac_float<num_bits,0,num_bits,AC_RND>>();
    filt.process_fp(input_path, output_float_vec);

    //Cleanup
    ////////////////////////////////////////////////////////////////////////////
    output_float_vec->clear();
    delete output_float_vec;
    
    return 0;
}
