#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include <cassert>
#include <map>
#include <iomanip>
#include "hybrid.h"

using namespace std;

int TYPE;
int val;

int main(int argc, char *argv[])
{
	//initializations
	int take=0,nottake=0,decide=0;
	FILE * trace_file;
	char actual_branch_value;
	unsigned long address;
	int n, k, m1, m2;
	
	//Allocate memory for the respective class objects
	hybrid * h=new hybrid;
	gshare * g=new gshare;
	bimodal * b=new bimodal;
	
	//Fname is the output file
	char * fname;
	nottake=0;
	/*
	BIMODAL=0
	GSHARE=1
	HYBRID=2
	*/
	
	//According to type get the proper arguments
	if(strcmp(argv[1],"hybrid") == 0) 
	{
		TYPE = 3;
		m1 = atoi(argv[5]);
		m2 = atoi(argv[3]);
		n = atoi(argv[4]);
		k = atoi(argv[2]);
		fname = argv[6];
		cout << "COMMAND\n./sim " << argv[1] << " "<< k << " " << m2 << " " << n  << " " << m1  << " "<< fname << endl;
	}
	else if(strcmp(argv[1],"gshare") == 0) 
	{
		TYPE = 2;
		m2 = atoi(argv[2]);
		n = atoi(argv[3]);
		fname = argv[4];
		cout << "COMMAND\n./sim " << argv[1] << " " << m2 << " " << n  << " " << fname << endl;
	}
	else if(strcmp(argv[1],"bimodal") == 0) 
	{
		TYPE = 1;
		m1 = atoi(argv[2]);
		fname = argv[3];
		cout << "COMMAND\n./sim " << argv[1] << " " <<  m1 << " " << fname << endl;
	}
	decide=0;
	take=1;
	if(decide==1)
		if(take&&nottake==0)
			decide=nottake;
	
	//open the trace file
	trace_file = fopen(fname, "r+");
	if(trace_file == 0) 
	{
		cout << "\nThere is a problem reading the trace file." <<endl;
		return false;
	}
	
	
	switch(TYPE)
	{
		case 3:
			h->initialize(m1, m2, n, k);
			//initialize hybrid table
			while(1) 
			{
				//check if actual branch is taken or not
				if(fscanf(trace_file, "%lx %c", &address, &actual_branch_value) != EOF)
				{
					if(actual_branch_value == 't')
					{
						val=1;
					}
					else 
					{
						val=0;

					}
					h->update_table(address, val);
				}
				else
				{
					break;
				}
			}
			h->print_output();
			decide=1;
			//h->print_stats();
		  break;
		  
		case 2:
			g->initialize(m2,n);  
			//initialize gshare table
			while(1) 
			{
				if(fscanf(trace_file, "%lx %c", &address, &actual_branch_value) != EOF)
				{
					//check if actual branch is taken or not
					if(actual_branch_value == 't')
					{
						val=1;
					}
					else 
					{
						val=0;
					}
					g->update_table(address, val);
				}
				else
				{				
					break;
				}
			}
			g->print_output();
			decide=0;
		  break;


		case 1: 
			b->initialize(m1);
			//initialize bimodal table
			while(1) 
			{
				if(fscanf(trace_file, "%lx %c", &address, &actual_branch_value) != EOF)
				{
					//check if actual branch is taken or not
					if(actual_branch_value == 't')
						val=1;
					else 
						val=0;
					b->update_table(address, val);
				}
				else
				{
					break;
				}
			}
			b->print_output();
			take=1;
		break;
		  
	}
	return 1;
}
	  

	  