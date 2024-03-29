/**
To do the optimal palindrome spaced seed
*/
#include <iostream>
#include <vector>
#include "command_line_parser.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;

int l;         //Seeds lengths
int  N;         //Length of the random region
double p;       //Similarity level
int w;          // Weight
int tries;     // tries num

void FLIP(char* maskedSeed, int pos1, int pos2){
    maskedSeed[pos1] = (maskedSeed[pos1] == '1') ? '0' : '1';
    maskedSeed[l - pos1 - 1] = maskedSeed[pos1]; 
    maskedSeed[pos2] = (maskedSeed[pos2] == '1') ? '0' : '1';
    maskedSeed[l - pos2 - 1] = maskedSeed[pos2];
    return;
}

int Int_Power(int base, int exp){
    int sum = 1;
    while (exp --){
        sum *= base;
    }
    return sum;
}

void printMaskedSeed(char* maskedSeed){
    for (int i = 0; i < l + 1; i++){
        cout << maskedSeed[i];
    }
    cout << endl;
    return;
}
// OC(S) = 2^sigma(i), i = 1 .... l
double Two_Seed_Overlaps(char* currentSeed){ // compute the number of overlaps 
    double overlaps = 0;
    int one_pairs = 0, i = 0, one_star_pairs = 0, two_star_pairs=0, shift = 0; 
    for (shift = 1; shift < l; shift++){
        one_pairs = 0;
        one_star_pairs = 0;
        two_star_pairs = 0;
        for (i = shift; i < l; i++){
            if ((currentSeed[i] == '1') && (currentSeed[i - shift] == '1')){
                one_pairs ++;
            }
            one_star_pairs = 2*(w - one_pairs);
            two_star_pairs = (l + shift) - one_pairs - one_star_pairs;
            overlaps += Int_Power(2, one_pairs);
        }
    }
    return overlaps;
    
}
inline long long Bin_Reversed_To_INT(char *maskedSeed){
    long long val = 0, temp = 1;
    int i = 0; 
    for (i = 0; i <= l - 1; i++){
        if (maskedSeed[i] == '1') val += temp;
        temp *= 2;
    }
    return val;
}

void Palindrome_Swap(char* maskedSeed){
    int i = 0, j = 0;
    double cur_overlaps = 0, best_overlaps = 0;
    int best_i = 0, best_j = 0;
    best_overlaps = Two_Seed_Overlaps(maskedSeed);
    for (i = 0; i < l / 2; i++){
        for (j = i + 1; j < l / 2; j++){
            if ((maskedSeed[i] == '1' && maskedSeed[j] == '0') || (maskedSeed[i] == '0' && maskedSeed[j] == '1')) {
                FLIP(maskedSeed, i, j);
                cur_overlaps = Two_Seed_Overlaps(maskedSeed);
                if (best_overlaps > cur_overlaps){
                    best_overlaps = cur_overlaps;
                    best_i = i; best_j = j;
                }
                FLIP(maskedSeed, i, j); // restore
            }
            
        }
    }
    FLIP(maskedSeed, best_i, best_j);

    return;
}



