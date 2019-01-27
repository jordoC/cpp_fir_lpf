%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%8-bit Quantizer design for Ciena DSP position
%   Author: J.Clarke
%   Date:   January 2019
%   Note:   Tested using Matlab R2018a
%   Note:   Quantizer is optimized to input data provided: 65536 sample N(0,1)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
format short;
num_bits = 8;
num_frac_bits = 4;
input_data = csvread('input_data.csv');

%hist(input_data,20);
mu_input = mean(input_data);
var_input = var(input_data);

max_input_data = max(input_data);
min_input_data = min(input_data);

%We will assume a 8 bit quantizer centered on the mean of the input data
% which will have dynamic range of +/-5 sigma

uniform_codebook = -5:10/256:5-10/256;
uniform_partition = -5+10/256:10/256:5-10/256;

[nonuniform_partition nonuniform_codebook] = lloyds(input_data,256);

[index,quants,distor] = quantiz(input_data,uniform_partition,uniform_codebook);
[index2,quant2,distor2] = quantiz(input_data,nonuniform_partition,nonuniform_codebook);

eight_bit_codebook = fi(nonuniform_codebook,1,num_bits,num_frac_bits);
eight_bit_codebook_vec = zeros(256,1);
for i = 1:256
    tmp_fi = eight_bit_codebook(i);
    eight_bit_codebook_vec(i) = str2num(tmp_fi.Value);
end

[index2,quant2,distor3] = quantiz(input_data,nonuniform_partition,eight_bit_codebook_vec);

distor
distor2
distor3

%eight_bit_partition = fi(nonuniform_partition,1,num_bits,num_frac_bits);

csvwrite('nonuniform_quantizer_partition.csv',nonuniform_partition');
csvwrite('nonuniform_quantizer_codebook.csv',nonuniform_codebook');
csvwrite('nonuniform_quantizer_eight_bit_codebook.csv',eight_bit_codebook_vec);
