#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <stdio.h>
#include <iomanip>
#include<map>
#include <string>

using namespace std;

class gshare
{
	//initializations and declarations
	public:
	map<int,int>::iterator it;
	int size;
	int n, k, m1, m2;
	int masking_value1,masking_value2;
	unsigned long index;
	map<int,int> branch_predictor_table;
	unsigned long predictions, mispredictions; 
	int gbhr, predicted_t_or_n;
	
	void initialize(int m,int n1)
	{
		this->m2 = m;
		this->n = n1;
		//size=2^m where m is no of bits 
		size = (int) pow(2, (double) m2);
		index=0;
		masking_value1=size-1;
		mispredictions=0; 
		predictions=0;
		gbhr=0;
		//size=2^n - 1 where n is no of bits 
		masking_value2 = (int) pow(2, (double) n) - 1;
		
		//initialize table to 4
		for (int i = 0; i < size; i++) 
		{	
			branch_predictor_table[i] = 4;
		}
	}
	
	int update_table(unsigned long address, int actual_branch_value)
	{
		//getting the address value to index the table
		predictions++;
		unsigned long temp;
		//shift to right
		temp = address >> 2;
		temp = temp & masking_value1;
		if (n > 0) 
		{
			unsigned long lower_n;
			//exoring ghr with address
			lower_n = temp & masking_value2;
			lower_n = lower_n ^ gbhr;
			temp = temp &(~masking_value2);
			index = temp | lower_n;
		}
		else
			index = temp;
		
		//check for taken or not table
		predicted_t_or_n = branch_predictor_table.at(index) >= 4 ? 1 : 0;
		if(actual_branch_value==1)
		{
			if(branch_predictor_table.at(index) < 7) 
			{
				branch_predictor_table.at(index)++;
			}
			if (n > 0) 
			{
				gbhr = gbhr >> 1; 
				gbhr = gbhr | (1 << (n - 1));
			}
			if(predicted_t_or_n == 0) 
			{
				mispredictions++;
			}	
		}
		if(actual_branch_value==0)
		{
			if(branch_predictor_table.at(index) > 0) 
			{
				branch_predictor_table.at(index)--;
			}
			if (n > 0) 
			{
				gbhr = gbhr >> 1; 
				gbhr = gbhr | (0 << (n - 1));
			}
			if(predicted_t_or_n == 1) 
			{
				mispredictions++;
			}
			
			
		}
		return 1;
		
	}
	
	void print_output()
	{
		cout << "OUTPUT\nnumber of predictions: " << predictions << "\nnumber of mispredictions: " << mispredictions << fixed << setprecision(2) << "\nMisprediction rate: " << (double)mispredictions*100/predictions << "%\n";
		cout << "FINAL GSHARE CONTENTS\n";
		for (it=branch_predictor_table.begin(); it!=branch_predictor_table.end(); ++it) 
			cout << it->first << "\t" << it->second << "\n";
	}

	//for hybrid accesss
	
	int update_table_hybrid(unsigned long address, int actual_branch_value)
	{

		if(actual_branch_value==1)
		{
			if(branch_predictor_table.at(index) < 7) 
			{
				branch_predictor_table.at(index)++;
			}
			if (n > 0) 
			{
				gbhr = gbhr >> 1; 
				gbhr = gbhr | (1 << (n - 1));
			}
		}
		if(actual_branch_value==0)
		{
			if(branch_predictor_table.at(index) > 0) 
			{
				branch_predictor_table.at(index)--;
			}
			if (n > 0) 
			{
				gbhr = gbhr >> 1; 
				gbhr = gbhr | (0 << (n - 1));
			}
		}
		return 1;
	}
	 void update_gbhr(int i)
	 {
		if (n > 0) {
		  gbhr = gbhr >> 1; 
		  gbhr = gbhr | (i << (n- 1));
		}
	 }
	int is_taken(unsigned long address)
	{
		unsigned long temp;
		temp = address >> 2;
		temp = temp & masking_value1;
		if (n > 0) 
		{
			unsigned long lower_n;
			lower_n = temp & masking_value2;
			lower_n = lower_n ^ gbhr;
			temp = temp &(~masking_value2);
			index = temp | lower_n;
		}
		else
			index = temp;
		return branch_predictor_table.at(index) >=4 ? 1 : 0;
	}
	
	void print_status()
	{
		cout << "FINAL GSHARE CONTENTS\n";
		for (it=branch_predictor_table.begin(); it!=branch_predictor_table.end(); ++it) 
		cout << it->first << "\t" << it->second << "\n";
	}	
};
