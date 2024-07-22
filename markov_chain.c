#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "markov_chain.h"
#include "linked_list.h"





Node* add_to_database(MarkovChain *markov_chain, char *data_ptr) {

    if (is_node_in_database(markov_chain, data_ptr) != NULL)
    {
        return is_node_in_database(markov_chain, data_ptr);

    }

    MarkovNode *markov_node = new_markov_node(data_ptr);

    if (markov_node == NULL)
    {
        return NULL;
    }

    if (add(markov_chain->database, markov_node) != 0)
    {
        free_markov_node(markov_node);
        return NULL;
    }
    return markov_chain->database->last;

}

Node* get_node_from_database(MarkovChain *markov_chain, char *data_ptr)
{
    if (markov_chain->database->size == 0)
    {
        return NULL;
    }
    if (is_node_in_database(markov_chain, data_ptr) == 0)
    {
        return NULL;
    }
    return is_node_in_database(markov_chain, data_ptr);
}

bool add_node_to_frequencies_list(MarkovNode *first_node,
                                  MarkovNode *second_node)
{
    char *word = second_node->data;
    if (!first_node->frequencies_list)
    {
        first_node->frequencies_list = new_frequency_list(second_node);
        if (!first_node->frequencies_list)
        {
            printf("Allocation failure: frequency list. (1)\n");
            return false;
        }
        return true;
    }
    int freq_size = first_node->frequencies_list->num_of_nodes;
    MarkovNodeFrequency *runner = first_node->frequencies_list;
    for (int i = 0; i < freq_size; i++)
    {
        if (strcmp(runner->markov_node->data, word) == 0)
        {
            first_node->frequencies_list[i].frequency += 1;
            return true;
        }
        runner++;
    }
    MarkovNodeFrequency *temp =
            realloc(first_node->frequencies_list
                    ,sizeof(MarkovNodeFrequency) * (freq_size+1));
    if (temp == NULL)
    {
        printf("Allocation failure: adding to frequency list. (2)\n");
        return false;
    }
    temp[freq_size].markov_node = second_node;
    temp[freq_size].frequency = 1;
    first_node->frequencies_list = temp;
    first_node->frequencies_list->num_of_nodes += 1;
    return true;
}

void free_database(MarkovChain ** ptr_chain)
{
    free_linked_list((*ptr_chain)->database);
    free(*ptr_chain);
    *ptr_chain = NULL;


}


MarkovNodeFrequency *new_frequency_list (MarkovNode *data_ptr)
{
    MarkovNodeFrequency *new = malloc (sizeof(MarkovNodeFrequency));
    if (new == NULL)
    {
        printf("Allocation failure: markov frequency.\n");
        return NULL;
    }

    new->markov_node = data_ptr;
    new->frequency = 1;
    new->num_of_nodes = 1;
    return new;
}

MarkovNode *new_markov_node (char *word)
{
    MarkovNode *new_markov = malloc(sizeof(MarkovNode));
    if (new_markov == NULL)
    {
        printf("Allocation failure: markov node.\n");
        return NULL;
    }

    strncpy(new_markov->data, word, WORD_LEN);
    new_markov->frequencies_list = NULL;
    return new_markov;

}


void free_linked_list (LinkedList *list)
{
    Node *runner = list->first;
    for (int i = 0; i < list->size; i++)
        {
            free_markov_node (runner->data);
            Node *temp = runner->next;
            free(runner);
            runner = temp;
        }
    free(list);
    list = NULL;

}

void free_markov_node (MarkovNode *markov_node)
{
    free(markov_node->frequencies_list);
    //free(markov_node->data);
    free(markov_node);
}


Node *is_node_in_database (MarkovChain *markov_chain, char *word_ptr)
{
    Node *runner = markov_chain->database->first;
    while (runner != NULL) {
        if (strcmp(runner->data->data, word_ptr) == 0) {
            return runner;
        }
        runner = runner->next;

    }
    return NULL;
}

int get_random_number (int max_number)
{
    return rand() % max_number;
}

MarkovNode* get_first_random_node(MarkovChain *markov_chain)
{
    Node *first = markov_chain->database->first;
    int flag = 0, num;
    Node *word;
    while(!flag)
    {
        num = get_random_number(markov_chain->database->size);
        word = first;
        for (int i = 0; i < num; i++)
        {
            word = word->next;
        }
        if (check_if_word_final(word->data->data) == 0)
        {
            flag = 1;

        }

    }
    return word->data;

}

MarkovNode* get_next_random_node(MarkovNode *state_struct_ptr)
{
    int sum = 0;
    MarkovNodeFrequency *freq_ptr = state_struct_ptr->frequencies_list;
    int size = state_struct_ptr->frequencies_list->num_of_nodes;

    for (int i = 0; i<size; i++)
    {
        sum += freq_ptr->frequency;
        freq_ptr++;
    }

    const int num = get_random_number(sum);
    MarkovNodeFrequency *markov_ptr = state_struct_ptr->frequencies_list;
    int num_freq = markov_ptr->frequency;
    int count = 0;
    while (count + num_freq <= num)
    {
        count += num_freq;
        markov_ptr++;
        num_freq = markov_ptr->frequency;

    }
    return (markov_ptr)->markov_node;

}

void generate_tweet(MarkovChain *markov_chain, MarkovNode *
first_node, int max_length)
{
    int length = 1;
    MarkovNode *last_markov = first_node;
    while (length < max_length)
    {
        MarkovNode *new_word = get_next_random_node(last_markov);
        if (!new_word)
        {
            return;
        }
        if (check_if_word_final(new_word->data) == 1)
        {
            // end of sentence!
            printf("%s", new_word->data);
            return;
        }
        printf("%s ", new_word->data);
        last_markov = (get_node_from_database
                (markov_chain, new_word->data))->data;
        length++;
    }

}


int check_if_word_final (const char *word_ptr)
{
    // 0 means valid, 1 means the end of sentence

    const char *check = word_ptr;
    unsigned int count = 0;
    while (*(check + count) != '\0')
    {
        count++;
    }
    if (*(check + count - 1) == '.')
    {
        return 1;
    }
    return 0;
}

