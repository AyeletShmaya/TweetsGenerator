#ifndef _MARKOV_CHAIN_H_
#define _MARKOV_CHAIN_H_
#define WORD_LEN 101
#define SENTENCE_LEN 1001
#define DECI 10
#define MAX_LEN 20
#define FOUR_ARGS 4
#define FIVE_ARGS 5

#include "linked_list.h"
#include <stdio.h>  // For printf(), sscanf()
#include <stdlib.h> // For exit(), malloc()
#include <stdbool.h> // for bool
struct MarkovNodeFrequency;

#define ALLOCATION_ERROR_MASSAGE "Allocation failure: Failed to allocate"\
            "new memory\n"



typedef struct MarkovChain
{
    LinkedList *database;
} MarkovChain;

typedef struct MarkovNode
{
    char data[WORD_LEN];
    struct MarkovNodeFrequency *frequencies_list;
} MarkovNode;

typedef struct MarkovNodeFrequency
{
    MarkovNode *markov_node;
    int frequency;
    int num_of_nodes ;
} MarkovNodeFrequency;



/**
* Creates a new frequency list for a markov node, if there's no one already
 * if successful, returns the ptr to the new markov freq node
 * otherwise return NULL.
 * @param *data_ptr - a pointer to the markov node
 * of the word that comes next.
 */
MarkovNodeFrequency *new_frequency_list (MarkovNode *data_ptr);

/**
* Creates a new markov node
 * if successful, returns the ptr to the new markov freq node
 * otherwise return NULL.
 * @param *word - a string of the word
 */
MarkovNode *new_markov_node (char *word);

/**
* Checks if a word is in the database
 * if it is, returns a pointer to the node
 * otherwise return NULL.
 * @param *markov_chan - a pointer to the markov chain
 * @param word- a string of the word
 */
Node *is_node_in_database (MarkovChain *markov_chain, char word[WORD_LEN]);

/**
* frees markov node from memory
 * returns nothing
 * @param markov_node - a pointer to the markov node that needs to be freed
 */
void free_markov_node (MarkovNode *markov_node);

/**
* frees linked list from heap
 * returns nothing
 * @param list - a pointer to the linked list that needs to be freed
 */
void free_linked_list (LinkedList *list);

/**
* Checks if a word ends with a dot '.'.
 * returns 1 if the word does end with '.', 0 otherwise.
 * @param word_ptr - a string of the word.
 */
int check_if_word_final (const char *word_ptr);

/**
* If data_ptr in markov_chain, return it's node. Otherwise, create new
 * node, add to end of markov_chain's database and return it.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return markov_node wrapping given data_ptr in given chain's database,
 * returns NULL in case of memory allocation failure.
 */
Node* add_to_database(MarkovChain *markov_chain, char *data_ptr);

/**
* Check if data_ptr is in database.
 * If so, return the markov_node wrapping it in
 * the markov_chain, otherwise return NULL.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return Pointer to the Node wrapping given state, NULL if state not in
 * database.
 */
Node* get_node_from_database(MarkovChain *markov_chain, char *data_ptr);

/**
 * Add the second markov_node to the frequency list of the first markov_node.
 * If already in list, update it's occurrence frequency value.
 * @param first_node
 * @param second_node
 * @return success/failure: true if the process was successful, false if in
 * case of allocation error.
 */
bool add_node_to_frequencies_list(MarkovNode *first_node,
                                  MarkovNode *second_node);

/**
 * Free markov_chain and all of it's content from memory
 * @param markov_chain markov_chain to free
 */
void free_database(MarkovChain ** ptr_chain);

/**
 * Get one random state from the given markov_chain's database.
 * @param markov_chain
 * @return
 */
MarkovNode* get_first_random_node(MarkovChain *markov_chain);

/**
 * Choose randomly the next state, depend on it's occurrence frequency.
 * @param state_struct_ptr MarkovNode to choose from
 * @return MarkovNode of the chosen state
 */
MarkovNode* get_next_random_node(MarkovNode *state_struct_ptr);

/**
 * Receive markov_chain, generate and print random sentence out of it. The
 * sentence most have at least 2 words in it.
 * @param markov_chain
 * @param first_node markov_node to start with,
 *                   if NULL- choose a random markov_node
 * @param  max_length maximum length of chain to generate
 */
void generate_tweet(MarkovChain *markov_chain, MarkovNode *
first_node, int max_length);


#endif /* _MARKOV_CHAIN_H_ */
