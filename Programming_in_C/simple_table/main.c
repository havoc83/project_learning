#include <stdio.h>
#include <string.h>

#define DEBUG 1
// Functional prototype
int menu(void);
int ops_menu(void);
void operations(int, FILE*, char*);
void select(FILE*);
FILE *create_table(char*);
FILE *select_table(char*);

int main(){
	int main_selection;
	int ops_selection = 1;
	char file_name[50];
	FILE* table_file = NULL;
	do {
		main_selection = menu();
		if (DEBUG){
			printf("DEBUG: Main selection is %d\n", main_selection);
		}
		if (main_selection == -99) {
			return 0;
		}
	} while (main_selection < 1 || main_selection > 2);

	if (main_selection == 1){
		table_file = create_table(file_name);
	} else {
		table_file = select_table(file_name);
	}

	while (ops_selection > 0) {
		ops_selection = ops_menu();
		operations(ops_selection, table_file, file_name);
	}

	fclose(table_file);
	return 0;
}


void operations(int op, FILE *table, char* file_name){
	int del;
	printf("%d was the option selected\n", op);
	switch (op) {
		case 1: 
			select(table);
			break;
		case 2:
			printf("Deleting file %s\n", file_name);
			del = remove(file_name);
			if (!del){
				puts("Delete was successful");
			} else {
				puts("Delete was not successful");
			}
			break;
		default:
			break;	
	
	}
}

void select(FILE *table){
	char buff[255];
	fseek(table, 0, SEEK_SET);
	while ( fgets(buff, 255, table) != NULL ) {
		printf("%s", buff);
	}
}

int menu(void){
	int main_selection;
	puts("Enter in the number of the action you would like:");
	puts("[1] Create table");
	puts("[2] Use existing table");
	puts("[-99] Exit");
	scanf("%d", &main_selection);
	return main_selection;
}

int ops_menu(void){
	int ops_selection;
	puts("Enter in the operation you wish to perform:");
	puts("[1] Select data");
	puts("[2] Drop table");
	puts("[-99] Exit");
	scanf("%d", &ops_selection);
	return ops_selection;
}

FILE *create_table(char* file_path){
	FILE* table_file;
	char table_name[32];
	char field[32];
	puts("Enter the name of the table you would like to create (max 32 characters):");
	scanf(" %32s", table_name);
	sprintf(file_path, "./tables/%s.txt", table_name);
	table_file=fopen(file_path, "w+");
	fprintf(table_file, "id|");
	for (int i=0; i<20; i++ ){
		puts("Enter in the column name (-99 after all columns are entered):");
		scanf(" %s", field);
		if (strcmp(field, "-99") == 0) {
			break;
		}
		fprintf(table_file, "%s|", field);
	}
	fprintf(table_file, "\n");
	return table_file;
}

FILE *select_table(char* file_path) {
	FILE* table_file;
	char table_name[32];
	puts("Enter the name of the table you would like to work on.");
	scanf(" %32s", table_name);
	sprintf(file_path, "./tables/%s.txt", table_name);
	table_file=fopen(file_path, "r+");
	return table_file;

}
