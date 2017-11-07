#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main( int argc, char *argv[] ){
	FILE* track = fopen(argv[1], "r");
	FILE* newtrack = fopen ("temp_no_header.dat", "w+");
	char line[256];
	int len = 0;
	double split;
	double time;
	double leftSample;
	double rightSample;
	int nbZero = 0;
	int nbOther = 0;
	int nbSample = 0;
	int nbLine = 0;

	while (fgets(line, sizeof line, track) != NULL){
		nbSample +=1;
		split = atof(strtok (line, " \t"));
		time = split;
		split = atof(strtok (NULL, " \t"));
		leftSample = split;
		split = atof(strtok (NULL, " \t"));
		rightSample = split;
		if (leftSample == 0 && rightSample == 0){
			nbZero += 1;
		}
		else {
			nbOther += 1;
		}
		if (!(leftSample == 0 && rightSample == 0)){
			//fprintf(newtrack, "%s\n", line, nbLine);
			fprintf(newtrack, "%g %g %g\n", (double)nbLine/44100.0, leftSample, rightSample);
			//putc('\n', newtrack);
			//nbLine += 1;
			//fputs("prout", newtrack);
			nbLine += 1;

		}
		//printf("Time : %f | Left Sample : %f | Right Sample :%f\n", time, leftSample, rightSample);
	}
	printf("nbSample = %d | nbZero = %d | nbOther = %d\n", nbSample, nbZero, nbOther);
	printf ("ratio 48/44.1 = %f | ratio autre/zero = %f\n", 48.0/44.1, (float)nbSample/(float)nbOther);
	fclose(track);
	fclose(newtrack);


	return 0;

}

