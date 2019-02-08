%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%FIR filter design for DSP Design Position
%   Author: J.Clarke
%   Date:   January 2019
%   Note:   Tested using Matlab R2018a
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

format long;

F_samp = 100e9;     %Samp/s
n_taps = 25;
F_cutoff = 25e9;    %Hz
max_pb_rip = 0.2;   %dB
F_stop = 35e9;      %Hz
max_sb_rip = -30;   %dB


w_cutoff = F_cutoff/F_samp;
%Assuming PB magnitude is 1:
max_pb_rip_lin = 10^(max_pb_rip/20)-1;
max_sb_rip_lin = 10^(max_sb_rip/20);

%We can play with this variable to put emphasis on stop/pass band criteria
wt = F_stop/F_samp; %stop-band emphasis
wt = F_cutoff/F_samp - 0.042; %pass-band emphasis--a convenient tuning parameter

coeffs = fircls1(n_taps-1,w_cutoff,max_pb_rip_lin,max_sb_rip_lin,wt)
fvtool(coeffs,1);

csvwrite('filter_coeffs.csv',coeffs');

input = csvread('input_data.csv');

figure(8); clf; hold on;
hist(input,20)
xlabel('Value');
ylabel('Frequency');

figure(9); clf; hold on;
plot(20*log10(abs(fft(input,1024)/1024)))
xlabel('FFT Bin index');
ylabel('Magnitude (dB)');

output = conv(input,coeffs);

figure(10); clf; hold on;
plot(20*log10(abs(fft(output,1024)/1024)))
xlabel('FFT Bin index');
ylabel('Magnitude (dB)');

figure(11); clf; hold on;
hist(output,20)
xlabel('Value');
ylabel('Frequency');
