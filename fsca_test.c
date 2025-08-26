#include <stdio.h>
#include <math.h>
#include <stdint.h>

#define FSCA_MAX 32767

int main(){
	
	int deg;
	double total_error = 0;
	double max_error   = 0;
	int max_error_deg  = 0;

	for(deg = 0; deg <= 360; deg++){
	    double exact = (deg * FSCA_MAX) / 360.0;
	    int32_t approx =  (deg * 23301 + 128) >> 8; 
	    double error = fabs(approx - exact);
	    total_error += error;

	        if(error > max_error){
		   max_error = error;
		   max_error_deg = deg;
			
		}
	}


	double avg_error = total_error / 361; 

	printf("testing deg_to_fsca approximation\n");
	printf("max error: %f at %d degrees\n", max_error, max_error_deg);
	printf("average error: %f\n", avg_error);

	return 0;
}
