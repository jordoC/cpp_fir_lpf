#include "../fir/FirFilter.hpp"
#include "../csv/CsvRow.cpp"

using namespace fir_filter;
using namespace std;
////////////////////////////////////////////////////////////////////////////////
FirFilter::FirFilter(string coeffs_path, string partition_path, 
                string codebook_path, uint8_t num_frac_bits)
{
    //CSVRow coeffs_csv;
    ifstream coeffs_file(coeffs_path);

    vector<ac_fixed<_num_bits,0,true>> *_coeffs_vec_fixed = 
        new vector<ac_fixed<_num_bits,0,true>>();
    vector<ac_float<_num_bits,0,1,AC_RND>> *_coeffs_vec_float = 
        new vector<ac_float<_num_bits,0,1,AC_RND>>();

    int i = 0;
    for(CSVIterator loop(coeffs_file); loop != CSVIterator(); ++loop, i++)
    {
        float floating_coeff = stof((*loop)[0]);
        ac_fixed<_num_bits,0,true> fixed_coeff(floating_coeff);
        ac_float<_num_bits,0,1,AC_RND> float_coeff(floating_coeff);
        _coeffs_vec_fixed->push_back(fixed_coeff);
        _coeffs_vec_float->push_back(float_coeff);
        //cout << (*loop)[0] << ", ";
        //cout << fixed_coeff << ", ";
        //cout << float_coeff << endl;
    }

    for(int i=0; i<_coeffs_vec_float->size(); i++)
        cout << (*_coeffs_vec_float)[i] << endl;
    return;
}
////////////////////////////////////////////////////////////////////////////////
FirFilter::~FirFilter()
{
    //REMINDER: delete things in reverse alloaction order
    cout << _coeffs_vec_float; 
    //_coeffs_vec_float->clear();
    //delete _coeffs_vec_float;
    //delete _coeffs_vec_fixed;
    return;
}
////////////////////////////////////////////////////////////////////////////////
