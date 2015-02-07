#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
int main (int c, char *v[]) {
    int len,i=0;
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
    	char* f_name = pDirent->d_name;
    	FILE* ifile = fopen(f_name,"r");

        //---get word from file---//
        char *word,tmp[32];
    	while(1){
    		//read a wor from file
			
			c = fscanf(ifile,"%32[^A-Za-z]",tmp);		//read not alphabet to move cursor
			word = g_malloc(sizeof(word));		//allocate memory
			c = fscanf(ifile,"%32[A-Za-z]",word);	// got one word from the file	//
			if(c == EOF)break;

			//to Lower
			int j;
			for( j= 0; word[j]; j++){
  				word[j] = tolower(word[j]);
			}
		//	printf("not found %s \n",word);
    	}










    }
  
    closedir (pDir);
    return 0;

}