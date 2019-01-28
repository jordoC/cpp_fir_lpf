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
    //Useful bit-accurate AC types
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
         *  process_fp
         *  Filter the input data using 8-bit bit-accurate floating point
         *  math. 
         *  @param  string input_data_path
         *          Path to a CSV file who contains a floating point
         *          representation of the input data to be fed into
         *          the filter. The method expects data to be in a single
         *          row. 
         *  @param  input_len uint_32_t
         *          Number of input samples in the CSV file row.
         *  @param  vector<ac_8_fp0_t>>*
         *          The output vector passed by reference. Method clears
         *          this vector, convolves the input given with the filter
         *          impulse response, then saves the value into the vector.
         *          The vector will have length of input_len + the number of 
         *          filter taps -1 after the call. 
         */
        void process_fp(string input_data_path, uint32_t input_len,
                vector<ac_8fp0_t> *output_vec);
        
        /**
         *  process_fx
         *  Filter the input data using 8-bit bit-accurate fixed point
         *  math. 
         *  @param  string input_data_path
         *          Path to a CSV file who contains a floating point
         *          representation of the input data to be fed into
         *          the filter. The method expects data to be in a single
         *          row. 
         *  @param  input_len uint_32_t
         *          Number of input samples in the CSV file row.
         *  @param  vector<ac_8_fp0_t>>*
         *          The output vector passed by reference. Method clears
         *          this vector, convolves the input given with the filter
         *          impulse response, then saves the value into the vector.
         *          The vector will have length of input_len + the number of 
         *          filter taps -1 after the call. 
         */
        void process_fx(string input_data_path, const uint32_t input_len,
                vector<ac_8fx0_t> *output_vec);
        
    ////////////////////////////////////////////////////////////////////////////
    private:
        /**
         *  8-bit floating point representation of filter coefficeints
         */
        vector<ac_8fx0_t> *_coeffs_vec_fixed;

        /**
         *  8-bit fixed point representation of filter coefficeints
         */
        vector<ac_8fp0_t> *_coeffs_vec_float;

        /**
         *  8-bit floating point representation of non-uniform quantizer
         */
        map<double, ac_8fp0_t> *_quantizer_float;

        /**
         *  8-bit fixed point representation of non-uniform quantizer
         */
        map<double,ac_8fx4_t> *_quantizer_fixed;
        
};
////////////////////////////////////////////////////////////////////////////////
};
#endif  
