#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <stdio.h>
#include <iomanip>
#include<map>
#include <string>

using namespace std;


class bimodal
{
	//initializations and declarations
	public:
	map<int,int>::iterator it;
	int size;
	int masking_value;
	unsigned long predictions, mispredictions;
	int predicted_t_or_n; 
	map<int,int> branch_predictor_table;
	unsigned long index; 
	
	void initialize(int m1) 
	{
		//size=2^m where m is no of bits 
		size = (int) pow(2, (double) m1);
		index=0;
		masking_value=size-1;
		mispredictions=0; 
		predictions=0;
		
		//initialize the table to 4
		for(int i = 0; i < size; i++)
		{
			branch_predictor_table[i] = 4; 
		}
	}
	
	int update_table(unsigned long address, int actual_branch_value)
	{
		predictions++;
		index = address >> 2;
		index = index & masking_value; 
		predicted_t_or_n = branch_predictor_table.at(index) >= 4 ? 1 : 0;   
		
		//check for mispredictions and update the table value
		if(actual_branch_value==1)
		{
			if(predicted_t_or_n == 0) 
			{
				mispredictions++;
			}
			if(branch_predictor_table.at(index) < 7) 
			{
				branch_predictor_table.at(index)++;
			}
			
		}
		if(actual_branch_value==0)
		{
			if(predicted_t_or_n == 1) 
			{
				mispredictions++;
			}
			if(branch_predictor_table.at(index) > 0) 
			{
				branch_predictor_table.at(index)--;
			}
			
		}
		return 1;
	}
	void print_output()
	{
		cout << "OUTPUT\nnumber of predictions: " << predictions << "\nnumber of mispredictions: " << mispredictions << fixed << setprecision(2) << "\nMisprediction rate: " << (double)mispredictions*100/predictions << "%\n";
		cout << "FINAL BIMODAL CONTENTS\n";
		for (it=branch_predictor_table.begin(); it!=branch_predictor_table.end(); ++it) 
		{
			cout << it->first << "\t" << it->second << "\n"; 
		}
	}
	
	//hybrid update_table
	int update_table_hybrid(unsigned long address, int actual_branch_value)
	{
		if(actual_branch_value==1)
		{
			if(predicted_t_or_n == 0) 
			{
				mispredictions++;
			}
			if(branch_predictor_table.at(index) < 7) 
			{
				branch_predictor_table.at(index)++;
			}
			
		}
		if(actual_branch_value==0)
		{
			if(predicted_t_or_n == 1) 
			{
				mispredictions++;
			}
			if(branch_predictor_table.at(index) > 0) 
			{
				branch_predictor_table.at(index)--;
			}
			
		}
		return 1;
	}
	
	int is_taken(unsigned long address)
	{
		index = address >> 2;
		index = index & masking_value;
		return branch_predictor_table.at(index) >= 4? 1 : 0;    
	}

	void print_status()
	{
		cout << "FINAL BIMODAL CONTENTS\n";
		for (it=branch_predictor_table.begin(); it!=branch_predictor_table.end(); ++it) 
		{
			cout << it->first << "\t" << it->second << "\n"; 
		}
	}
};

