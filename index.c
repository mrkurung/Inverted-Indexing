#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>

GSList *list = NULL;
GHashTable *hash;
FILE *ofile;

void looklist(char* word);
void printindex(GSList *index);
int compare_int (int *a, int *b);

int main (int c, char *v[]) {
	int len;
	struct dirent *pDirent;
	DIR *pDir;
	hash= g_hash_table_new ( g_str_hash,g_str_equal);

	if (c < 2) {
		printf ("Usage: testprog <dirname>\n");
		return 1;
	}
	
	pDir = opendir (v[1]);
	if (pDir == NULL) {
		printf ("Cannot open directory '%s'\n", v[1]);
		return 1;
	}
	
	GSList* indexlist = NULL;
	while ((pDirent = readdir(pDir)) != NULL) {

		char f_path[24] ;
		int*index=g_malloc(sizeof(int));
		sprintf(f_path,"%s/%s",v[1],pDirent->d_name);
		FILE* ifile = fopen(f_path,"r");
		sscanf(f_path,"%*[^0123456789]%d",index);
		printf("%d\n",*index );


		while(1){

			char * word = g_malloc(1024);
			c = fscanf(ifile,"%[^A-Za-z]",word);
			c = fscanf(ifile,"%[A-Za-z]",word);
			word = realloc(word,strlen(word)+1);
			if(c == EOF)break;

			int j;
			for( j= 0; word[j]; j++){
  				word[j] = tolower(word[j]);
			}
			
			if(!g_slist_find_custom (list,word,(GCompareFunc)g_strcmp0)){
				
				list = g_slist_insert_sorted(list,word,(GCompareFunc)g_strcmp0);

				GSList* indexlist = NULL;
				indexlist = g_slist_append(indexlist,index);
				g_hash_table_insert (hash,word,indexlist);
				
			}else{

				GSList* indexlist=g_hash_table_lookup(hash,word);

				int *value=(g_slist_last(indexlist))->data;
				if(*value!=*index){
				indexlist= g_slist_append(indexlist,index);

				}
			}
    	}

		fclose(ifile);
	}

	closedir (pDir);
	ofile=fopen("output","w");
	fprintf(ofile, "%d\n",g_slist_length(list));
	g_slist_foreach(list,(GFunc)looklist,NULL);
	fclose(ofile);
	return 0;
}

void looklist(char* word){

	GSList *indexlist=g_hash_table_lookup(hash,word);
	indexlist = g_slist_sort(indexlist,(GCompareFunc)compare_int);
	int *arraylist=indexlist->data;

	fprintf(ofile,"%s:%d:%d",word,g_slist_length(indexlist),*arraylist);
	indexlist=indexlist->next;
	while( indexlist!=NULL){
		arraylist=indexlist->data;
		fprintf(ofile,",%d",*arraylist);
		indexlist=indexlist->next;

	}
	fprintf(ofile,"\n");
}

int compare_int (int *a, int *b){
	return (*a-*b);
  
}
