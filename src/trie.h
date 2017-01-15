#include <stdio.h>
#include <stdlib.h>


#define ALPHABET_SIZE 27
#define MAX_WORDS 200
typedef struct _word_list {
	char** words;
	int max_word_size;
	int max_size;
	int num_words;
} word_list;

word_list* new_word_list(int max_size, int max_word_size);
void destroy_word_list(word_list*);
void insert_word(word_list* wlist, const char* new_word);


/************************************/
/***********Tree Nodes***************/
typedef struct _tree_node{
    unsigned char character;    
	int is_full_word;
    struct _tree_node* children[ALPHABET_SIZE];
} tree_node;

typedef struct _trie {
	size_t num_word;
	tree_node* data;
} trie;
//init from file
trie* init_trie_from_file (const char* sourcefile);
void destroy_trie(trie*);

//Searches for a given word in the existing
//tree as is, or in the source_file if the
//source file hasn't been read to the end. 
//As it searches, it augments the inner-tree
//with additional information
//returns 1 if the inner_tree contains the word
//0 otherwise
int contains_word(const trie* t, const char* word);
void print_all(const trie* t);
trie* list_words(trie* source, const char* pattern);


