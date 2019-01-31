/* Code for COMP10002 Assignment 2, May 2017.
   Jeremy Tee Yee Lun, teej1@student.unimelb.edu.au
   Algorithms are fun !
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#define DIV "=========================" /* stage header */
/* stage numbers */
#define STAGE_NUM_ONE 1 
#define STAGE_NUM_TWO 2
#define STAGE_NUM_THREE 3
#define STAGE_NUM_FOUR 4
#define STAGE_NUM_FIVE 5

#define SIZE_OF_WORD 30
#define NUM_PROBABILITY 3
#define MAX_WORDS 100

#define READ_OK 1
#define READ_FAILED 0

#define BS_NOT_FOUND (-1)
#define BS_FOUND 0

#define LAST_NAME "LAST_NAME"
#define FIRST_NAME "FIRST_NAME"
#define NOT_NAME "NOT_NAME"

/* store the dict words */
typedef struct data data_t;
struct data 
{
	char name[SIZE_OF_WORD + 1];
	int array[NUM_PROBABILITY];
};

/* linked list for the sentence */
typedef struct node node_t;

struct node 
{
	char word[SIZE_OF_WORD + 1];
	node_t *next;
};

typedef struct 
{
	node_t *head;
	node_t *tail;
} list_t;


void stage_one(data_t names[], int *num_names);
void stage_two(data_t names[], int *num_names);
void stage_three(list_t *list, node_t *tmp);
void stage_four(list_t *list, data_t names[], node_t *tmp, int *num_names);
void print_stage_heading(int stage);
void free_list(list_t *list);
void insert_at_tail(list_t *list, char value[]);
int binary_search(data_t names[], int lo, int num_names, char string[], 
	int* locn);
list_t *make_empty_list(void);
int getword(char W[], int limit);

int
main (int argc, char* argv[])
{	
	data_t names[MAX_WORDS];
	list_t *list;
	list = make_empty_list();
	int num_names = 0;
	node_t *cursor;
	cursor = (node_t*)malloc(sizeof(*cursor));
	stage_one(names, &num_names);
	stage_two(names, &num_names);
	stage_three(list, cursor);
	stage_four(list, names, cursor, &num_names);
	free_list(list);
	return 0;
}

/* from 10.2 pg 172 of textbook */
list_t
*make_empty_list(void)
{
	list_t *list;
	list = (list_t*)malloc(sizeof(*list));
	assert(list != NULL);
	list -> head = list -> tail = NULL;
	return list;
}

/* from listop.c */
void
free_list(list_t *list) {
	node_t *curr, *prev;
	assert(list!=NULL);
	curr = list->head;
	while (curr) {
		prev = curr;
		curr = curr->next;
		free(prev);
	}
	free(list);
}

/* from 10.2 pg 173 of textbook */
void
insert_at_tail(list_t *list, char value[])
{
	node_t *new;
	new = (node_t*)malloc(sizeof(*new));
	assert(list != NULL && new != NULL);
	strcpy(new -> word, value);
	new -> next = NULL;
	if(list -> head == NULL)
	{
		list -> head = list -> tail = new;
	}
	
	else
	{
		list -> tail -> next = new;
		list -> tail = new;
	}
}

/* from skeleton code assmt1 */
void 
print_stage_heading(int stage) {
	/* main job is to print the stage number passed as an argument */
	if (stage > STAGE_NUM_ONE) {
		/* separate the stages */
		printf("\n");
	}
	printf("%sStage %d%s\n", DIV, stage, DIV);
}

/* from 7.9 pg 121 of the textbook */
int
getword(char W[], int limit)
{
	int c, len = 0;
	/* first, skip over any non-alphabetics */
	while ((c = getchar()) != EOF && !isalpha(c))
	{
		/* do nothing more */
	}
	
	if (c == EOF)
	{
		return EOF;
	}
	
	W[len++] = c;
	while(len < limit && (c = getchar()) != EOF && isalpha(c))
	{
		W[len++] = c;
	}
	
	W[len] = '\0';
	return 0;
}

