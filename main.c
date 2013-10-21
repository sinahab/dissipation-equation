
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

void initialise(int *, double *, double *, double *, double *);
void initialconditions(double *, int, double, double);

void tridiag(double *, double *, double *, double *, int);
void initmatrix(double *, double *, double *, int, double);

void output(FILE *, double*, int, double);

int main() {

	// variable to distinguish between the three methods
	// method = 1: forward difference (explicit) method
	// method = 2: backward difference (implicit) method
	// method = 3: Crank-Nicholson method 
	int method;

	FILE *output_file;

	// declarations of variables
	double step;
	double delta_t, final_t, t;
	double T_left, T_right;
	int N_grid, i;

	int printinterval, counter;
	double *bar, *barout, *l, *c, *r;


	// read in input data from screen
	initialise(&N_grid, &delta_t, &final_t, &T_left, &T_right);
	// N_grid is the number of grid points on the bar including the endpoints

	for (method=1; method<4; method++) {
	// doing the calculations for all three methods

		printf("starting next method calculations.\n");

		// opening the output file
		if (method==1)
			output_file = fopen("bar-explicit_deltaxE-1_deltatE-3", "w");
		else if (method==2)
			output_file = fopen("bar-implicit_deltaxE-1_deltatE-3", "w");
		else if (method==3)
			output_file = fopen("bar-crank-nicholson_deltaxE-1_deltatE-3", "w");

		fprintf(output_file, "##bar at 100 C with endpoints at %lf and %lf with %d gridpoints and delta_t = %lf \n\n", T_left, T_right, N_grid, delta_t);

		bar = malloc(N_grid*sizeof(double));
		barout = malloc(N_grid*sizeof(double));

		initialconditions(bar, N_grid, T_left, T_right);

		barout[0] = bar[0];
		barout[N_grid-1]=bar[N_grid-1];
		t=0;

		output(output_file, bar, N_grid, t);

		step = delta_t*(N_grid-1)*(N_grid-1);
		if (method==3)
			step = (step/2);
		printinterval = (int) floor(final_t/delta_t/10);

		l = (double *)malloc((N_grid)*sizeof(double));
		c = (double *)malloc((N_grid)*sizeof(double));
		r = (double *)malloc((N_grid)*sizeof(double));

		counter=0;

		while(t<final_t) {
			for(i=1; i<N_grid-1; i++)
				if (method==1)
					barout[i] = bar[i] + step*( bar[i+1]+bar[i-1]-2*bar[i] );
				else if (method==2){
					initmatrix(l,c,r,N_grid,step);
					tridiag(l,c,r,bar,N_grid);
				}
				else if (method==3){
					barout[i] = bar[i] + step*( bar[i+1]+bar[i-1]-2*bar[i] );
					initmatrix(l,c,r,N_grid,step);
					tridiag(l,c,r,barout,N_grid);
				}
			t+=delta_t;
			counter++;
			if(counter==printinterval){
				if (method==1)
					output(output_file, barout, N_grid, t);
				else if (method==2)
					output(output_file, bar, N_grid, t);
				else if (method==3)
					output(output_file, barout, N_grid, t);
				counter=0;
			}
			for(i=1; i<N_grid-1; i++)
				bar[i]=barout[i];
		}

		fclose(output_file);
		free(bar);
		free(barout);

		free(l);
		free(c);
		free(r);

	}

	return 0;
} // end main program

void initialise(int *N_grid, double *delta_t, double *final_t, double *T_left, double *T_right) {

	printf("Read in fron screen the grid size, delta_t, final time, left temperature and right temperature\n");
	scanf("%d %lf %lf %lf %lf", N_grid, delta_t, final_t, T_left, T_right);
	return;

} // end of function initialise.

void initialconditions(double *bar, int N_grid, double T_left, double T_right) {

	int i;

	bar[0]=T_left;
	bar[N_grid-1]=T_right;
	for(i=1; i<N_grid-1; i++)
		bar[i]=20.0;
	return;
}

void tridiag(double *l, double *c, double *r, double *unew, int N_grid) {

	int i;
	
	for(i=1; i<N_grid-1; i++){
		c[i]=c[i]-l[i]*r[i-1];
		unew[i]=(unew[i]-l[i]*unew[i-1])/c[i];
		r[i]=r[i]/c[i];
	}
	for(i=N_grid-1; i>1; i--){
		unew[i-1]=-r[i-1]*unew[i]+unew[i-1];
	}

	return;
}

void initmatrix(double *l, double *c, double *r, int N_grid, double alpha){

	int i;

	for(i=1; i<N_grid; i++){
		l[i]=-alpha;
		c[i]=1+2*alpha;
		r[i]=-alpha;
	}

	l[0]=0.0;
	l[N_grid-1]=0.0;
	r[0]=0.0;
	r[N_grid-1]=0.0;

	return;
}

void output(FILE *output_file, double *barout, int N_grid, double t) {

	int i;
	fprintf(output_file, "## time %12.10E \n", t);

	for(i=0; i<N_grid; i++)
		fprintf(output_file, "%d \t %12.10E \n", i, barout[i]);
	
	fprintf(output_file, "## \n\n");

	return;
} // end of function output

