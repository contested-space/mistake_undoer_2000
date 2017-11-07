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
	char ch;

	do{ 
		ch = fgetc(track);
		if (ch == '\n'){
			nbLine+=1;
		}
	}
	while (ch != EOF);
	fclose(track);
	track = fopen(argv[1], "r");


	//array[param][nodeligne] 0: time| 1: left sample | 2: right sample
	//double trackArray[nbLine][3];
	

	//double **trackArray= malloc(nbLine * sizeof *trackArray + (nbLine * (3 * sizeof **trackArray)));

	//trackArray[0][0] = 1.0;
	//trackArray[0][10] = 1;

	//double** trackarray;

	double* trackArrayTime;
	double* trackArrayLeftSample;
	double* trackArrayRightSample;

	trackArrayTime  = (double *)malloc(sizeof(double) * nbLine);
	trackArrayLeftSample  = (double *)malloc(sizeof(double) * nbLine);
	trackArrayRightSample  = (double *)malloc(sizeof(double) * nbLine);

	double* newtrackArrayTime;
	double* newtrackArrayLeftSample;
	double* newtrackArrayRightSample;

	newtrackArrayTime  = (double *)malloc(sizeof(double) * nbLine);
	newtrackArrayLeftSample  = (double *)malloc(sizeof(double) * nbLine);
	newtrackArrayRightSample  = (double *)malloc(sizeof(double) * nbLine);








	printf("nblignes: %d\n", nbLine);

	while (fgets(line, sizeof line, track) != NULL){
		trackArrayTime[nbSample] = atof(strtok (line, " \t"));
		trackArrayLeftSample[nbSample] = atof(strtok (NULL, " \t"));
		trackArrayRightSample[nbSample] = atof(strtok (NULL, " \t"));

		if (trackArrayLeftSample[nbSample] == 0 && trackArrayRightSample[nbSample] == 0){
			nbZero += 1;
		}
		else {
			nbOther += 1;
		}
		/*
		if (!(trackArrayLeftSample[nbSample] == 0 && trackArrayRightSample[nbSample] == 0)){
			//fprintf(newtrack, "%s\n", line, nbLine);
			fprintf(newtrack, "%g %g %g\n", trackArrayTime[nbSample], trackArrayLeftSample[nbSample], trackArrayRightSample[nbSample]);
			//putc('\n', newtrack);
			//nbLine += 1;
			//fputs("prout", newtrack);
			nbLine += 1;

		}
		*/
		nbSample +=1;
		//printf("Time : %f | Left Sample : %f | Right Sample :%f\n", time, leftSample, rightSample);
	}

	int i;
	int j;
	int fadeWidth = 5;
	for(i = 0; i < nbLine; i++){
		if (trackArrayLeftSample[i] == 0 && trackArrayRightSample[i] == 0){
			if((trackArrayLeftSample[i+1] == 0 && trackArrayRightSample[i+1] == 0) && !(trackArrayLeftSample[i-1] == 0 && trackArrayRightSample[i-1] == 0))
				for (j = i - fadeWidth; j <= i; j++){
				trackArrayLeftSample[j] *= (abs(i - j)/(float)fadeWidth);
				trackArrayRightSample[j] *= (abs(i - j)/(float)fadeWidth);
			}
		}
		if (trackArrayLeftSample[i] == 0 && trackArrayRightSample[i] == 0){
			if((trackArrayLeftSample[i-1] == 0 && trackArrayRightSample[i-1] == 0) && !(trackArrayLeftSample[i+1] == 0 && trackArrayRightSample[i+1] == 0))
				for (j = i; j <= i + fadeWidth; j++){
				trackArrayLeftSample[j] *= (abs(i - j)/(float)fadeWidth);
				trackArrayRightSample[j] *= (abs(i - j)/(float)fadeWidth);
			}
		}
	}

	for (i = 0; i < nbLine; i++){
		if (!(trackArrayLeftSample[i] == 0 && trackArrayRightSample[i] == 0)){
			fprintf(newtrack, "%g %g %g\n", trackArrayTime[i], trackArrayLeftSample[i], trackArrayRightSample[i]);
		}
	}


	/*
	while (fgets(line, sizeof line, track) != NULL){
		printf("at least this happens once\n");
		if (nbSample % 48000 == 0){
			printf("Sample %d atteint", nbSample);
		}
		
		trackArray[0][nbSample] = atof(strtok (line, " \t"));
		trackArray[1][nbSample] = atof(strtok (NULL, " \t"));
		trackArray[2][nbSample] = atof(strtok (NULL, " \t"));

		printf("%dst allocation is OK)\n", nbSample);

		if (trackArray[1][nbSample] == 0 && trackArray[2][nbSample] == 0){
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
		
		nbSample +=1;
		//printf("Time : %f | Left Sample : %f | Right Sample :%f\n", time, leftSample, rightSample);
	}
	*/
	
	printf("nbSample = %d | nbZero = %d | nbOther = %d\n", nbSample, nbZero, nbOther);
	printf ("ratio 48/44.1 = %f | ratio autre/zero = %f\n", 48.0/44.1, (float)nbSample/(float)nbOther);
	fclose(track);
	fclose(newtrack);


	return 0;

}

