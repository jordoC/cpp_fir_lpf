#include "../fir/FirFilter.hpp"
#include "../csv/CsvRow.cpp"

using namespace fir_filter;
using namespace std;
////////////////////////////////////////////////////////////////////////////////
FirFilter::FirFilter(string coeffs_path, string partition_path, 
                string codebook_path)
{
    ifstream coeffs_file(coeffs_path);
   _coeffs_vec_fixed = new vector<ac_8fx0_t>();
   _coeffs_vec_float = new vector<ac_8fp0_t>();
    int i = 0;
    for(CSVIterator coeff_itr(coeffs_file); 
            coeff_itr!= CSVIterator(); 
            coeff_itr++, i++)
    {
        _coeffs_vec_fixed->push_back(
                ac_8fx0_t(stof((*coeff_itr)[0])));
        _coeffs_vec_float->push_back(
                ac_8fp0_t(stof((*coeff_itr)[0])));
    }
    
    ifstream partition_file(partition_path);
    ifstream codebook_file(codebook_path);
    _quantizer_fixed = new map<double,ac_8fx4_t>();
    _quantizer_float = new map<double, ac_8fp0_t>();
    i = 0;
    double prev_part_val;
    for(CSVIterator part_itr(partition_file), codebook_itr(codebook_file); 
            codebook_itr!= CSVIterator(); 
            part_itr++, codebook_itr++, i++)
    {
        //Note: The partition vector has one less element than the codebook
        //      Therefore, we must tack one element onto the end of the map
        //      implementation of the quantizer to make sure the last 
        //      quantizer output has a key mapping it.
        if(part_itr == CSVIterator())
        {
            (*_quantizer_fixed)[prev_part_val+1.0] = 
                ac_8fx4_t(stof((*codebook_itr)[0]));
            (*_quantizer_float)[prev_part_val+1.0] = 
               ac_8fp0_t(stof((*codebook_itr)[0]));
        }
        else
        {

            (*_quantizer_fixed)[(stod((*part_itr)[0]))] = 
                ac_8fx4_t(stof((*codebook_itr)[0]));
            (*_quantizer_float)[(stod((*part_itr)[0]))] = 
               ac_8fp0_t(stof((*codebook_itr)[0]));
            prev_part_val = stod((*part_itr)[0]);
        }
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
void FirFilter::process_fp(string input_data_path, const uint32_t input_len,
        vector<ac_8fp0_t> *output_vec)
{

    ifstream input_file(input_data_path);
    vector<double> input_vec_float(input_len);

    //TODO: fix this up: the input vector is a row vector, and the other
    //      design files are column vectors: a bit inconsistent.
    for(CSVIterator input_itr(input_file); 
            input_itr!= CSVIterator(); 
            input_itr++) 
        for(uint32_t i=0; i<input_len; i++)
            input_vec_float.at(i) = stod((*input_itr)[i]);

    output_vec->clear();
    output_vec->resize(input_vec_float.size()+_coeffs_vec_float->size()-1);
    for(auto output_idx(0); 
            output_idx<output_vec->size();
            output_idx++)
    {
        int const f_idx_min = (output_idx >= input_len - 1)? 
            output_idx - (input_len - 1) : 0;
        int const f_idx_max = (output_idx <  (_coeffs_vec_float->size() - 1))? 
            output_idx : (_coeffs_vec_float->size() - 1);

        for(auto filt_idx(f_idx_min); filt_idx<=f_idx_max; filt_idx++)
        {
            output_vec->at(output_idx) = 0;
            ac_8fp0_t quantized_input = (_quantizer_float->lower_bound(
                        input_vec_float.at(output_idx - filt_idx))--)->second;
            output_vec->at(output_idx) +=  quantized_input * 
                _coeffs_vec_float->at(filt_idx); 
        }
    }
    return;
}
////////////////////////////////////////////////////////////////////////////////
void FirFilter::process_fx(string input_data_path, const uint32_t input_len,
        vector<ac_8fx0_t> *output_vec)
{

    ifstream input_file(input_data_path);
    vector<double> input_vec_float(input_len);

    //TODO: fix this up: the input vector is a row vector, and the other
    //      design files are column vectors: a bit inconsistent.
    for(CSVIterator input_itr(input_file); 
            input_itr!= CSVIterator(); 
            input_itr++) 
        for(uint32_t i=0; i<input_len; i++)
            input_vec_float.at(i) = stod((*input_itr)[i]);

    output_vec->clear();
    output_vec->resize(input_vec_float.size()+_coeffs_vec_fixed->size()-1);
    for(auto output_idx(0); 
            output_idx<output_vec->size();
            output_idx++)
    {
        int const f_idx_min = (output_idx >= input_len - 1)? 
            output_idx - (input_len - 1) : 0;
        int const f_idx_max = (output_idx <  (_coeffs_vec_fixed->size() - 1))? 
            output_idx : (_coeffs_vec_fixed->size() - 1);

        for(auto filt_idx(f_idx_min); filt_idx<=f_idx_max; filt_idx++)
        {
            output_vec->at(output_idx) = 0;
            ac_8fx4_t quantized_input = (_quantizer_fixed->lower_bound(
                        input_vec_float.at(output_idx - filt_idx))--)->second;
            output_vec->at(output_idx) +=  quantized_input * 
                _coeffs_vec_fixed->at(filt_idx);
            
        }
    }
    return;
}
////////////////////////////////////////////////////////////////////////////////
