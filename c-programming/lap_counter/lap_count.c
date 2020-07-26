/* This application is used to give a 
 * proper distance in miles based on the number of 
 * laps swam
 */

#include <stdio.h>
#include <stdlib.h>

#define FT_PER_MILE 5280
#define LENGTH_OF_POOL 48

int main(int argc, char *argv[]) {
	float laps,
	      lcount,  
	      lfeet;
	float total_miles;

	if( argc < 2) {
		printf("Please enter in the number of laps swam.");
		exit(1);
	} 
	for(char **arg = argv; *arg; ++arg){
		sscanf(*arg, "%f", &lcount);
		laps = laps + lcount;
	} 
	printf("The number of laps swam: %.0f\n", laps);
	lfeet = laps * LENGTH_OF_POOL;
	total_miles = lfeet / (float)FT_PER_MILE;
	printf("The number of feet swam: %.0f\n", lfeet);
	printf("Total miles swam: %.2f\n", total_miles);
	exit(0);

}

