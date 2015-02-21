#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>

GArray *list = NULL;
GHashTable *hash;
FILE *ifile,*ofile;

char f_path[30] ;
void looklist(char* word);
void addkey(GString* g_word,int* index);
void printindex(GSList *index);
int compare_int (int *a, int *b);
GString* getword();
int sort_strcmp(guint *a,guint *b);

int main (int c, char *v[]) {
//	struct dirent *pDirent;
//	DIR *pDir;
	hash= g_hash_table_new ( g_str_hash,g_str_equal);
	list = g_array_new(FALSE, FALSE, sizeof(char*));
	//http://stackoverflow.com/questions/3554120/open-directory-using-c

//	pDir = opendir (v[1]);
	int i,*index;
	for(i=1;TRUE;++i){
		index=malloc(sizeof(int*));
		*index=i;
		sprintf(f_path,"%s/file%d.txt",v[1],i);
//		ifile = fopen(f_path,"r");
//		printf("%s\n",f_path );
///		fclose(ifile);
	//}

//	while ((pDirent = readdir(pDir)) != NULL) {
//		int*index=g_malloc(sizeof(int*));
//		sprintf(f_path,"%s/%s",v[1],pDirent->d_name);
		ifile = fopen(f_path,"r");
		if (!ifile)break;
		
//		sscanf(pDirent->d_name,"%*[^0-9]%d",index);
		while(1){
			GString *g_word = getword();
			if(g_word==NULL)break;
			addkey(g_word,index);
		}
		fclose(ifile);
	}
//	closedir (pDir);
	ofile=fopen("output","w");
//	list=g_slist_sort(list,(GCompareFunc)strcmp); gint sort_strcmp(guint *a,guint *b)
	 g_array_sort(list, (GCompareFunc)sort_strcmp);
	fprintf(ofile, "%d\n",list->len);
//	g_slist_foreach(list,(GFunc)looklist,NULL);
//	int i;
	for(i=0;i<list->len;i++){
		looklist(g_array_index(list, char*, i));
	}
	fclose(ofile);
	return 0;
}
void looklist(char* word){
	GArray *indexlist=g_hash_table_lookup(hash,word);
//	indexlist = g_slist_sort(indexlist,(GCompareFunc)compare_int);
	int *index=g_array_index(indexlist,int*,0);
	fprintf(ofile,"%s:%d:%d",word,indexlist->len,*index);
	int i;
	for(i=1;i<indexlist->len;++i){
		index = g_array_index(indexlist,int*,i);
		fprintf(ofile, ",%d",*index );
	}

	fprintf(ofile,"\n");
//	g_slist_free(indexlist);
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
 	GArray* indexlist=g_hash_table_lookup(hash,g_word->str);
	if(!indexlist){
	char* word = g_string_free(g_word,FALSE);

	list = g_array_append_val(list,word);
	indexlist = g_array_new(FALSE, FALSE, sizeof(int*));
	indexlist = g_array_append_val(indexlist,index);
	g_hash_table_insert (hash,word,indexlist);
	}else{
		int *value=g_array_index(indexlist,int*,(indexlist->len)-1);
		if(*value!=*index){
			indexlist=  g_array_append_val(indexlist,index);
			//g_hash_table_insert(hash,g_word->str,indexlist);
		}
			g_string_free(g_word,TRUE);
	}
 }
int sort_strcmp(guint *a,guint *b){
	return g_strcmp0(GUINT_TO_POINTER(*a),GUINT_TO_POINTER(*b));
}