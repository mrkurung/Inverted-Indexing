#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>

GSList* list = NULL;
GHashTable *hash;

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
        //---Open file ---//
		char f_path[24] ;
		int*index=g_malloc(sizeof(int));
		sprintf(f_path,"%s/%s",v[1],pDirent->d_name);
		FILE* ifile = fopen(f_path,"r");
		sscanf(f_path,"%*[^0123456789]%d",index);
		

		while(1){
			//read a word from file
			char * word = malloc(sizeof(char*));	//allocate memory
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
				GSList* indexlist = NULL;
	//			indexlist = g_slist_append(indexlist,GINT_TO_POINTER(1));
				indexlist = g_slist_append(indexlist,index);
				//	int *arraylist=indexlist->data;
//				printf("not found:\t%s\t%d\t%d\t%d\t%d\n",word,hashval,hashval->data,arraylist,*arraylist );
				g_hash_table_insert (hash,word,indexlist);
				
			}else{

				GSList* indexlist=g_hash_table_lookup(hash,word);

				int *value=(g_slist_last(indexlist))->data;
				
			//	printf("\n(%d  %d)",*value,*index );
			//	hashval = g_slist_append(hashval,index);	
			//	hashval->next =g_new(GSList,1);
			//	hashval->next->data=index;
			
				if(*value!=*index){
	//			printf("%d\n",indexlist->data );
				//indexlist->data=indexlist->data+1;
				indexlist= g_slist_append(indexlist,index);
				}
//				hashval= g_slist_insert_sorted(hashval,index,(GCompareFunc)compare_int);
//				hashval= g_slist_insert_sorted(hashval,index,(GCompareFunc)compare_int);
//				printf("found add:\t%s\t%d\t%d\t%d\t%d\t%d\n",word,hashval,hashval->data,index,*arraylist,*index);
				//printf("found\t%s\n",word );
//				looklist(word);
			}
    	}

		fclose(ifile);
	}
	closedir (pDir);
	g_slist_foreach(list,(GFunc)looklist,NULL);
	return 0;

}

void looklist(char* word){

	GSList *indexlist=g_hash_table_lookup(hash,word);
	indexlist = g_slist_sort(indexlist,(GCompareFunc)compare_int);
///	printf("%d\n",*arraylist);
	printf("%s:%d:",word,g_slist_length(indexlist));
	do{
		int *arraylist=indexlist->data;
		printf("%d,",*arraylist);
		//printf("%d,",indexlist->data);
		indexlist=indexlist->next;
/*		if(indexlist!=NULL){
			arraylist=indexlist->data;
			printf("%d\n",*arraylist);
		}*/
	}while( indexlist!=NULL);
	printf("\b\n");
//	g_slist_foreach(indexlist,(GFunc)printf,NULL);
}
int compare_int (int *a, int *b)
{	return (*a-*b);
//	printf("compare_int is :%d\n",c );
  
}
/*void printindex(GSList * index){
	printf("%d ,",index->data );

}*/