/* from lecture slides */
int
binary_search(data_t names[], int lo, int num_names, char string[], int* locn)
{
	int mid, outcome;
	
	if (lo >= num_names)
	{
		return BS_NOT_FOUND;
	}
	
	mid = (lo + num_names)/2;
	if ((outcome = strcmp(string, names[mid].name)) < 0)
	{
		return binary_search(names, lo, mid, string, locn);
	}
	else if (outcome > 0)
	{
		return binary_search(names, mid+1, num_names, string, locn);
	}
	else
	{
		*locn = mid;
		return BS_FOUND;
	}
}

/* read in all the dict words and probability and print first word */
void
stage_one(data_t names[], int *num_names)
{
	char ch1, ch2 = '#';
	int i = 0;
	
	/* dict words start with #, read when # is detected */
	while(scanf(" %c", &ch1) == 1 && ch1 == ch2)
	{
		scanf("%s",names[i].name);
		int j;
		for (j = 0; j < NUM_PROBABILITY; j++)
		{
			scanf("%d", &names[i].array[j]);
		}
		i++;
		/* increment for every word in dict - stage 2 */
		(*num_names)++;
	}
	
	print_stage_heading(STAGE_NUM_ONE);
	
	/* print based on output required */
	printf("Word 0: %s\n", names[0].name);
	printf("Label probabilities: ");
	for (i = 0; i < NUM_PROBABILITY; i++)
	{
		printf("%d%%",names[0].array[i]);
		if (i < NUM_PROBABILITY - 1)
		{
			printf(" ");
		}
	}
	printf("\n");
}
	
/* use strlen to calculate the number of char per word */
void
stage_two(data_t names[], int *num_names)
{
	print_stage_heading(STAGE_NUM_TWO);
	printf("Number of words: %d\n", *num_names);
	int i;
	double count = 0;
	 
	/* increment count to obtain the total num of char for every word */
	for (i = 0; i < *num_names; i++)
	{
		count += strlen(names[i].name);
	}
	
	printf("Average number of characters per word: %.2f\n", count/(*num_names));
	
}

/* insert every word from sentence into linked list */
void
stage_three(list_t *list, node_t *cursor)
{
	char scan_word[SIZE_OF_WORD + 1];
	
	print_stage_heading(STAGE_NUM_THREE);
	
	/* get word from sentence and add into tail of linked list */
	while (getword(scan_word, SIZE_OF_WORD) != EOF)
	{
		insert_at_tail(list, scan_word);
	}
	
	/* assign a cursor to act as a head to print the whole linked list */
	  strcpy(cursor -> word, list -> head -> word);
	  cursor -> next = list -> head -> next;
	  while (cursor != NULL) {
	  	  printf("%s\n", cursor -> word);
		cursor = cursor -> next;
	  }
}

/* print the word from sentence and obtain probability result */
void
stage_four(list_t *list, data_t names[], node_t *cursor, int *num_names)
{
	print_stage_heading(STAGE_NUM_FOUR);
	int locn;
	
	/* assign a cursor to act as a head */
	strcpy(cursor -> word, list -> head -> word);
	cursor -> next = list -> head -> next;
	
	/* use bsearch for the whole linked list and return from data_t names */
	  while (cursor != NULL) {
	  	
	  	/* for the word from sentence */
	  	printf("%-32s", cursor -> word);
	  	
	  	/* if word not in dict */
		if(binary_search(names, 0, *num_names, cursor -> word, &locn) == 
			BS_NOT_FOUND)
		{
			printf("%s\n",NOT_NAME);
		}
		
		/* word in dict */
		else
		{
			/* compare the probability from names[].array[] */
			if (names[locn].array[0] > 0 && names[locn].array[1] > 0)
			{
				printf("%s, %s", FIRST_NAME, LAST_NAME);
			}
			
			else if (names[locn].array[0] > 0)
			{
				printf("%s", FIRST_NAME);
			}
			
			else if (names[locn].array[1] > 0)
			{
				printf("%s", LAST_NAME);
			}
			printf("\n");
		}
		/* shift cursor to the next word in linked list */
		cursor = cursor -> next;
	  }	
}
		 