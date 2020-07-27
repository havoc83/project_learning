#include<stdio.h>
#include<string.h>

#define CUBIC_WATER_MASS 1000
#define GRAVITATIONAL_CONST 9.8
#define SOLAR_VARIABILITY .75
// Functional prototype
double solar_source(void);
double hydro_source(void);

int main(void) {
	char source[7];
	puts("Enter generator source [hydro, nuclear, solar]:");
	scanf(" %7s", source);
	if (strcmp(source, "nuclear") == 0){
		puts("Sorry that source has not been implimented yet.\n");
		return 0;
	} else if(strcmp(source, "solar") == 0 ){
		double kilowatts = solar_source();
		printf("KiloWatts produced %.2lfKWh\n", kilowatts);
	} else if(strcmp(source, "hydro") == 0){
		double megawatts = hydro_source();
		printf("MegaWatts Produced: %.2lfMW\n", megawatts);
	} else{
		puts("That is not a valid source.");
		return 1;
	}

	return 0;
}
/* This function computes the total power output of solar panels */
double solar_source(void) {
	int number_of_panels;
	double panel_watt_rating;
	double hours_per_day;
	puts("Enter the total number of Solar Panels:");
	scanf("%i", &number_of_panels);
	puts("Enter the weighted average watt rating");
	scanf("%lf", &panel_watt_rating);
	puts("Enter average number of hours of sunlight per day.");
	scanf("%lf", &hours_per_day);
	double watt_hours =  number_of_panels * panel_watt_rating * hours_per_day * SOLAR_VARIABILITY;
	return watt_hours / 1000.0;

}

/* This function holds the logic for computing hydro dam power output in megawatts*/
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
