#include <stdio.h>
#include <string.h>
#include "trie.h"



//constructor for an array of words
//structure
word_list* new_word_list(int size, int max_word_size) {
	word_list* result;
	result = (word_list*)malloc(sizeof(word_list));
	result->words = (char**)malloc(size * sizeof(char*));
	result->max_word_size = 100;
	result->max_size = size;
	result->num_words = 0;
	return result; 
}

void insert_word(word_list* wlist, const char* word) {
	if (wlist->num_words < wlist->max_size) {
		wlist->words[wlist->num_words++] = _strdup(word);
	}
}
void insert_into(word_list* SuperSet, word_list* SubSet) {
	
}
void print_words(word_list* wlist) {
	int i;
	for (i = 0; i < wlist->num_words; i++) {
		printf("%s\n", wlist->words[i]);
	}
}

void destroy_word_list(word_list* wlist) {
	int i;
	for (i = 0; i < wlist->num_words; i++) {
		free(wlist->words[i]);
	}
	free(wlist->words);
}

/*************************************/
/***************Converter Struct******/
/*************************************/
//converter structure holds a character, 
//and an int representing the index
//corresponding to that char in the 
//trie structure
typedef struct _converter {
	int trie_index;
	char character;
	int valid; //1 if trie_index is a valid trie index
} converter;
int convert_to_idx(char letter) {
	if (letter >= 'a' && letter <= 'z') return letter - 'a';
	if (letter >= 'A' && letter <= 'Z') return letter - 'A';
	if (letter == '\'') return 'z' - 'a' + 1;
	return -1;
}
int process_converter(converter* cv) {
	cv->trie_index = convert_to_idx(cv->character);
	if (cv->trie_index >= 0 && cv->trie_index <= ALPHABET_SIZE) 
		{ cv->valid = 1; return 1; }
	else 
		{ cv->valid = 0; return 0; }
}
//constructing a new converter
converter* new_converter(char c) {
	converter* cv;
	cv = (converter*)malloc(sizeof(converter));
	cv->character = c;
	process_converter(cv);
	return cv;
}
//a new converter from a word and an index
converter* new_converter_word_pos(const char* word, int idx) {
	if (idx < 0 || idx > strlen(word)) {
		return NULL;
	}
	return new_converter(word[idx]);
}

/******************************************/
/********************** Tree Nodes ********/
/******************************************/
tree_node* init_tree(){
	tree_node* new_tree = (tree_node*)malloc(sizeof(tree_node));
	new_tree->character = '*';
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		new_tree->children[i] = NULL;
	}
	new_tree->is_full_word = 0;
	return new_tree;
}
tree_node* insert_at_node(tree_node* node, const char* word, size_t idx) {
	converter* c = new_converter_word_pos(word, idx);
	if (c->character == '\0') {
		node->is_full_word = 1;
		return node;
	}
	if (c->valid == 0) {
		fprintf(stderr, "Value of errno: %d\n", errno);
		perror("Invalid Character insert_at_node");
		free(c);
		return NULL;
	}
	if (node->children[c->trie_index] == NULL) {
			node->children[c->trie_index] = init_tree();
			node->children[c->trie_index]->character = c->character;
		}
	tree_node* modified_node = insert_at_node(node->children[c->trie_index], word, idx + 1);
	free(c);
	if (modified_node == NULL) {
		//todo error handling
	}
	return modified_node;
}
//inserts a word in the tree
//returns 1 if succeeded
tree_node* insert_in_tree(tree_node* tree_root, const char* word){
	tree_node* inserted_node;
	inserted_node = insert_at_node(tree_root, word, 0);
	return inserted_node;
}


/***************************************************/
/*************Finding words*************************/
/***************************************************/
/*word_list* get_suffixes(const tree_node* node, int max_word_size) {
//returns list of word endings starting from node argument
	int i;
	word_list *Asuffixes, *Osuffixes;
	for (i = 0; i < ALPHABET_SIZE; i++) {
		Asuffixes = new_word_list(200, max_word_size);
		if (node->children[i] != NULL) {
			Osuffixes = get_suffixes(node->children[i], max_word_size);
			insert_into(Asuffixes, Osuffixes);
			destroy_word_list(Osuffixes);
		}
	}
	prefix_by(node->character, Asuffixes);
	return Asuffixes;
}

void get_match(const trie* t, word_list* wlist, const char* pattern) {
	int i;
	converter* cv;
	if (pattern[0] == '*') {
		for (i = 0; i < ALPHABET_SIZE; i++) {
			if (t->data->children[i]) {
				__subfunc(t->data->children[i], wlist, pattern, 0);
			}
		}
		return wlist;
	}
	cv = new_converter(pattern[0]);
	if (cv->valid && t->data->children[cv->trie_index] != NULL) {
		__subfunc(t->data->children[cv->trie_index], wlist, pattern, 0);
	}
}*/

/*****************************************************/
/*************Initializing a new trie*****************/
/*****************************************************/
trie* init_empty_trie() {
	trie* new_trie = (trie*)malloc(sizeof(trie));
	new_trie->num_word = 0;
	new_trie->data = init_tree();
	return new_trie;
}

trie* init_trie_from_file(const char* file_name){
	trie* new_trie = init_empty_trie();
    FILE* input_file = fopen(file_name, "r");
	if (input_file == NULL) {
		//stderr unable to open input file
		destroy_trie(new_trie);
		return NULL;
	}
	while (!feof(input_file)) {
		char  line[100];
		fgets(line, 100, input_file);
		// remove the newline character
		line[strlen(line) - 1] = '\0';
		//insert the word in the data tree
		insert_in_tree(new_trie->data, line);
		new_trie->num_word++;
		//printf("Inserted word %s\n", line);
		}
	fclose(input_file);
    return new_trie;
}

int contains(const tree_node* t, const char* word, size_t idx) {
	converter * cv = new_converter_word_pos(word, idx);
	int result = 0;
	if (cv->character == '\0' && t->is_full_word) {
		free(cv); return 1;
	}
	if (cv->valid == 0) {
		//todo error handling
		free(cv); return 0;
	}
	if (t->children[cv->trie_index]) result = contains (t->children[cv->trie_index], word, idx + 1);
	free(cv);
	return result;
}
int contains_word(const trie* t, const char* word) {
	int idx = convert_to_idx(word[0]);
	const tree_node* begin_trie = t->data->children[idx];
	if (begin_trie != NULL) return contains(begin_trie, word, 1);
	return 0;
}
void print_branch(const tree_node* trie_branch) {

}
void print_all(const trie* t) {
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		if (t->data->children[i] != NULL) {
			print_branch(t->data->children[i]);
		}
	}
}
trie* filter_trie(const trie* source_trie, trie* dest_trie, const char* pattern, int idx) {
	converter* cv = new_converter_word_pos(pattern,idx);
	if (cv->character == '*') {
		free(cv);
		return NULL;
	}
	if(cv->valid){
	}
	free(cv);
	return NULL;
}
trie* list_words(const trie* source, const char* pattern) {
	trie* result_trie;
	result_trie = init_empty_trie();
	filter_trie(source, result_trie, pattern, 0);
	return result_trie;
}

void destroy_trie(trie* corpus){
    //delete all of corpus->data children
    //delete corpus->data
    //delete corpus
    
}


    