/*
* Computing sensitivity of a palindrome seed with the given parameters
* using the dynamic programming of (Li et al., 2004)
* I used this code segment from SpEED software to help me computing sensitivity.
*/
double Sensitivity(char** SEEDS, int NO_SEEDS, long long N, double P)
{
	long long i=0, j=0, b=0, pos=0, MAX_L=0, level=0, prev_level_start=0, prev_level_end=0, compatible=0,
		hit=0, suffix_link=0, zero_link=0, new_i=0, tmp=0;
	long long b_zero=0, b_one=0;
	double f0=0, f1=0;
// compute the lengths of the seeds and MAX_L = the length of the longest seed

	long long* seed_length = new long long [NO_SEEDS];
	for (i=0; i<=NO_SEEDS-1; i++) {
		seed_length[i] = strlen(SEEDS[i]);
		if (MAX_L < seed_length[i]) MAX_L = seed_length[i];
	}
// compute the integer values of the reversed seeds INTeger REVersed SEEDS

	long long* INT_REV_SEEDS = new long long[NO_SEEDS];
	for (i=0; i<=NO_SEEDS-1; i++)
		INT_REV_SEEDS[i] = Bin_Reversed_To_INT(SEEDS[i]); // !!! this works like * = 0

// create the tree of BS --- 1..NO_BS-1 *********************************************************
// BS[i][0] = the integer value of b^r (except for epsilon, any b starts with 1)
// BS[i][1] = index j in BS of left son: BS[j][0] = integer value of b^r0 = (0b)^r (-1 if it doesn't exist)
// BS[i][2] = index j in BS of right son: BS[j][0] = integer value of b^r1 = (1b)^r (-1 if it doesn't exist)
// BS[i][3] = (suffix link) index j in BS of (B(b^r))^r i.e. BS[j][0] = integer value of (B(b^r))^r
// BS[i][4] = 1 if b is a hit and 0 otherwise			//B(x) is the longest prefix of x that is in B
														//B = set of compatible but not hits b's
// BS[i][5] = its level = the length of the string
// BS[i][6] = the longest prefix of 0b which reversed means the longest suffix followed by a 0

	long long MAX_NO_BS = NO_SEEDS;
	for (i=0; i<=NO_SEEDS-1; i++) {		// compute maximum possible no of b's
		tmp = 1;
		for (j=strlen(SEEDS[i])-1; j>=0; j--) {
			if (SEEDS[i][j] != '1') tmp *= 2;
			MAX_NO_BS += tmp;        // add previous value if 1 in seed or double (tmp *= 2 above) if a * in seed
		}
	}
	long long NO_BS = MAX_NO_BS;

	//bound for computing sensitivity (not allocate more than 120GB)

	long long **BS = new long long *[NO_BS];

	for (i=0; i<=NO_BS-1; i++)	{
		BS[i] = new long long [7];
        BS[i][0] = BS[i][3] = BS[i][4] = BS[i][5] = BS[i][6] = 0; // initialize
		BS[i][1] = BS[i][2] = -1;
	}
// create the tree by levels: all b's of length i are on level i
	BS[0][0] = 0; //epsilon
	BS[0][1] = -1; //no left son since b=0 is not compatible -- seeds end with 1
	BS[0][2] = 1; //right son is BS[1][0] = 1
	BS[0][3] = 0; //suffix link to itself
	BS[0][4] = 0; //epsilon is not hit
	BS[0][5] = 0;
	BS[0][6] = 0;
	BS[1][0] = 1;
	BS[1][3] = 0;
	BS[1][4] = 0; // assume 1 is not a hit
	BS[1][5] = 1;
	prev_level_start = 1; prev_level_end = 1; // indices in BS between which previous level is found
	pos = 2; //first empty position in BS
	for (level=2; level<=MAX_L; level++) { // complete level "level"
		for (i = prev_level_start; i <= prev_level_end; i++)
			if (BS[i][4] != 1) { // not a hit
				b = BS[i][0]; //integer value
				b_zero = 2 * b; // try b0
				compatible = 0; hit = 0;
				for (j=0; j<=NO_SEEDS-1; j++) 	// check long enough seeds to seee if b0 is compat/hit
					if (seed_length[j] >= level)
						if (((INT_REV_SEEDS[j] >> (seed_length[j] - level)) & (~b_zero)) == 0 ) {
							compatible = 1;
							if (level == seed_length[j])
								hit = 1;
						}
				if (compatible) {
					BS[i][1] = pos;
                    BS[pos][0] = 2 * b;
					BS[pos][4] = hit; // hit = 1 if it is hit by a seed
					BS[pos][5] = level;
					suffix_link = BS[i][3];
					while ((suffix_link != 0) && (BS[suffix_link][1] == -1)) {
						suffix_link = BS[suffix_link][3];
					}
					if (suffix_link != 0) {
						BS[pos][3] = BS[suffix_link][1];
						if (BS[BS[pos][3]][4] == 1)		// if suffix link is hit then also itself is hit
							BS[pos][4] = 1;
					}
					pos++;
				}
				b_one = 2 * b + 1; // try b1
				compatible = 0; hit = 0;
				for (j=0; j<=NO_SEEDS-1; j++) 	// check all long enough seed to seee if b0 is compat/hit
					if (seed_length[j] >= level)
						if (((INT_REV_SEEDS[j] >> (seed_length[j] - level)) & (~b_one)) == 0 ) {
							compatible = 1;
							if (level == seed_length[j])
								hit = 1;
						}
				if (compatible) {
					BS[i][2] = pos;
					BS[pos][0] = 2 * b + 1;
					BS[pos][4] = hit; // hit = 1 if it is hit by a seed
					BS[pos][5] = level;
					suffix_link = BS[i][3];
					while (BS[suffix_link][2] == -1) {
						suffix_link = BS[suffix_link][3];
					}
					BS[pos][3] = BS[suffix_link][2];
					if (BS[BS[pos][3]][4] == 1)		// if suffix link is hit then also itself is hit
						BS[pos][4] = 1;

					pos++;
				}
			}
			prev_level_start = prev_level_end+1;
			prev_level_end = pos - 1;
	}
// zero_links -- longest suffix of b0 in the tree
	for (i=1; i<=NO_BS-1; i++)
		if (BS[i][1] != -1)		// has left son, that is, 0-son
			BS[i][6] = BS[i][1];
		else {
			zero_link = BS[i][3];
			while ((zero_link != 0) && (BS[zero_link][1] == -1)) {
				zero_link = BS[zero_link][3];
			}
			if (zero_link != 0)
				BS[i][6] = BS[zero_link][1];
		}
// compute the f's  f[i][j] = probab to hit a prefix of length i that ends with INT_TO_BIN_REVERSED[BS[j][0]]
	double **f;
	f = new double* [N+1];
	for (i=0; i<=N; i++) {
		f[i] = new double [NO_BS];
		for (j=0; j<=NO_BS-1; j++) f[i][j] = 0; //initialize
	}


	for (i=0; i<=N; i++) {
		for (j=NO_BS-1; j>=0; j--) {
			if (i==0) f[i][j] = 0;					// empty prefix of random region cannot be hit
			else if (i < BS[j][5]) f[i][j] = 0;		// too short
			else if (BS[j][4] == 1) f[i][j] = 1;	// hit
			else {
				new_i = i - BS[j][5] + BS[BS[j][6]][5] - 1;
				if (new_i < 0) new_i = 0;
				f0 = f[ new_i ][ BS[j][6] ];
				if (BS[j][2] < 0)
					f1 = 1;
				else
					f1 = f[ i ][ BS[j][2] ];
				f[i][j] = (1-P)*f0 + P*f1;
			}
		}
	}
	double result = f[N][0];
// free memory
	delete[] seed_length; delete[] INT_REV_SEEDS;
	for (i=0; i<=N; i++)
		delete[] f[i];
	delete[] f;
	for (i=0; i<=MAX_NO_BS-1; i++)
		delete[] BS[i];
	delete[] BS;

	return(result);
}
 

