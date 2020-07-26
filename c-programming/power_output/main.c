#include<stdio.h>
#include<string.h>

#define CUBIC_WATER_MASS 1000
#define GRAVITATIONAL_CONST 9.8
int main(void) {
	char source[7];
	double hydro_source(void);
	puts("Enter generator source [hydro, nuclear, solar]:");
	scanf(" %7s", source);
	if (strcmp(source, "nuclear") == 0 || strcmp(source, "solar") == 0){
		puts("Sorry that source has not been implimented yet.\n");
		return 0;
	} else if(strcmp(source, "hydro") == 0){
		double megawatts = hydro_source();
		printf("MegaWatts Produced: %.2lfMW\n", megawatts);
	} else{
		puts("That is not a valid source.");
		return 1;
	}

	return 0;
}

double hydro_source(void) {
	int height;
	double flowrate;
	double work;
	puts("Enter height in meters.");
	scanf("%i", &height);
	puts("Enter cubic meters of water per second:");
	scanf("%lf", &flowrate);
	work = flowrate * CUBIC_WATER_MASS * GRAVITATIONAL_CONST * height;
	return work / 1000000.0;	
}
