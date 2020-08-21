#include<stdio.h>

#define DENSITY 1.23
#define MAX_VELOCITY 40
#define VELOCITY_STEP 5

void menu(double *coef, double *area);
double drag_force(double *coef, double *area, int velocity);

int main(){
	double coef, area, force;
	menu(&coef, &area);
	for (int velocity = 0;velocity <= MAX_VELOCITY;velocity += VELOCITY_STEP){
		force = drag_force(&coef, &area, velocity);
		printf("The drag force at %d m/s is %lf\n", velocity, force);
	}
	return 0;
}

void menu(double *coef, double *area){
	printf("Enter the drag coefficient >> ");
	scanf("%lf", coef);
	printf("Enter the projected area >> ");
	scanf("%lf", area);
}

double drag_force(double *coef, double *area, int velocity) {
	return (0.5 * *coef) * *area * DENSITY * velocity;
}
