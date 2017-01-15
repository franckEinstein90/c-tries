#include "../src/trie.c"
#include <iostream>
using namespace std;


int test_num;
void test(int test_result, int test_result_expected) {
	if (test_result == test_result_expected) {cout << "Passed ";}
	else { cout << "Failed "; }
	cout << "Test no" << test_num << endl;
	test_num++;
}

trie* word_tries;
trie* verb_tries;
int isword(const char* word_to_lookup) {
	return contains_word(word_tries, word_to_lookup);
}
int isverb(const char* verb_to_lookup) {
	return contains_word(verb_tries, verb_to_lookup);
}
int main() {
	word_list* wlist;
	test_num = 0;
	const char* word_file = "word100k.txt";
	const char* verb_file = "verbs.txt";
	
	
	//testing word_list object
	wlist = new_word_list(20, 100);
	insert_word(wlist, "\n");
	insert_word(wlist, "*****begin wlist tests*******");
	insert_word(wlist, "bart");
	insert_word(wlist, "ball");
	insert_word(wlist, "*****end wlist tests*******");
	print_words(wlist);
	destroy_word_list(wlist);
	
	//initializing the tries
	word_tries = init_trie_from_file(word_file);
	verb_tries = init_trie_from_file(verb_file);

	test(isword("abeam"), 1); //abeam is a word, returns 1
	test(isword("Ball"), 1); //case insensitive, Ball is a word, returns 1
	test(isword("ballA"), 0); //ballA is not a word, returns 0
	test(isverb("EaT"), 1); //Eat is a word
	test(isverb("begin"), 1); //begin is a verb, returns 1
	test(isverb("patri"), 0); //patri is not a verb, returns 0
	test(isword("franck"), 0); //franck is not a word, returns 0
	test(isword("porcupine"), 1);
	test(isword("lover"), 1);
	test(isverb("lover"), 0);

/*
	wlist = new_word_list(30, 100);*/
//	get_match(word_tries, wlist);

	destroy_trie(word_tries);
	destroy_trie(verb_tries);
	return 0;
}