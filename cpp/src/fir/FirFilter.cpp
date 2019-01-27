#include "../fir/FirFilter.hpp"
#include "../csv/CsvRow.cpp"

using namespace fir_filter;
using namespace std;
////////////////////////////////////////////////////////////////////////////////
FirFilter::FirFilter(string coeffs_path, string partition_path, 
                string codebook_path)
{
    ifstream coeffs_file(coeffs_path);
   _coeffs_vec_fixed = new vector<ac_fixed<_num_bits,0,true>>();
   _coeffs_vec_float = new vector<ac_float<_num_bits,0,_num_bits,AC_RND>>();
    int i = 0;
    for(CSVIterator coeff_itr(coeffs_file); 
            coeff_itr!= CSVIterator(); 
            coeff_itr++, i++)
    {
        _coeffs_vec_fixed->push_back(
                ac_fixed<_num_bits,0,true>(stof((*coeff_itr)[0])));
        _coeffs_vec_float->push_back(
                ac_float<_num_bits,0,_num_bits,AC_RND>(stof((*coeff_itr)[0])));
        //cout << (*coeff_itr)[0] << ", ";
        //cout << fixed_coeff << ", ";
        //cout << float_coeff << endl;
    }
    
    ifstream partition_file(partition_path);
    ifstream codebook_file(codebook_path);
    _quantizer_fixed = new map<double,ac_fixed<_num_bits,4,true>>();
    _quantizer_float = new map<double, ac_float<_num_bits,0,_num_bits,AC_RND>>();
    i = 0;
    for(CSVIterator part_itr(partition_file), codebook_itr(codebook_file); 
            part_itr!= CSVIterator(); 
            part_itr++, codebook_itr++, i++)
    {
        (*_quantizer_fixed)[(stod((*part_itr)[0]))] = 
            ac_fixed<_num_bits,4,true>(stof((*codebook_itr)[0]));
        (*_quantizer_float)[(stod((*part_itr)[0]))] = 
           ac_float<_num_bits,0,_num_bits,AC_RND>(stof((*codebook_itr)[0]));
        //cout << (*loop)[0] << ", ";
        //cout << fixed_coeff << ", ";
        //cout << float_coeff << endl;
    }
    return;
}
////////////////////////////////////////////////////////////////////////////////
FirFilter::~FirFilter()
{
    //REMINDER: delete things in reverse alloaction order
    _quantizer_float->clear();
    delete _quantizer_float;
    _quantizer_fixed->clear();
    delete _quantizer_fixed;
    _coeffs_vec_float->clear();
    delete _coeffs_vec_float;
    _coeffs_vec_fixed->clear();
    delete _coeffs_vec_fixed;
    return;
}
////////////////////////////////////////////////////////////////////////////////
void FirFilter::process_fp(string input_data_path, 
        vector<ac_float<_num_bits,0,_num_bits,AC_RND>> *output_vec)
{
    output_vec->clear();

    ifstream input_file(input_data_path);
    const uint32_t input_len = 65536;
    vector<double> input_vec_float(input_len);
    for(CSVIterator input_itr(input_file); 
            input_itr!= CSVIterator(); 
            input_itr++) 
    {
        for(uint32_t i=0; i<input_len; i++)
        {
            input_vec_float.at(i) = stod((*input_itr)[i]);
                    //cout << (*input_itr)[i] << endl;
        }
    }

    //for ( i = 0; i < sampleCount; i++ )
    //{
    //    y[i] = 0;                       // set to zero before sum
    //    for ( j = 0; j < kernelCount; j++ )
    //    {
    //        y[i] += x[i - j] * h[j];    // convolve: multiply and accumulate
    //    }
    //}
    for(int i=0; i<10; i++)
        cout << input_vec_float[i] << endl;
    //cout << _coeffs_vec_float->size() << endl;
    //for(int i=0; i<_coeffs_vec_float->size(); i++)
    //    cout << (*_coeffs_vec_float)[i] << endl;
    //for(int i=0; i<_coeffs_vec_fixed->size(); i++)
    //    cout << (*_coeffs_vec_fixed)[i] << endl;
    return;
}
////////////////////////////////////////////////////////////////////////////////
