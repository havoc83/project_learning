#include<stdio.h>
// preprocessor directives
#define MAX_POINTS 20
#define INPUT_FILE "./points.txt"

// Function definitions
int    get_corners(FILE *input, double x[], double y[]);
double polygon_area(double x[], double y[]);

int main() {
	double x[MAX_POINTS], y[MAX_POINTS];
	double area;
	int corners;
	FILE *input, *output;
	input = fopen(INPUT_FILE, "r");
	if (input == NULL){
		printf("Could not open file %s\n", INPUT_FILE);
		return 1;
	}
	corners = get_corners(input, x, y);
	fclose(input);
	printf("Number of corners %d\n", corners);
	area = polygon_area(x,y);
	printf("Area of polygon %lf\n", area);
	return 0;
}

int get_corners(FILE *input, double x[], double y[]) {
	int i = 0;
	while(fscanf(input,"%lf %lf", &x[i], &y[i]) != EOF){
		i++;
		if(i >= MAX_POINTS){
			break;
		}
	}
	return i;
}

double polygon_area(double x[], double y[]) {
	double sum; 
	int j = MAX_POINTS;
	for(int i = MAX_POINTS - 1;i >= 0;i--){
		sum += (x[j] * y[i]) - (y[j] * x[i]);
		j--;
	}
	if(sum < 0){
		return (sum / 2) * -1;
	} else {
		return (sum / 2);
	}
}
