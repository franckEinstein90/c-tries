#include <stdio.h>
#include <stdlib.h>


#define CHILD_MAX 27
typedef struct _tree_node{
    unsigned char character;    
	int is_full_word;
    struct _tree_node* children[CHILD_MAX];
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
int contains_word(trie* t, const char* word);
trie* list_words(trie* source, const char* pattern);


