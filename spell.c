#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

char* strip_punc(char *s) {
  for ( ; *s && !isalpha(*s); ++s)
    ; // skip leading non-alpha chars
  if (*s == '\0')
    return s; // there are no alpha characters

  char *tail = s + strlen(s);
  for ( ; !isalpha(*tail); --tail)
    ; // skip trailing non-alpha chars
  *++tail = '\0'; // truncate after the last alpha

  return s;
}

bool check_word(const char* word, hashmap_t hashtable[]) {
    int bucket = hash_function(word);
    hashmap_t cursor = hashtable[bucket];
    while(cursor != NULL){
        if(word == cursor->word){
            return true;
        }
        cursor = cursor->next;
    }
    free(cursor);
    return false;
}

int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]) {
    int num_misspelled = 0;
    size_t len = 0;
    ssize_t read;
    char * line = NULL;
    char * token= NULL;
    char * rest= NULL;
    char * word= NULL;
    bool check;
    while((read = getline(&line, &len, fp)) != EOF){
        for (token = strtok_r(line, " ", &rest); token != NULL; token = strtok_r(NULL, " ", &rest)) {
            for (int i = 0; i < strlen(token); i++) {
                token[i] = tolower(token[i]);
            }
            word = strip_punc(token);
            if((check = check_word(word, hashtable)) == false){
                misspelled[num_misspelled] = word;
                num_misspelled += 1;
            }
        }
    }
    free(line);
    free(rest);
    free(token);
    return num_misspelled;
}


bool load_dictionary(const char* dictionary_file, hashmap_t
                     hashtable[]) {
    size_t len = 0;
    ssize_t read;
    char * line = NULL;
    char * token= NULL;
    char * rest= NULL;
    
    for (int i=0; i < HASH_SIZE; i++){
        hashtable[i] = NULL;
    }
    FILE *fp = fopen(dictionary_file, "r");
    if(fp == NULL) {
        return false;
    }
    
    while((read = getline(&line, &len, fp)) != EOF){
        for (token = strtok_r(line, "\n", &rest); token != NULL; token = strtok_r(NULL, "\n", &rest)) {
            for (int i = 0; i < strlen(token); i++) {
                token[i] = tolower(token[i]);
            }
            hashmap_t new_node;
            new_node = (hashmap_t)malloc(sizeof(struct node));
            new_node->next = NULL;
            strncpy(new_node->word, token, strlen(token));
            int bucket = hash_function(new_node->word);
            if ((hashtable[bucket]) == NULL) {
                hashtable[bucket] = new_node;
            }
            else{
                new_node->next = hashtable[bucket];
                hashtable[bucket] = new_node;
            }
            free(new_node);
        }
    }
    fclose(fp);
    return true;
}
