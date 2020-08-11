#include<stdio.h>

#define ELEM_COUNT 6

void menu(double[]);
// Compute the rate of heat transfer
double calc_h(double[]);
// Coefficient of thermal conductivity
double calc_k(double[]);
// Cross-sectional Area
double calc_a(double[]);
// Thickness
double calc_thick(double[]);
// Temp
double calc_temp(double[]);

enum values{
	heat_trans, coeff, cs_area, temp_1, temp_2, thickness
} conduction;
const char *names[] = {"Heat transfer", "Coefficient", "Cross-section Area",
		       "Side 1 Temp", "Side 2 Temp", "Thickness"};

int main() {
	int name;	
	double result;
	double vals[ELEM_COUNT];
	double (*funcs[ELEM_COUNT]) (double[]) = {calc_h, calc_k, calc_a, calc_thick, calc_temp};
	menu(vals);
	for(int i = 0; i < ELEM_COUNT; i++){
		if (vals[i] <= 0.0){
			result = (*funcs[i])(vals);
			name = i;
		}
	}

	printf("%s is %lf\n", names[name], result);

	return 0;
}

void menu(double values[]){
	printf("Rate of heat transfer (watts) >> ");
	scanf("%lf", &values[heat_trans]);
	printf("Coefficient of thermal conductivity (W/m-k) >> ");
	scanf("%lf", &values[coeff]);
	printf("Cross-sectional area of conductor (m^2) >> ");
	scanf("%lf", &values[cs_area]);
	printf("Temperature on one side (K) >> ");
	scanf("%lf", &values[temp_1]);
	printf("Temperature on other side (K) >> ");
	scanf("%lf", &values[temp_2]);
	printf("Thickness of conductor (m) >> ");
	scanf("%lf", &values[thickness]);
}

double calc_thick(double values[]){
	return values[thickness];
}

double calc_h(double values[]){}
double calc_k(double values[]){}
double calc_a(double values[]){}
double calc_temp(double values[]){}