void Optimal_Palindrome_Spaced_Seed(char* maskedSeed, int weight, int length, int tries, int N, double p, double bestSensitivity){
    double currentSensitivity = 0;
    int pos = 0, k = 0, j = 0;
    srand((unsigned)(time(0)));
    double t[2] = {0, 0};
    t[0] = clock() / 1000000.0;
    
    for (k = 0; k < tries; k++){ 
        //initial seed
        for (j = 1; j < l / 2 ; j++){
            maskedSeed[j] = '0';
            maskedSeed[l-j-1] = '0';
        }
        maskedSeed[0] = '1';
        maskedSeed[l/2] = '1';
        maskedSeed[l - 1] = '1';
        maskedSeed[l] = '\0';
        //method 1
        for (j = 2; j < w / 2 + 1; j++){
            pos = rand()%(l/2 - j + 1) + 1;
            int j1 = 0;
            while(pos > 0){
                if (maskedSeed[j1] == '0'){
                    pos--;
                }
                j1++;
            }
            maskedSeed[j1 - 1] = '1';
            maskedSeed[l - j1] = '1';

        }
        //method 2  
        // j = 2;
        // while (j <= w / 2){
        //     pos = rand()%(l/2 - j + 1) + 1;
        //     while (maskedSeed[pos] != '0' && pos < l / 2){
        //         pos++;
        //     }
        //     maskedSeed[pos] = '1';
        //     maskedSeed[l-pos-1] = '1';
        //     j++;
        // }
        // Palindrome Swap
        Palindrome_Swap(maskedSeed);
        char** sensitivity_seed = &maskedSeed;
        currentSensitivity = Sensitivity(sensitivity_seed, 1, N, p);
        if (currentSensitivity != -1){ // need to check
            if (currentSensitivity > bestSensitivity){
                bestSensitivity = currentSensitivity;
                t[1] = clock() / 1000000.0;
                cout << "\n --- random try number " << k + 1 << " --- " << endl;
                cout << "seeds: " << endl;
                printMaskedSeed(maskedSeed);
                cout << "sensitivity = " << bestSensitivity << endl;
                cout << "time (since beginning): " << t[1] - t[0] << endl << flush;
            }
        }
    }
    cout << endl << "Best sensitivity is " << bestSensitivity << endl;
    cout << "Computed in " << t[1] - t[0] << "seconds" << endl << endl;
    
    return;
}


