#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>

GSList *list = NULL;
GHashTable *hash;
FILE *ifile,*ofile;
//char buffer[];
char f_path[24] ;
void looklist(char* word);
void addkey(GString* g_word,int* index);
void printindex(GSList *index);
int compare_int (int *a, int *b);
GString* getword();

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


		int*index=g_malloc(sizeof(int));
		sprintf(f_path,"%s/%s",v[1],pDirent->d_name);
		ifile = fopen(f_path,"r");
		sscanf(f_path,"%*[^0-9]%d",index);
//		printf("file%d\n",*index );


		while(1){


			GString *g_word = getword();
			if(g_word==NULL)break;


		//	printf("%s\n",g_word->str );
			addkey(g_word,index);

		}

		fclose(ifile);
	}

	closedir (pDir);
	ofile=fopen("output","w");
	list=g_slist_sort(list,(GCompareFunc)strcmp);
	fprintf(ofile, "%d\n",g_slist_length(list));
	g_slist_foreach(list,(GFunc)looklist,NULL);
	fclose(ofile);
	return 0;
}

void looklist(char* word){
//	printf("%s\n",word );

	GSList *indexlist=g_hash_table_lookup(hash,word);
	indexlist = g_slist_sort(indexlist,(GCompareFunc)compare_int);
	int *index=indexlist->data;

	fprintf(ofile,"%s:%d:%d",word,g_slist_length(indexlist),*index);
	indexlist=indexlist->next;
	while( indexlist!=NULL){
		index=indexlist->data;
		fprintf(ofile,",%d",*index);
		indexlist=indexlist->next;

	}
	fprintf(ofile,"\n");
	g_slist_free(indexlist);
	//fprintf(ofile,"%s\n",word );*/
}

int compare_int (int *a, int *b){
	return (*a-*b);
  
}
 GString* getword(){

 	char c;
 	GString* g_word= g_string_new(NULL);
 	c=fgetc(ifile);



	while(c<'A'||(c>'Z'&&c<'a')||c>'z'){
	 	if(c==-1)return NULL;
	 	c=fgetc(ifile);
	}

 	while(!(c<'A'||c>'z'||(c>'Z'&&c<'a'))){
 		g_string_append_c(g_word,tolower(c));
 		 c=fgetc(ifile);
	}

  	return g_word;

 }
 void addkey(GString* g_word,int* index){

 	GSList* indexlist=g_hash_table_lookup(hash,g_word->str);
	if(!indexlist){
	char* word = g_string_free(g_word,FALSE);
//	if(!g_slist_find(list,word)){	
//	list = g_slist_insert_sorted(list,word,(GCompareFunc)g_strcmp0);
	list = g_slist_prepend(list,word);
	GSList* indexlist = NULL;
	indexlist = g_slist_prepend(indexlist,index);
	g_hash_table_insert (hash,word,indexlist);
//	g_string_free(g_word,FALSE);
	}else{

		
		
		int *value=indexlist->data;
//		printf("(%d %d)",*value ,*index );
		if(*value!=*index){
//printf(" add \n");
			indexlist= g_slist_prepend(indexlist,index);
			g_hash_table_replace (hash,g_word->str,indexlist);
			g_string_free(g_word,FALSE);
		}else{
		g_string_free(g_word,TRUE);
		}
		
	}
 }