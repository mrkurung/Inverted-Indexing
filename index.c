#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>

GSList* list;
GHashTable *hash;
void looklist(char* word);
void addindex(char*word ,int index);

int main (int c, char *v[]) {
	hash= g_hash_table_new ( g_direct_hash,g_direct_equal);
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
		char f_path[24];
		int index;
		sprintf(f_path,"%s/%s",v[1],pDirent->d_name);
		FILE* ifile = fopen(f_path,"r");
		sscanf(f_path,"%*[^0123456789]%d",&index);

		while(1){
			//read a word from file
			char * word = malloc(sizeof(char)*32);	//allocate memory
			c = fscanf(ifile,"%*[^A-Za-z]",word);	//read not alphabet to move cursor
			c = fscanf(ifile,"%32[A-Za-z]",word);	// got one word from the file	//
			if(c == EOF)break;

			//to Lower
			int j;
			for( j= 0; word[j]; j++){
  				word[j] = tolower(word[j]);
			}

			if(!g_slist_find_custom (list,word,(GCompareFunc)g_strcmp0)){
				list = g_slist_insert_sorted(list,word,(GCompareFunc)g_strcmp0);

				int*indexlist=g_malloc(sizeof(int)*2);
				indexlist[0]=1;
				indexlist[1]=index;
				g_hash_table_insert(hash,word,indexlist);
				printf("addword\n");
			}else{
				addindex(word,index);
//				int *oldindexlist = g_hash_table_lookup(hash,word);
//				printf("%s\t:%d:",word,oldindexlist[0]);
/*			int *tmp = g_realloc(indexlist, sizeof(int)*(indexlist[0]+2));
					if(!tmp){
					tmp=g_malloc(sizeof(int)*(indexlist[0]+2));
					memcpy(tmp, indexlist, indexlist[0] * sizeof(int));
					indexlist=tmp;
				}
*/				printf(" found\n" );

			}
    	}

		fclose(ifile);
	}
	closedir (pDir);
	g_slist_foreach(list,(GFunc)looklist,NULL);
	return 0;

}

void addindex(char *word, int index){
		//int *indexlist = g_hash_table_lookup(hash,word);
		//printf("%s\t:%d:",word,indexlist[0]);
}

void looklist(char* word){
	int i,*indexlist = g_hash_table_lookup(hash,word);
	printf("%s\t:%d:",word,indexlist[0]);
	for ( i = 1; i <= indexlist[0]; ++i)
	{
		printf("%d,",indexlist[i]);
	}
	printf("\n");
}