void PalindromeMasked(char* S){
    srand((unsigned)(time(0)));
    double time[2]; // measure real time
    time[0] = clock()/ 1000000.0;

    cout << "Generating " << "length of palindrome spaced seed " << l << " seed of weight " << w
    << " for similarity level " << p << " and length of homology region " << N << endl << endl;

    cout << "The program starts computing ..." << endl;
    cout << "If you reach a seed with your desired sensitivity you can kill the program... " << endl;
    Optimal_Palindrome_Spaced_Seed(S, w, l, tries, N, p, 0.0);
    time[1] = clock() / 1000000.0;
    cout << "Computed in " << time[1] - time[0] << " seconds" << endl;
    return ;
}



int main(int argc, char *argv[]){
    CommandLineParser parser;
    parser.addAuthorInformation("PalindromeMasked, author:Gary Zheng\nTo generate a highly sensitive palindrome spaced seed.");
    parser.addArgument('l', "Masked seeds length(default: 51)", "51");
    parser.addArgument('N', "Length of the homology region(default: 76)", "76");
    parser.addArgument('w', "Masked seeds weight for \"care\" positions(default: 31)", "31");
    parser.addArgument('p', "similarity level (default: 0.95)", "0.95");
    parser.addArgument('t', "Iterative tries(default: 5000)", "5000");


    parser.parse(argc, argv);
    l = stoi(parser.getArgument('l'));
    N = stoi(parser.getArgument('N'));
    p = stod(parser.getArgument('p'));
    w = stoi(parser.getArgument('w'));
    tries = stoi(parser.getArgument('t'));
    char* masked = new char [l + 1]; // the length of palindrome masked seed
    PalindromeMasked(masked);
    // char** S = new char*[1];
    // *S = new char[100];
    // cin >> *S;
    // double sensitivity = Sensitivity(S, 1, N, p);
    // cout << "Masked k-mer is:" << *S << endl;
    // cout << "Sensitivity:" << sensitivity << endl;
    // free(*S);
    // free(S);
    free(masked);

    
    return 0;
}
