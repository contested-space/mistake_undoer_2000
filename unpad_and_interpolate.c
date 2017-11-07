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

	//pour avoir le nombre total de samples (initialement)
	
	do{ 
		ch = fgetc(track);
		if (ch == '\n'){
			nbLine+=1;
		}
	}
	while (ch != EOF);
	fclose(track);
	track = fopen(argv[1], "r");
	
	//allocation de la memoire pour les samples

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

	double* noDoubleArrayTime;
	double* noDoubleArrayLeftSample;
	double* noDoubleArrayRightSample;

	noDoubleArrayTime  = (double *)malloc(sizeof(double) * nbLine);
	noDoubleArrayLeftSample  = (double *)malloc(sizeof(double) * nbLine);
	noDoubleArrayRightSample  = (double *)malloc(sizeof(double) * nbLine);

	printf("nblignes: %d\n", nbLine);

	//lecture du fichier ligne par ligne et sauvegarde des donnees dans les tables

	
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
		nbSample +=1;
	}
	

	int i;
	int j;
	
	i=0;
	j=0;
	int k = 0;
	int zeroes[1000000];
	int nbInterpol = 0;

	int interpolWidth = 5; //nombre de samples insérés dans les discontinuités



	double lastLeft = 0;
	double lastRight = 0;


	//retire tous les doublons (peu probable que deux samples successifs aient la meme valeur)

	/*
	while(i<nbLine){
		if (!(trackArrayLeftSample[i] == lastLeft || trackArrayRightSample[i] == lastRight)){
			noDoubleArrayTime[j] = trackArrayTime[i];
			noDoubleArrayLeftSample[j] = trackArrayLeftSample[i];
			noDoubleArrayRightSample[j] = trackArrayRightSample[i];
			lastLeft = trackArrayLeftSample[i];
			lastRight = trackArrayRightSample[i];
			i++;
			j++;
		}
		i++;
	}
	*/

	i = 0;
	j = 0;

	//nbLine = 10;
	while (i<nbLine){
		//printf("TIME: %f | LEFT: %f | RIGHT: %f\n", trackArrayTime[i], trackArrayLeftSample[i], trackArrayRightSample[i]);
		//si les samples sont non-zero, copie et avance dans les deux tableaux
		if (!(trackArrayLeftSample[i] == 0 || trackArrayRightSample[i] == 0)){
			newtrackArrayTime[j] = trackArrayTime[i];
			newtrackArrayLeftSample[j] = trackArrayLeftSample[i];
			newtrackArrayRightSample[j] = trackArrayRightSample[i];
			i++;
			j++;
		}
		else {
			//note l'indice du premier zero pour le retrouver au moment de l'interpolation
			zeroes[nbInterpol] = j;
			nbInterpol++;

			//ajoute un nombre de zeros egal a la largeur d'interpolation
			//et avance l'indice du tableau de sortie
			for(k=0;k<interpolWidth;k++){
				//printf("doin something at line %d\n", j);
				newtrackArrayTime[j] = 0; //pas propre, a arranger (sans consequence, SoX recalcule le temps en fonction du SR)
				newtrackArrayLeftSample[j] = 0;
				newtrackArrayRightSample[j] = 0;
				//printf("pre-interpol padding: i: %d j:%d k:%d| left:%f | right:%f\n", i, j, k, trackArrayLeftSample[i], trackArrayRightSample[i]);
				j++;
			}
			//avance l'indice du tableau initial pour sauter par dessus tous les zeros
			while (trackArrayLeftSample[i] == 0 || trackArrayRightSample[i] == 0){
				i++;
			}

		}

	}
	int newArraySize = j;

	int firstInterpol = 1;
	
	printf("nombre d'interpolations de %d samples: %d\n",interpolWidth, nbInterpol);
	void interpolateLinear(double* array, int index, int width){
		int i;
		double begin = array[index - 1];
		double end = array [index + width];
		//debug
		if(firstInterpol){
			printf("INTERPOL Begin: %f\n", begin);
		}
		for (i = 0; i < width; i++){
			array[index + i] = (begin + (i + 1)*(end - begin)/(width+1));
			//debug
			if (firstInterpol){
				printf("INTERPOL: %f\n", array[index + i]);
			}
		}

		if(firstInterpol){

			printf("INTERPOL END: %f\n", end);

			firstInterpol = 0;
		}


	}
	
	for (i = 0; i< nbInterpol; i++){
		if (zeroes[i] + interpolWidth < newArraySize){
			interpolateLinear(newtrackArrayLeftSample, zeroes[i], interpolWidth);
			interpolateLinear(newtrackArrayRightSample, zeroes[i], interpolWidth);
		}
	}
	
	printf("First interpolated line: %d\n", zeroes[0]);

	/*
	double test[10];
	for(i = 0; i< 10; i++){
		test[i]=0;
	}
	test[0] = -1;
	test[9] = 1;

	interpolateLinear(test, 1, 1);
	for (i=0; i< 10; i++){
		printf("Test: %f\n", test[i]);

	}
	*/

	
	//copie des samples traités dans un autre fichier
	for (i = 0; i < newArraySize; i++){
		fprintf(newtrack, "%g %g %g\n", newtrackArrayTime[i], newtrackArrayLeftSample[i], newtrackArrayRightSample[i]);
	}
	
	printf("nbSample = %d | nbZero = %d | nbOther = %d\n", nbSample, nbZero, nbOther);
	printf ("ratio 48/44.1 = %f | ratio autre/zero = %f\n", 48.0/44.1, (float)nbSample/(float)nbOther);
	fclose(track);
	fclose(newtrack);


	return 0;

}

