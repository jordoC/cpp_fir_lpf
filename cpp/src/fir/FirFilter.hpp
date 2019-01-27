#ifndef FIR_FILTER_H_
#define FIR_FILTER_H_ 

#include <string>
#include <vector>
#include <map>
#include <stdint.h>
#include "../ac/include/ac_fixed.h"
#include "../ac/include/ac_float.h"

using namespace std;

/**
 *  A class that implements floating point and fixed point input/output of
 *  a finite impulse response filter.
 *
 *  The filter requires several design funtions to be performed prior to 
 *  construction of the class. Namely, the design of the filter coefficients,
 *  and the quantizer partition/codebook parameters (required for fixed point
 *  implementation.
 *  
 */
namespace fir_filter 
{
    typedef ac_fixed<8,0,true> ac_8fx0_t;
    typedef ac_fixed<8,4,true> ac_8fx4_t;
    typedef ac_float<8,0,8,AC_RND> ac_8fp0_t;
////////////////////////////////////////////////////////////////////////////////
class FirFilter
{   
    ////////////////////////////////////////////////////////////////////////////
    public:
        static const uint8_t _num_bits = 8;
        /**
         *  Constructor
         *  @param  string coeffs_path
         *          Path to CSV file of filter coefficients
         *  @param  string partition_path 
         *          Path to CSV file of quantizer partitons
         *  @param  string codebook_path 
         *          Path to CSV file of quantizer codebook 
         */
        FirFilter(string coeffs_path, string partition_path, 
                string codebook_path);
        
        /**
         *  Destructor
         */
        virtual ~FirFilter();

        /**
         * process_fp
         */
        void process_fp(string input_data_path, uint32_t input_len,
                vector<ac_8fp0_t> *output_vec);
        
        /**
         * process_fx
         */
        void process_fx(string input_data_path);
        
    ////////////////////////////////////////////////////////////////////////////
    private:
        vector<ac_fixed<_num_bits,0,true>> *_coeffs_vec_fixed;
        vector<ac_float<_num_bits,0,8,AC_RND>> *_coeffs_vec_float;

        map<double,ac_fixed<_num_bits,4,true>> *_quantizer_fixed;
        map<double, ac_float<_num_bits,0,8,AC_RND>> *_quantizer_float;
        
};
////////////////////////////////////////////////////////////////////////////////
};
#endif  
