#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <pthread.h>
#define NUM_THREADS	4

GSList *list = NULL;
GHashTable *hash;
FILE *ifile,*ofile;
char * word[NUM_THREADS];

char f_path[24] ;
void looklist(char* word);
void* addkey(void *threadarg);
void printindex(GSList *index);
int compare_int (int *a, int *b);
void* getword();
struct thread_data{
	int thread;
	int  *index;
	char *word;
};

struct thread_data thread_data_array[NUM_THREADS];

int main (int c, char *v[]) {
	int len;
	struct dirent *pDirent;
	DIR *pDir;
	hash= g_hash_table_new ( g_str_hash,g_str_equal);
	pthread_t thread[NUM_THREADS];
	pthread_attr_t attr;

	int rc;
	long t;
	void *status;

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
			for(t=0; t<NUM_THREADS; t++) {
				pthread_create(&thread[t], NULL, getword, NULL); 


				word[t] = status;
				if(word[t]==NULL)break;
				printf("%s\n",word[t]);
			}


			for(t=0; t<NUM_THREADS; t++) {
				//printf("Main: creating thread %ld\n", t);

				   thread_data_array[t].word = word[t];
				   thread_data_array[t].index = index;
				   thread_data_array[t].thread = t;
				rc = pthread_create(&thread[t], NULL, addkey, (void *) &thread_data_array[t]); 
				if (rc) {
					printf("ERROR; return code from pthread_create() is %d\n", rc);
					exit(-1);
			    }
	      	}

			//addkey(word,index);

		}

		fclose(ifile);
	}

	closedir (pDir);
	ofile=fopen("output","w");
	list=g_slist_sort(list,(GCompareFunc)strcmp);
	fprintf(ofile, "%d\n",g_slist_length(list));
	g_slist_foreach(list,(GFunc)looklist,NULL);
	fclose(ofile);
	pthread_exit(NULL);
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
	//fprintf(ofile,"%s\n",word );
}

int compare_int (int *a, int *b){
	return (*a-*b);
  
}
void* getword(){

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

  	return  (void*)g_word->str;

 }
 void *addkey(void *threadarg){

   char *word;
   int*index;
   struct thread_data *my_data;

 	my_data = (struct thread_data *) threadarg;
 	word=my_data->word;
 	index=my_data->index;

 	GSList* indexlist=g_hash_table_lookup(hash,word);
	if(!indexlist){
//	if(!g_slist_find(list,word)){	
//	list = g_slist_insert_sorted(list,word,(GCompareFunc)g_strcmp0);
	list = g_slist_prepend(list,word);
	GSList* indexlist = NULL;
	indexlist = g_slist_prepend(indexlist,index);
	g_hash_table_insert (hash,word,indexlist);
			
	}else{

		
		
		int *value=indexlist->data;
//		printf("(%d %d)",*value ,*index );
		if(*value!=*(index)){
//printf(" add \n");
			indexlist= g_slist_prepend(indexlist,index);
			g_hash_table_replace (hash,word,indexlist);
		}
	} //printf("thread %d word %s  index %d\n",my_data->thread,word, *index);
	   pthread_exit(NULL);
 }