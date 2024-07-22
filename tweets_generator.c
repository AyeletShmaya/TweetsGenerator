#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "markov_chain.h"



int fill_database(FILE *fp, int words_to_read, MarkovChain *markov_chain)
{
    //fills the database with the words from the file
        int count = 0;
        char buffer[SENTENCE_LEN];
        while(fgets(buffer, SENTENCE_LEN, fp) != NULL)
        {
            Node *last_node = NULL;
            char *token = strtok(buffer, " \n\r");
            while (token != NULL &&
            (words_to_read == - 1 || count < words_to_read))
            {
                    Node *node = add_to_database(markov_chain, token);
                    if (node == NULL)
                    {
                        printf("Allocation failure: adding a node.\n");
                        free_database(&markov_chain);
                        return 1;
                    }

                    if (last_node != NULL &&
                    check_if_word_final(last_node->data->data) == 0)
                    {
                        bool ok = add_node_to_frequencies_list(
                                last_node->data, node->data);
                        if (!ok)
                        {
                            free_database(&markov_chain);
                            return EXIT_FAILURE;
                        }
                    }
                    token = strtok(NULL, " \n\r");
                    count++;
                    last_node = node;
            }

        }
        return 0;
}

void tweet_generator(MarkovChain *markov_chain, int num_of_tweets)
{
    // generates num_of tweets number of tweets
    for (int i = 1; i <= num_of_tweets; i++)
    {
        MarkovNode *first_word = get_first_random_node(markov_chain);
        printf("Tweet %d: %s ", i, first_word->data);
        generate_tweet(markov_chain,first_word, MAX_LEN);
        printf("\n");
    }
}

int main(int argc,char *argv[])
{
    // 1st - seed, 2nd - amount of tweets,
    // 3rd - text path, 4th (optional) - amount of words
    if (argc != FOUR_ARGS && argc != FIVE_ARGS)
    {
        printf("Usage: wrong number of elements.\n 1st arg: seed, 2nd arg - amount of tweets, "
               "3rd - text path, 4th (optional) - amount of words per tweet.");
        return EXIT_FAILURE;
    }
    FILE *text_stream = fopen(argv[3], "r");
    if (text_stream == NULL)
    {
        printf("Error: invalid path.\n");
        return EXIT_FAILURE;
    }
    long int num_of_tweets = strtol(argv[2], NULL, DECI);
    long int seed = strtol(argv[1], NULL, DECI);
    srand(seed);
    MarkovChain *markov_chain = malloc(sizeof (MarkovChain));
    if (markov_chain == NULL)
    {
        printf("Allocation failure: markov chain\n");
        return EXIT_FAILURE;
    }
    LinkedList *linked = malloc(sizeof(LinkedList) * 1);
    if (linked == NULL)
    {
        printf("Allocation failure: linked list.\n");
        free(markov_chain);
        return EXIT_FAILURE;
    }
    linked->size = 0; linked->first = NULL; linked->last = NULL;
    markov_chain->database = linked;
    if (argc == FIVE_ARGS)
    {
        int num_of_words = strtol(argv[4], NULL, DECI);
        fill_database(text_stream, num_of_words, markov_chain);
    }
    else
    {
        fill_database(text_stream, -1, markov_chain);
    }
    fclose(text_stream);
    tweet_generator(markov_chain, num_of_tweets);
    free_database(&markov_chain);
    return EXIT_SUCCESS;
}
