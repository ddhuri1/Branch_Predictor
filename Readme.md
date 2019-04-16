#######Simulator Specification 
Model a gshare branch predictor with parameters {m, n}, where: 
 m is the number of low‐order PC bits used to form the prediction table index. Note: discard the 
lowest two bits of the PC, since these are always zero, i.e., use bits m+1 through 2 of the PC. 
 n is the number of bits in the global branch history register. Note: n ≤ m. Note: n may equal 
zero, in which case we have the simple bimodal branch predictor. 
3.1. n=0: bimodal branch predictor 
When n=0, the gshare predictor reduces to a simple bimodal predictor. In this case, the index is based 
on only the branch’s PC, as shown in Fig. 1 below. 
Entry in the prediction table: 
An entry in the prediction table contains a single 3‐bit counter. All entries in the prediction table should 
be initialized to 4 (“weakly taken”) when the simulation begins. 
Regarding branch interference: 
Different branches may index the same entry in the prediction table. This is called “interference”. 
Interference is not explicitly detected or avoided: it just happens. (There is no tag array, no tag checking, 
and no “miss” signal for the prediction table.) 
When you get a branch from the trace file, there are three steps: 
(1) Determine the branch’s index into the prediction table. 
(2) Make a prediction. Use index to get the branch’s counter from the prediction table. If the counter 
value is greater than or equal to 4, then the branch is predicted taken, else it is predicted not‐taken. 
(3) Update the branch predictor based on the branch’s actual outcome. The branch’s counter in the 
prediction table is incremented if the branch was taken, decremented if the branch was not‐taken. 
The counter saturates at the extremes (0 and 7), however.


3.2. n>0: gshare branch predictor
When n > 0, there is an n‐bit global branch history register. In this case, the index is based on both the 
branch’s PC and the global branch history register, as shown in Fig. 2 below. The global branch history 
register is initialized to all zeroes (00...0) at the beginning of the simulation. 
When you get a branch from the trace file, there are four steps: 
(1) Determine the branch’s index into the prediction table. Figure 2 shows how to generate the index: 
the current n‐bit global branch history register is XORed with the lowermost n bits of the index (PC) 
bits. 
(2) Make a prediction. Use index to get the branch’s counter from the prediction table. If the counter 
value is greater than or equal to 4, then the branch is predicted taken, else it is predicted not‐taken. 
(3) Update the branch predictor based on the branch’s actual outcome. The branch’s counter in the 
prediction table is incremented if the branch was taken, decremented if the branch was not‐taken. 
The counter saturates at the extremes (0 and 7), however. 
(4) Update the global branch history register. Shift the register right by 1 bit position, and place the 
branch’s actual outcome into the most‐significant bit position of the register. 

3.3. Hybrid branch predictor
Model a hybrid predictor that selects between the bimodal and the gshare predictors, using a chooser 
table of 2k
 2‐bit counters. All counters in the chooser table are initialized to 1 at the beginning of the 
simulation. 
When you get a branch from the trace file, there are six top‐level steps 
(1) Obtain two predictions, one from the gshare predictor (follow steps 1 and 2 in Section 3.2) and 
one from the bimodal predictor (follow steps 1 and 2 in Section 3.1). 
(2) Determine the branch’s index into the chooser table. The index for the chooser table is bit k+1 to 
bit 2 of the branch PC (i.e., as before, discard the lowest two bits of the PC). 
(3) Make an overall prediction. Use index to get the branch’s chooser counter from the chooser table. 
If the chooser counter value is greater than or equal to 2, then use the prediction that was obtained 
from the gshare predictor, otherwise use the prediction that was obtained from the bimodal 
predictor. 
(4) Update the selected branch predictor based on the branch’s actual outcome. Only the branch 
predictor that was selected in step 3, above, is updated (if gshare was selected, follow step 3 in 
Section 3.2, otherwise follow step 3 in Section 3.1). 
(5) Note that the gshare’s global branch history register must always be updated, even if bimodal was 
selected (follow step 4 in Section 3.2). 

###### Inputs to Simulator 
The simulator reads a trace file in the following format: 
<hex branch PC>  t|n 
<hex branch PC>  t|n 
... 
Where: 
 <hex branch PC> is the address of the branch instruction in memory. This field is used to index 
into predictors. 
  "t" indicates the branch is actually taken (Note! Not that it is predicted taken!). Similarly, "n" 
indicates the branch is actually not‐taken. 
Example: 
00a3b5fc t   
00a3b604 t   
00a3b60c n  


######Outputs from Simulator 
The simulator outputs the following measurements after completion of the run: 
a. number of accesses to the predictor (i.e., number of branches) 
b. number of branch mispredictions (predicted taken when not‐taken, or predicted not‐taken when 
taken) 
c. branch misprediction rate (# mispredictions / # branches) 


### Validation requirements 
Sample simulation outputs will be provided on the website. These are called “validation runs”. You must 
run your simulator and debug it until it matches the validation runs. 
Each validation run includes: 
  (1) The branch predictor configuration. 
  (2) The final contents of the branch predictor. 
  (3) All measurements described in Section 5. 
Your simulator must print outputs to the console (i.e., to the screen). (Also see Section 6.2 about this 
requirement.) 
Your output must match both numerically and in terms of formatting, because the TAs will literally “diff” 
your output with the correct output. You must confirm correctness of your simulator by following these 
two steps for each validation run: 
1) Redirect the console output of your simulator to a temporary file. This can be achieved by placing “> 
your_output_file” after the simulator command. 
2) Test whether or not your outputs match properly, by running this unix command: 
“diff –iw <your_output_file> <posted_output_file>” 
The –iw flags tell “diff” to treat upper‐case and lower‐case as equivalent and to ignore the amount of 
whitespace between words. Therefore, you do not need to worry about the exact number of spaces or 
tabs as long as there is some whitespace where the validation runs have whitespace. 




