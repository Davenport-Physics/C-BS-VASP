#include <stdio.h>
#include <malloc.h>
#include <string.h>

void initialize();
void ParseArgs(int argc, char *argv[]);
void ReadData(int iteration);
void Transpose(int iteration);

typedef enum Verbose {

	FALSE = 0,
	TRUE

} Verbose;

static char *Filename 			= "EIGENVAL";
static char OutputFilename[128] = "upBand";

static float *FirstColumn;
static float *SecondColumn;

static int NumberOfFields   = 0;
static int NumberOfSections = 0;

static Verbose Debug = TRUE;
static Verbose Tabs  = FALSE;

static FILE *fp;

int main(int argc, char *argv[]) {

	initialize();
	ParseArgs(argc, argv);

	int x;
	for (x = 0;x < NumberOfSections;x++) {
	
		ReadData((x + 1));
		Transpose((x + 1));
	
	}

	free(FirstColumn);
	free(SecondColumn);

	fclose(fp);

	return 0;
}

void ParseArgs(int argc, char *argv[]) {

	int x;
	for (x = 1;x < argc;x++) {
	
		if (strcasecmp(argv[x], "-tabs") == 0) {
		
			Tabs = TRUE;
			
		} else if (strcasecmp(argv[x], "-output-file") == 0) {
		
			if ((x+1) == argc) {
				
				printf("Did not pass output file name.\n");
				
			} else {
			
				strncpy(OutputFilename, argv[x+1], 128);
				
			}
			
		}
		
	}
	
}

void initialize() {

	fp = fopen(Filename, "r");

	char Buffer[256];

	int x;
	for (x = 0;x < 5;x++)
		fgets(Buffer, 256, fp);
	fscanf(fp, "%*d%d%d\n", &NumberOfSections, &NumberOfFields);	

	if (Debug == TRUE)
		printf("NumberOfSections = %d, NumberOfFields = %d\n", NumberOfSections, NumberOfFields);	

	FirstColumn  = malloc(NumberOfFields * sizeof(float));
	SecondColumn = malloc(NumberOfFields * sizeof(float)); 

}

void ReadData(int iteration) {

	char Buffer[256];
	
	/*
	 * This conditional statement fixes a bug that I do not want
	 * to spend time diagnosing properly.
	 * 
	 * */
	if (iteration == 1) {
	
		fgets(Buffer, 256, fp);
		
	} else {
		
		fgets(Buffer, 256, fp);
		fgets(Buffer, 256, fp);
		fgets(Buffer, 256, fp);
		
	}
	
	int x;
	for (x = 0;x < NumberOfFields;x++) {

		fscanf(fp, "%*d%f%f", &FirstColumn[x], &SecondColumn[x]);

	}

}

void Transpose(int iteration) {

	FILE *upBand = fopen(OutputFilename, "a");
	
	if (Tabs == FALSE) {
		
		fprintf(upBand, "%d,", iteration);
		
	} else {
	
		fprintf(upBand, "%d\t", iteration);
		
	}
	
	int x;
	for (x = 0;x < NumberOfFields;x++) {

		if (Tabs == FALSE) {

			if ((x + 1) == NumberOfFields)
				fprintf(upBand, "%.4f", FirstColumn[x]);
			else
				fprintf(upBand, "%.4f,", FirstColumn[x]);
		
		} else {
		
			if ((x + 1) == NumberOfFields)
				fprintf(upBand, "%.4f", FirstColumn[x]);
			else
				fprintf(upBand, "%.4f\t", FirstColumn[x]);
			
		}

	}
	fprintf(upBand, "\n");


	fclose(upBand);

}

