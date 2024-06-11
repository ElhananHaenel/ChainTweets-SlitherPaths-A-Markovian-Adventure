#include "markov_chain.h"

#define NINE_TEEN 19
#define HUNDRED 101
#define TWENY 20

Node *add_to_database (MarkovChain *markov_chain, char *data_ptr)
{
  //printf("\n%s and ",data_ptr);
  // Traverse the linked list to see if data_ptr already exists in the database
  Node *current_node = markov_chain->database->first;
  while (current_node != NULL)
    {
      MarkovNode *current_markov_node = current_node->data;
      if (strcmp (current_markov_node->data, data_ptr) == 0)
        {
          // data_ptr already exists in the database, so return its node
          return current_node;
        }
      current_node = current_node->next;
    }
  //printf("%s\n",data_ptr);

  // data_ptr doesn't exist in the database, so create a new node for it
  MarkovNode *new_markov_node = malloc (sizeof (MarkovNode));
  if (new_markov_node == NULL)
    {
      // Memory allocation failure
      return NULL;
    }

  strcpy (new_markov_node->data, data_ptr);
  new_markov_node->frequencies_list = NULL;
  new_markov_node->size_list = 0;

  // Add the new node to the end of the linked list
  if (add (markov_chain->database, new_markov_node) != 0)
    {
      // Failed to add the new node to the linked list
      free (new_markov_node);
      return NULL;
    }

  // Return the new node
  return markov_chain->database->last;
}

Node *get_node_from_database (MarkovChain *markov_chain, char *data_ptr)
{
  if (markov_chain == NULL || data_ptr == NULL)
    {
      return NULL;
    }

  // Check if the state is already in the database
  Node *current_node = markov_chain->database->first;
  while (current_node != NULL)
    {
      if (strcmp (current_node->data->data, data_ptr) == 0)
        {
          return current_node;
        }
      current_node = current_node->next;
    }
  return NULL;
}

bool
add_node_to_frequencies_list (MarkovNode *first_node, MarkovNode *second_node)
{
  if (first_node == NULL || second_node == NULL)
    {
      return false;
    }
  if (first_node->frequencies_list == NULL)
    {
      first_node->frequencies_list = (MarkovNodeFrequency *)
          calloc (1, sizeof (MarkovNodeFrequency));
      if (first_node->frequencies_list == NULL)
        {
          return false;
        }
      first_node->frequencies_list[0].markov_node = second_node;
      first_node->frequencies_list[0].frequency = 1;
      first_node->size_list = 1;
      first_node->total_len=1;
      return true;
    }
  int size = first_node->size_list;
  for (int i = 0; i < size; i++)
    {
      if (first_node->frequencies_list[i].markov_node == second_node)
        {
          first_node->frequencies_list[i].frequency++;
          first_node->total_len++;
          return true;
        }
    }
  // Second node not found in the frequencies list, need to add it
  MarkovNodeFrequency *new_frequencies_list = (MarkovNodeFrequency *)
      realloc (first_node->frequencies_list,
               sizeof (MarkovNodeFrequency) *
               (first_node->size_list + 1));
  if (new_frequencies_list == NULL)
    {
      return false;
    }
  first_node->frequencies_list = new_frequencies_list;
  first_node->frequencies_list[first_node->size_list].markov_node =
      second_node;
  first_node->frequencies_list[first_node->size_list].frequency = 1;
  first_node->size_list++;
  first_node->total_len++;
  return true;
}

void free_database (MarkovChain **ptr_chain)
{
  if (ptr_chain == NULL || *ptr_chain == NULL)
    {
      return;
    }
  LinkedList *database = (*ptr_chain)->database;
  while (database->first != NULL)
    {
      Node *current = database->first;
      database->first = current->next;
      MarkovNode *markov_node = current->data;
      free (markov_node->frequencies_list);
      markov_node->frequencies_list = NULL;

      free (markov_node);
      markov_node = NULL;

      free (current);
      current = NULL;
    }
  free (database);
  (*ptr_chain)->database = NULL;

  free (*ptr_chain);
  *ptr_chain = NULL;
}

int get_random_number (int max_number)
{
  return rand () % max_number;
}

MarkovNode *get_first_random_node (MarkovChain *markov_chain)
{
  Node *node = NULL;
  int size = 0;
  int rand = 0;
  int len = 0;

  while (true)
    {
      node = markov_chain->database->first;
      size = markov_chain->database->size;
      rand = get_random_number (size);
      for (int i = 0; i < rand; i++)
        {
          node = node->next;
        }
      len = strlen (node->data->data);
      //printf("%s\n",node->data->data);
      if (node->data->data[len - 1] != '.')
        {
          return node->data;
        }
    }
}

MarkovNode *get_next_random_node (MarkovNode *state_struct_ptr)
{
  //int size = 0;
  //int counter = 0;
  unsigned int rand = 0;
  //size = state_struct_ptr->size_list;

  rand = get_random_number (state_struct_ptr->total_len);
  int i = 0;
  unsigned int counter1 = 0;
  while (true)
    {
      counter1 += state_struct_ptr->frequencies_list[i].frequency;
      if (counter1 > rand)
        {
          return state_struct_ptr->frequencies_list[i].markov_node;
        }
      //rand = rand - state_struct_ptr->frequencies_list[i].frequency;
      i++;
    }
}

void
generate_tweet (MarkovChain *markov_chain, MarkovNode *first_node, int
max_length)
{

  char list_word[TWENY][HUNDRED];
  int i = 1;
  int len = 0;
  int count_num_words = 1;
  MarkovNode *general = NULL;

  i = 1;
  if (first_node == NULL)
    {
      general = get_first_random_node (markov_chain);
    }
  else
    {
      general = first_node;
    }

  strcpy (list_word[0], general->data);
  count_num_words = 1;
  while (i != max_length)
    {
      general = get_next_random_node (general);
      strcpy (list_word[i], general->data);
      len = strlen (list_word[i]);
      if ((list_word[i][len - 1] == '.'))
        {
          i = NINE_TEEN;
        }
      i++;
      count_num_words++;
    }

  for (int j = 0; j < count_num_words; j++)
    {
      printf (" %s", list_word[j]);
    }

}




