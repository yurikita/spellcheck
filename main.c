#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "spell.h"
#include "dictionary.h"

int main() {
    bool loaded_dict;
    char *dictionary_file = "/Users/yurikajachmann/Documents/spell_xcode/spell_xcode/wordlist.txt";
    node* hashtable[HASH_SIZE];
    loaded_dict = load_dictionary(dictionary_file, hashtable);
    FILE *fp = fopen("/Users/yurikajachmann/Documents/spell_xcode/spell_xcode/test_worlist.txt", "r");
    if(fp == NULL) {
        perror("fopen");
        exit(1);
    }
    char * misspelled[1000];
    int num_misspelled;
    num_misspelled = check_words(fp, hashtable,misspelled);
    printf("%d", num_misspelled);
    return 0;
}
