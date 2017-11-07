#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main( int argc, char *argv[] ){
	FILE* track = fopen(argv[1], "r");
	char line[256];
	int len = 0;
	double split;
	double time;
	double leftSample;
	double rightSample;
	int nbZero = 0;
	int nbOther = 0;
	int nbSample = 0;
	double lastLeft = 0;
	double lastRight = 0;
	int nbDoubles = 0;

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
		if(leftSample == lastLeft && rightSample == lastRight && leftSample!=0 && rightSample!=0){
			nbDoubles++;
		}
		lastLeft = leftSample;
		lastRight = rightSample;
		//printf("Time : %f | Left Sample : %f | Right Sample :%f\n", time, leftSample, rightSample);
	}
	printf("nbSample = %d | nbZero = %d | nbOther = %d\n", nbSample, nbZero, nbOther);
	printf ("ratio 48/44.1 = %f | ratio autre/zero = %f\n", 48.0/44.1, (float)nbSample/(float)nbOther);
	printf("nombre de doublons: %d\n", nbDoubles);
	printf("ratio doublons + zeros: %f\n", (float)nbSample/(float)(nbOther-nbDoubles));
	fclose(track);


	return 0;

}

