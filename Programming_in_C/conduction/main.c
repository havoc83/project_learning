#include<stdio.h>

#define ELEM_COUNT 6

int menu(double[]);
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
double calc_tempb(double[]);
enum values{
	heat_trans, coeff, cs_area, temp_1, temp_2, thickness
} conduction;
const char *names[] = {"Heat transfer", "Coefficient", "Cross-section Area",
		       "Side 1 Temp", "Side 2 Temp", "Thickness"};

double (*funcs[]) (double[]) = {calc_h, calc_k, calc_a, calc_temp, calc_tempb, calc_thick};

int main() {
	int name;
	double result;
	double vals[ELEM_COUNT];
	name = menu(vals);
	result = (*funcs[name])(vals);
	printf("%s is %lf\n", names[name], result);
	return 0;
}

int menu(double values[]){
	char q_mark;
	int find;
	printf("Rate of heat transfer (watts) >> ");
	if (scanf("%lf", &values[heat_trans]) == 0){
		scanf(" %s", &q_mark);
		find = 0;
	}
	printf("Coefficient of thermal conductivity (W/m-k) >> ");
	if (scanf("%lf", &values[coeff]) == 0){
		scanf(" %s", &q_mark);
		find = 1;
	}
	printf("Cross-sectional area of conductor (m^2) >> ");
	if (scanf("%lf", &values[cs_area]) == 0){
		scanf(" %s", &q_mark);
		find = 2;	
	}
	printf("Temperature on one side (K) >> ");
	if (scanf("%lf", &values[temp_1]) == 0){
		scanf(" %s", &q_mark);
		find = 3;
	}
	printf("Temperature on other side (K) >> ");
	if (scanf("%lf", &values[temp_2]) == 0) {
		scanf(" %s", &q_mark);
		if (values[temp_2] < values[temp_1]){
			double tmp = values[temp_2];
			values[temp_2] = values[temp_1];
			values[temp_1] = tmp;
		}
		find = 4;
	}
	printf("Thickness of conductor (m) >> ");
	if (scanf("%lf", &values[thickness]) == 0){
		scanf(" %s", &q_mark);
		find = 5;
	}
	return find;
}

double calc_thick(double values[]){
	printf("Computing material thickness\n");
	double top;
	top = values[coeff] * values[cs_area] * (values[temp_2] - values[temp_1]);
	printf("Top value %lf\n", top);
	return values[heat_trans] * top;
}

double calc_h(double values[]){
	double top;
	top = values[coeff] * values[cs_area] * (values[temp_2] - values[temp_1]);
	return top / values[thickness];
}

double calc_k(double values[]){
	double bottom;
	bottom = values[cs_area] * (values[temp_2] - values[temp_1]);
	return (values[heat_trans] * values[thickness]) / bottom;
}
double calc_a(double values[]){
	double bottom;
	bottom = values[coeff] * (values[temp_2] - values[temp_1]);
	return (values[heat_trans] * values[thickness]) / bottom;
}
double calc_temp(double values[]){
 	double bottom, top;
	bottom = values[coeff] * values[cs_area];
	top = values[heat_trans] * values[thickness];
	return values[temp_1] - top/bottom;
}

double calc_tempb(double values[]){
	double bottom, top;
	bottom = values[coeff] * values[cs_area];
	top = values[heat_trans] * values[thickness];
	return values[temp_2] - top/bottom;
}
