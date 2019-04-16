#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <iomanip>
#include <stdlib.h>
#include<map>
#include "gshare.h"
#include "bimodal.h"

class hybrid : public gshare, public bimodal
{

	public:
	//required variables
		unsigned long index;
		bimodal * b;
		int n, k, m1, m2;
		unsigned long  mispredictions;
		map<int, int>::iterator it;
		gshare * g;
		int bimodal_correct_predictions, gshare_correct_predictions;
		unsigned long predictions;
		map<int, int> chooser_history_table;
		int mask, size;
		int predicted_t_or_n;
		int bimodal_predict, gshare_predict;
  
		void initialize(int m1, int m2, int n, int k) 
		{
			predictions = mispredictions = 0;
			index = 0;
			g = new gshare;
			b = new bimodal;
			b->initialize(m1);
			g->initialize(m2,n);
			//2^k is size of chooser table
			size = (int) pow(2, (double) k);
			mask = size - 1;
			//initialize the table to 1
			for (int i = 0; i < size; i++) 
			{
				chooser_history_table[i] = 1; 
			}
		}

		int update_table(unsigned long address, int actual_branch_value)
		{
			predictions++;
			index = address >> 2;
			index = index & mask; 
			
			//check the actual prediction for bimodal and gshare
			bimodal_predict = b->is_taken(address);
			gshare_predict = g->is_taken(address);
			
			//if chooser table value greater than equal to 2 we choose gshare else we choose bimodal 
			if(chooser_history_table.at(index) >= 2)
			{
				//check for mispredictions
				if (actual_branch_value==1  && gshare_predict==0)
					mispredictions++;
				if(actual_branch_value==0  && gshare_predict==1)
					mispredictions++;
				//update the gshare table
				g->update_table_hybrid(address, actual_branch_value); 
			} 
			else 
			{
				// check for mispredictions
				if (actual_branch_value==1  && bimodal_predict==0)
				{
					mispredictions++;
				}
				if(actual_branch_value==0  && bimodal_predict==1)
				{
					mispredictions++;
				}
				//update the bimodal table and the global history table
				g->update_gbhr(actual_branch_value);
				b->update_table_hybrid(address, actual_branch_value);  
			}
			
			//check if bimodal value is correct
			if((actual_branch_value==1  && bimodal_predict==1)||(actual_branch_value==0  && bimodal_predict==0)) 
			{				
				bimodal_correct_predictions=1;

			}
			else 
			{
				bimodal_correct_predictions=0;
			}
			
			//check if gshare is correct
			if((actual_branch_value==1  && gshare_predict==1)||(actual_branch_value==0  && gshare_predict==0))  
			{
				gshare_correct_predictions=1;

			}
			else 
			{
				gshare_correct_predictions=0;
			}
			
			//increment chooser table if gshare correct
			 if(gshare_correct_predictions==1 && bimodal_correct_predictions==0)
			 {
				if(chooser_history_table.at(index) < 3)
				{
					chooser_history_table.at(index)++; 
				}
			 }
			 
			 //increment chooser table if bomidal correct
			 if(bimodal_correct_predictions==1 && gshare_correct_predictions==0)
			{
				if(chooser_history_table.at(index) > 0)
				{
					chooser_history_table.at(index)--;
				}
			}
			
			return 1;
		}


		void print_output()
		{
			cout << "OUTPUT\nnumber of predictions: " << predictions << "\nnumber of mispredictions: " << mispredictions << fixed << setprecision(2) << "\nMisprediction rate: " << (double)mispredictions*100/predictions << "%\n";  
			cout << "FINAL CHOOSER CONTENTS\n";
			for (it=chooser_history_table.begin(); it!=chooser_history_table.end(); ++it) 
			  cout << it->first << "\t" << it->second << "\n";
			g->print_status();
			b->print_status();		
		}


};