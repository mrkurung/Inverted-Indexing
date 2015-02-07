#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <glib.h>

GSList* list;

void looklist(char* word);

int main (int c, char *v[]) {
	int len;
	struct dirent *pDirent;
	DIR *pDir;

	if (c < 2) {
		printf ("Usage: testprog <dirname>\n");
		return 1;
	}
	pDir = opendir (v[1]);
	if (pDir == NULL) {
		printf ("Cannot open directory '%s'\n", v[1]);
		return 1;
	}

	while ((pDirent = readdir(pDir)) != NULL) {
        //---Open file ---//
		char f_path[24] ;
		sprintf(f_path,"%s/%s",v[1],pDirent->d_name);
		FILE* ifile = fopen(f_path,"r");

		while(1){
			//read a word from file
			char * word = malloc(sizeof(char)*32);	//allocate memory
			c = fscanf(ifile,"%32[^A-Za-z]",word);		//read not alphabet to move cursor
			c = fscanf(ifile,"%32[A-Za-z]",word);	// got one word from the file	//
			if(c == EOF)break;

			//to Lower
			int j;
			for( j= 0; word[j]; j++){
  				word[j] = tolower(word[j]);
			}
			if(!g_slist_find_custom (list,word,(GCompareFunc)g_strcmp0)){
				list = g_slist_insert_sorted(list,word,(GCompareFunc)g_strcmp0);
			}
    	}

		fclose(ifile);
	}
	closedir (pDir);
	g_slist_foreach(list,(GFunc)looklist,NULL);
	return 0;

}

void looklist(char* word){
	printf("%s\n",word );
}
/*
////////
int main (int c, char *v[]) {
    int len,i=0;
    struct dirent *pDirent;
    DIR *pDir;
//	GSList* list;

    if (c < 2) {
        printf ("Usage: testprog <dirname>\n");
        return 1;
    }
    pDir = opendir (v[1]);
    if (pDir == NULL) {
        printf ("Cannot open directory '%s'\n", v[1]);
        return 1;
    }

    while ((pDirent = readdir(pDir)) != NULL) {
        
        //---Open file ---//
    	char* f_path ;
    	sprintf(f_path,"%s/%s",v[1], pDirent->d_name);
    	FILE* ifile = fopen(f_path,"r");
    	printf("%s\n",pDirent->d_name);
        /*---get word from file---*/
//      char *word;
/*	   	while(1){
    		//read a wor from file
			word = malloc(sizeof(word));	//allocate memory
			c = fscanf(ifile,"%32[^A-Za-z]",word);		//read not alphabet to move cursor
			c = fscanf(ifile,"%32[A-Za-z]",word);	// got one word from the file	//
			if(c == EOF)break;

			//to Lower
			int j;
			for( j= 0; word[j]; j++){
  				word[j] = tolower(word[j]);
			}


			printf("   %s\n",word);
    	}

//	fclose(ifile);
    }
  
    closedir (pDir);
    return 0;

}*/