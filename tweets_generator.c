#include "markov_chain.h"

#define TEN 10
#define FIVE 5
#define FOUR 4
#define TWENY 20
#define HUNDRED 101
#define TOUTHEND 1001


LinkedList *create_linked_list ()
{
  LinkedList *list = malloc (sizeof (LinkedList));
  if (!list)
    {
      return NULL; // allocation failed
    }
  list->first = NULL;
  list->last = NULL;
  list->size = 0;
  return list;
}

MarkovChain *create_markov_chain ()
{
  MarkovChain *markov_chain = (MarkovChain *) malloc
      (sizeof (MarkovChain));
  if (markov_chain == NULL)
    {
      return NULL;
    }
  markov_chain->database = create_linked_list ();
  if (markov_chain->database == NULL)
    {
      free (markov_chain);
      return NULL;
    }
  return markov_chain;
}


int fill_database (FILE *fp, int words_to_read, MarkovChain *markov_chain)
{
  char sentence[TOUTHEND];
  int counter = 0;
  bool first = true;
  Node *first_node,*second_node;
  while (fgets (sentence, sizeof(sentence), fp)!=NULL)
    {
      char *word = strtok(sentence, " \t\n\r");
      char last[HUNDRED];
      while (word != NULL)
        {
          int len = strlen (word);// move the position to the next word
          if (first)
            {
              if (add_to_database (markov_chain, word)==NULL){
                  return 1;
                }
              first = false;
            }
          else
            {
              if (add_to_database (markov_chain, word)==NULL){
                return 1;
              }
              first_node = get_node_from_database (markov_chain, last);
              second_node = get_node_from_database (markov_chain, word);

              if (add_node_to_frequencies_list (first_node->data,
                                                second_node->data)
                  == false)
                {
                  return 1;
                }
              if (word[len - 1] == '.')
                {
                  first = true;
                }
            }
          memset (last, 0, sizeof (last));
          strcpy (last, word);
          counter += 1;
          if (counter == words_to_read)
            {
              return 0;
            }
          word = strtok(NULL, " \t\n\r");
        }
    }
  return 0;
}

int main (int argc, char *argv[])
{
  if (argc != FOUR && argc != FIVE)
    {
      printf ("Usage: not good param");
      return EXIT_FAILURE;
    }
  char *p;
  int seed = strtol (argv[1], &p, TEN);
  int max_length = strtol (argv[2], &p, TEN);
  char *path = argv[3];
  int words_to_read = -1;
  if (argc == FIVE)
    {
      words_to_read = strtol (argv[4], &p, TEN);
    }
  srand(seed);
  FILE *file = fopen (path, "r");
  if (file == NULL)
    {
      printf ("Error: Failed to open file\n");
      return 0;
    }
  MarkovChain *chain = create_markov_chain ();

  if (fill_database (file, words_to_read, chain) == 1)
    {
      printf ("Error: don't work\n");
      free_database (&chain);
      return 0;
    }

  MarkovNode *send=NULL;
  //send = chain->database->first->data;
  int counter = 0;

  while (max_length != counter)
    {
      printf ("Tweet %d:", counter + 1);
      generate_tweet (chain, send, TWENY);
      counter++;
      printf ("\n");
    }
  free_database (&chain);
  fclose (file);
  return 1;
}
