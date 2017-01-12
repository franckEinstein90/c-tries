#include <stdio.h>
#include <string.h>
#include "trie.h"


int convert_to_idx(char letter) {
	if (letter >= 'a' && letter <= 'z') return letter - 'a';
	if (letter >= 'A' && letter <= 'Z') return letter - 'A';
	if (letter == '\'') return 'z' - 'a' + 1;

}
//returns a new tree_node
tree_node* init_tree(){
	tree_node* new_tree = (tree_node*)malloc(sizeof(tree_node));
	new_tree->character = '*';
	for (int i = 0; i < CHILD_MAX; i++) {
		new_tree->children[i] = NULL;
	}
	new_tree->is_full_word = 0;
	return new_tree;
}
tree_node* insert_at_node(tree_node* node, const char* word, size_t idx) {
	char letter = word[idx];
	if (letter == '\0') {
		node->is_full_word = 1;
		return node;
	}
	int insert_pt = convert_to_idx(letter);
	if (insert_pt >= 0 && insert_pt <= CHILD_MAX) {
		if (node->children[insert_pt] == NULL) {
			node->children[insert_pt] = init_tree();
			node->children[insert_pt]->character = letter;
		}
		return insert_at_node(node->children[insert_pt], word, idx + 1);
	}
	else {
		printf("this shouldn't happen");
	}
}
tree_node* insert_in_tree(tree_node* tree_root, const char* word){
	return insert_at_node(tree_root, word, 0);
}

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

int contains(tree_node* t, const char* word, size_t idx) {
	char tag = word[idx];
	if (tag == '\0' && t->is_full_word) return 1;
	if (t->children[convert_to_idx(tag)]) {
		return contains (t->children[convert_to_idx(tag)], word, idx + 1);
	}
	return 0;
}

int contains_word(trie* t, const char* word) {
	int idx = convert_to_idx(word[0]);
	tree_node* begin_trie = t->data->children[idx];
	if (begin_trie != NULL) return contains(begin_trie, word, 1);
	return 0;
}

int find_word(trie* t, const char* word) {
	if (contains_word(t, word)) return 1;
	return 0;
}

trie* filter_trie(trie* source_trie, trie* dest_trie, const char* pattern, int idx) {
	return NULL;
}
trie* list_words(trie* source, const char* pattern) {
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


    
