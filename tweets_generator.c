#include "markov_chain.h"
#include <string.h>

#define TEN 10
#define FIVE 5
#define FOUR 4
#define TWENY 20
#define HUNDRED 101
#define TOUTHEND 1001

static LinkedList *create_linked_list ()
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

static int is_last_period (const void *data)
{
  char *str = (char *) data;
  int len = strlen (str);
  if (len == 0)
    {
      return 0;
    }
  return str[len - 1] == '.';
}

static int string_comp_func (const void *a, const void *b)
{
  const char  *str_a = (const char *) a;
  const char  *str_b = (const char *) b;
  return strcmp (str_a, str_b);
}

static void print_string (void *data)
{
  if(is_last_period(data))
    {
      printf ("%s", (char *) data);
    }
  else{
      printf ("%s ", (char *) data);
    }
}

static void free_string (void *data)
{
  char *string = (char *) data;
  free (string);
}

static void* strdup_func(const void* str) {
  const char* str_casted = (const char*) str;
  size_t len = strlen(str_casted) + 1;
  char* copied_str = malloc(len);
  if (copied_str == NULL) {
      return NULL; // Memory allocation failed
    }
  memcpy(copied_str, str_casted, len);
  return (void*) copied_str;
}


static MarkovChain *create_markov_chain ()
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
  markov_chain->comp_func = string_comp_func;
  markov_chain->copy_func = strdup_func;
  markov_chain->free_data = free_string;
  markov_chain->is_last = is_last_period;
  markov_chain->print_func = print_string;

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
                  return EXIT_FAILURE;
                }
              first = false;
            }
          else
            {
              if (add_to_database (markov_chain, word)==NULL){
                  return EXIT_FAILURE;
                }
              first_node = get_node_from_database (markov_chain, last);
              second_node = get_node_from_database (markov_chain, word);
              if (add_node_to_frequencies_list (first_node->data,
                                                second_node->data,
                                                markov_chain)
                  == false)
                {
                  return EXIT_FAILURE;
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
              return EXIT_SUCCESS;
            }
          word = strtok(NULL, " \t\n\r");
        }
    }
  return EXIT_SUCCESS;
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
  srand (seed);
  FILE *file = fopen (path, "r");
  if (file == NULL)
    {
      printf ("Error: Failed to open file\n");
      return EXIT_FAILURE;
    }
  MarkovChain *chain = create_markov_chain ();

  if (fill_database (file, words_to_read, chain) == 1)
    {
      printf ("Allocation failure: \n");
      free_database (&chain);
      return EXIT_FAILURE;
    }

  MarkovNode *send = NULL;
  //send = chain->database->first->data;
  int counter = 0;

  while (max_length != counter)
    {
      printf ("Tweet %d: ", counter + 1);
      generate_tweet (chain, send, TWENY);
      counter++;
      printf ("\n");
    }
  free_database (&chain);
  fclose (file);
  return EXIT_SUCCESS;
}
