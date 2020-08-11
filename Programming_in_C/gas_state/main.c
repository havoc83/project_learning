#include <stdio.h>

#define ATM_PER_MOLE .08206
typedef enum 
{ helium, neon, hydrogen, carbon_dioxide, water_vapor }
gas_t;

void menu(int*, int*, int*, int*, int*, float*);

int main(void) {
	gas_t gas_type;
	int gas, temp, init_vol, final_vol, incr;
	float moles;
	double a, b, nrt, vnb, a_mol, p1, p2, liters;
	char* gas_name;
	menu(&gas, &temp, &init_vol, &final_vol, &incr, &moles);
	gas_type = gas;
	switch(gas_type) {
		case helium:
			gas_name = "helium";
			a = 3.46e-3;
			b = 23.71e-6;
			break;
		case neon:
			gas_name = "neon";
			a = 2.12e-2;
			b = 17.10e-6;
			break;
		case hydrogen:
			gas_name = "hydrogen";
			a = 2.45e-2;
			b = 26.61e-6;
			break;
		case carbon_dioxide:
			gas_name = "carbon dioxide";
			a = 3.592;
			b = 0.0427;
			break;
		case water_vapor:
			gas_name = "water vapor";
			a = 5.47e-1;
			b = 30.52e-6;
			break;
		default:
			return 1;
	}
	printf("%f moles of %s at %d kelvin\n\n", moles, gas_name, temp);
	printf("Volume (ml)\t\tPressure (atm)\n\n");
	nrt = moles * ATM_PER_MOLE * temp;
	a_mol = a * (moles * moles);
	for (int i = init_vol; i <= final_vol; i += incr){
		liters = (double)i/1000.0;
		vnb = liters - (moles * b);
		p1 = nrt / vnb;
		p2 = a_mol / (liters * liters);
		printf("  %d\t\t\t%.4lf\n", i, p1-p2);
	
	}	
	return 0;
}

void menu(int* gas, int* temp, int* init_vol, int* final_vol, int* incr, float* moles){
	puts("Number of gas ([0] helium, [1] neon, [2] hydrogen, [3] carbon_dioxide, [4] water_vapor)");
	scanf("%d", gas);
	printf("Quantity of carbon dioxide (moles) > ");
	scanf("%f", moles);
	printf("Temperature (kelvin) > ");
	scanf("%d", temp);
	printf("Initial Volume (milliliters) > ");
	scanf("%d", init_vol);
	printf("Final Volume (milliliters) > ");
	scanf("%d", final_vol);
	printf("Volume increment (milliliters) > ");
	scanf("%d", incr);
}


