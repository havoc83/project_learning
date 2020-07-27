#include <stdio.h>
#include <string.h>

// Function proto
double compute_bmi(double height, double weight);
void prompts(double *height_ft, double *height_in,  double *weight);
void display_bmi(double bmi);

int main(){
	double height_ft, height_in, total_height, weight, bmi;
	prompts(&height_ft, &height_in, &weight);
	total_height = (height_ft * 12) + height_in;	
	printf("Your height is %lf inches and your weight is %lf lbs\n", total_height, weight);
	bmi = compute_bmi(total_height, weight);
	printf("Your bmi is %lf\n", bmi);
	display_bmi(bmi);
}

void prompts(double *height_ft, double *height_in, double *weight) {
	char known_bp;
	int sbp;
	printf("Do you know you blood pressure (y,n)?\n");
	scanf(" %c", &known_bp);
	if (known_bp == 'y' || known_bp == 'Y'){
		printf("Please enter your blood pressure: ");
		scanf("%d", &sbp);
		if (sbp < 120) {
			printf("Your blood pressure is normal.\n");
		} else if (sbp < 140) {
			printf("Your blood pressure is Pre-hypertension.\n");
		} else {
			printf("WARNING, your blood pressure is clasified as Hypertension.\n");
		}
	}
	printf("Enter in your height (feet and inches for instance 5 10).\n");
	scanf("%lf %lf", height_ft, height_in);
	printf("Enter in your weight (in pounds).\n");
	scanf("%lf", weight);
}

double compute_bmi(double height, double weight){
	return (703 * weight) / (height * height);
}

void display_bmi(double bmi){
	if (bmi < 18.5) {
		printf("You are underweight.\n");
	} else if (bmi < 25) {
		printf("You are in the normal weight status.\n");	
	} else if (bmi < 30) {
		printf("You are in the overweight status.\n");
	} else {
		printf("You are in the obese status.\n");
	}
}
