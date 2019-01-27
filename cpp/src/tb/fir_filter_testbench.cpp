#include "../fir/FirFilter.hpp"

#include <iostream>
#include <stdint.h>
#include <unistd.h>
#include <cstdlib>
#include <math.h>
#include <cmath>
#include "../ac/include/ac_fixed.h"
#include "../ac/include/ac_float.h"


//#define DBG


////////////////////////////////////////////////////////////////////////////////
using namespace fir_filter;
using namespace std;
using namespace ac;
////////////////////////////////////////////////////////////////////////////////
void usage(void)
{

    cout << "FIR filter quantization comparison." <<endl;
    cout << "Usage:" <<endl;
    cout << "-h     Print this help message"<<endl;
    return;
}
////////////////////////////////////////////////////////////////////////////////
double calc_sqnr(vector<ac_8fp0_t> *output_vec_float, 
        vector<ac_8fx0_t> *output_vec_fixed)
{
    double sqnr;
    double numerator_running_sum = 0, denominator_running_sum = 0;
    for(auto i(0); i<output_vec_float->size(); i++)
    {
        numerator_running_sum = numerator_running_sum + 
            pow(output_vec_float->at(i).to_double(),2);
        denominator_running_sum = denominator_running_sum +
            pow(abs(output_vec_float->at(i).to_double() - 
            output_vec_fixed->at(i).to_double()),2);

        if(abs(output_vec_float->at(i).to_double() - 
            output_vec_fixed->at(i).to_double()) > 
            output_vec_float->at(i).to_double())
        {
            cout << i << endl;
            cout << output_vec_float->at(i).to_double()<< " vs." <<
                output_vec_fixed->at(i).to_double() << endl;
        }
    }
    cout << numerator_running_sum << "/"<< denominator_running_sum << endl;
    sqnr = 10*log10(numerator_running_sum/denominator_running_sum);
    return sqnr;
}
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    int c ;
    const uint32_t input_len = 65536;
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
    vector<ac_8fp0_t> *output_vec_float =
        new vector<ac_8fp0_t>();
    vector<ac_8fx0_t> *output_vec_fixed=
        new vector<ac_8fx0_t>();
    filt.process_fp(input_path, input_len, output_vec_float);
    filt.process_fx(input_path, input_len, output_vec_fixed);

    cout << output_vec_float->at(0) << ", " << output_vec_float->at(0).to_double()  << endl;
    cout << output_vec_fixed->at(0) << ", " << output_vec_fixed->at(0).to_double()  << endl;
    
    ac_8fx4_t tmp(0.3179384);
    ac_8fp0_t  tmp2(0.5);
    cout << tmp.to_double() << ", " << (tmp*tmp)  << endl;
    cout << tmp2.to_double() << ", " << (tmp*tmp)  << endl;

    cout << calc_sqnr(output_vec_float, output_vec_fixed) << endl;
    //Cleanup
    ////////////////////////////////////////////////////////////////////////////
    output_vec_fixed->clear();
    delete output_vec_fixed;
    output_vec_float->clear();
    delete output_vec_float;
    ////////////////////////////////////////////////////////////////////////////
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
