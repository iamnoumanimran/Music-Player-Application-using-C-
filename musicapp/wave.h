#pragma once

#include <fstream>
#include <math.h>
#include <cmath>

using namespace std;

struct Wave
{
	unsigned long file_size;        
	unsigned short channel;         
	unsigned long frequency;        
	unsigned long Bps;              
	unsigned short sample_num_bit;  
	unsigned long data_size;        
	double wav_time;	
};