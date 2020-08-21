#include<stdio.h>
#include<stdbool.h>

#define MAX_DENOM 99.0



int main(){
	double equal, pi, total;
	total = 1;
	bool neg = true;
	for(int i=3.0;i<=MAX_DENOM;i++){
		equal = 1.0 / i;
		if (neg){
			total -= equal;
			neg = false;
		} else {
			total += equal;
			neg = true;
		}
	}
	pi = 4 * total;
	printf("The approximation of PI is %lf.\n", pi);
	return 0;
}
