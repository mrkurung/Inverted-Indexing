#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>

GSList *list = NULL;
GHashTable *hash;
FILE *ifile,*ofile;

char f_path[30] ;
void looklist(char* word);
void addkey(GString* g_word,int* index);
void printindex(GSList *index);
int compare_int (int *a, int *b);
GString* getword();

int main (int c, char *v[]) {
	struct dirent *pDirent;
	DIR *pDir;
	hash= g_hash_table_new ( g_str_hash,g_str_equal);

	//http://stackoverflow.com/questions/3554120/open-directory-using-c
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
		int*index=g_malloc(sizeof(int*));
		sprintf(f_path,"%s/%s",v[1],pDirent->d_name);
		ifile = fopen(f_path,"r");
		sscanf(pDirent->d_name,"%*[^0-9]%d",index);
		while(1){
			GString *g_word = getword();
			if(g_word==NULL)break;
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

	list = g_slist_prepend(list,word);
	GSList* indexlist = NULL;
	indexlist = g_slist_prepend(indexlist,index);
	g_hash_table_insert (hash,word,indexlist);
	}else{
		int *value=indexlist->data;
		if(*value!=*index){
			indexlist= g_slist_prepend(indexlist,index);
			g_hash_table_insert(hash,g_word->str,indexlist);
		}
			g_string_free(g_word,TRUE);
	}
 }