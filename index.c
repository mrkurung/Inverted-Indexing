#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>

GSList* list;
GHashTable *hash;

void looklist(char* word);
void printindex(GSList *index);
int intcmp(int a,int b);

int main (int c, char *v[]) {
	int len;
	struct dirent *pDirent;
	DIR *pDir;
	hash= g_hash_table_new ( g_direct_hash,g_direct_equal);

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
		int*index=g_malloc(sizeof(int));
		sprintf(f_path,"%s/%s",v[1],pDirent->d_name);
		FILE* ifile = fopen(f_path,"r");
		sscanf(f_path,"%*[^0123456789]%d",index);

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
			GSList* hashval = NULL;
			if(!g_slist_find_custom (list,word,(GCompareFunc)g_strcmp0)){
				list = g_slist_insert_sorted(list,word,(GCompareFunc)g_strcmp0);
				
				hashval = g_slist_append(hashval,index);
				g_hash_table_insert (hash,word,hashval);
				printf("not found\t%s\n",word );
			}else{
		//		hashval=g_hash_table_lookup(hash,word);
		//		hashval= g_slist_insert_sorted(hashval,index,(GCompareFunc)intcmp);
				printf("found\t%s\n",word );
			}
    	}

		fclose(ifile);
	}
	closedir (pDir);
	g_slist_foreach(list,(GFunc)looklist,NULL);
	return 0;

}

void looklist(char* word){
	printf("%s",word );
	GSList* indexlist=g_hash_table_lookup(hash,word);
	printf("%d\n",GPOINTER_TO_INT( indexlist->data));

/*	do{
		printf("%d,",indexlist->data);

	}while( g_slist_next(indexlist)!=NULL);
*/	//g_slist_foreach(indexlist,(GFunc)printindex,NULL);
}
int intcmp(int a,int b){
	return a-b;
}
/*void printindex(GSList * index){
	printf("%d ,",index->data );

}*/