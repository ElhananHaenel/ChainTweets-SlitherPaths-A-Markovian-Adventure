#include <string.h> // For strlen(), strcmp(), strcpy()
#include "markov_chain.h"

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

#define EMPTY -1
#define BOARD_SIZE 100
#define MAX_GENERATION_LENGTH 60

#define DICE_MAX 6
#define NUM_OF_TRANSITIONS 20

#define TEN 10
#define HUNDRED 100
#define SIXTEN 60

/**
 * represents the transitions by ladders and snakes in the game
 * each tuple (x,y) represents a ladder from x to if x<y or a snake otherwise
 */
const int transitions[][2] = {{13, 4},
                              {85, 17},
                              {95, 67},
                              {97, 58},
                              {66, 89},
                              {87, 31},
                              {57, 83},
                              {91, 25},
                              {28, 50},
                              {35, 11},
                              {8,  30},
                              {41, 62},
                              {81, 43},
                              {69, 32},
                              {20, 39},
                              {33, 70},
                              {79, 99},
                              {23, 76},
                              {15, 47},
                              {61, 14}};

/**
 * struct represents a Cell in the game board
 */
typedef struct Cell
{
    int number; // Cell number 1-100
    int ladder_to;  // ladder_to represents the jump of the ladder in
    // case there is one from this square
    int snake_to;  // snake_to represents the jump of the snake in case
    // there is one from this square
    //both ladder_to and snake_to should be -1 if the Cell doesn't have them
} Cell;

/** Error handler **/
static int handle_error (char *error_msg, MarkovChain **database)
{
  printf ("%s", error_msg);
  if (database != NULL)
    {
      free_database (database);
    }
  return EXIT_FAILURE;
}

static int create_board (Cell *cells[BOARD_SIZE])
{
  for (int i = 0; i < BOARD_SIZE; i++)
    {
      cells[i] = malloc (sizeof (Cell));
      if (cells[i] == NULL)
        {
          for (int j = 0; j < i; j++)
            {
              free (cells[j]);
            }
          handle_error (ALLOCATION_ERROR_MASSAGE, NULL);
          return EXIT_FAILURE;
        }
      *(cells[i]) = (Cell) {i + 1, EMPTY, EMPTY};
    }

  for (int i = 0; i < NUM_OF_TRANSITIONS; i++)
    {
      int from = transitions[i][0];
      int to = transitions[i][1];
      if (from < to)
        {
          cells[from - 1]->ladder_to = to;
        }
      else
        {
          cells[from - 1]->snake_to = to;
        }
    }
  return EXIT_SUCCESS;
}

/**
 * fills database
 * @param markov_chain
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int fill_database (MarkovChain *markov_chain)
{
  Cell *cells[BOARD_SIZE];
  if (create_board (cells) == EXIT_FAILURE)
    {
      return EXIT_FAILURE;
    }
  MarkovNode *from_node = NULL, *to_node = NULL;
  size_t index_to;
  for (size_t i = 0; i < BOARD_SIZE; i++)
    {
      add_to_database (markov_chain, cells[i]);
    }
  for (size_t i = 0; i < BOARD_SIZE; i++)
    {
      from_node = get_node_from_database (markov_chain, cells[i])->data;
      if (cells[i]->snake_to != EMPTY || cells[i]->ladder_to != EMPTY)
        {
          index_to = MAX(cells[i]->snake_to, cells[i]->ladder_to) - 1;
          to_node = get_node_from_database (markov_chain,
                                            cells[index_to])->data;
          add_node_to_frequencies_list (from_node,
                                        to_node, markov_chain);
        }
      else
        {
          for (int j = 1; j <= DICE_MAX; j++)
            {
              index_to = ((Cell *) (from_node->data))->number + j - 1;
              if (index_to >= BOARD_SIZE)
                {
                  break;
                }
              to_node = get_node_from_database (markov_chain,
                                                cells[index_to])->data;
              add_node_to_frequencies_list (from_node,
                                            to_node, markov_chain);
            }
        }
    }
  for (size_t i = 0; i < BOARD_SIZE; i++)
    {
      free (cells[i]);
    }
  return EXIT_SUCCESS;
}

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

static void print_cell (void *data)
{
  Cell *cell = (Cell *) data;
  if (cell->ladder_to == -1 && cell->snake_to == -1)
    {
      if (cell->number != HUNDRED)
        {
          printf ("[%d] -> ", cell->number);
        }
      else
        {
          printf ("[%d]", cell->number);
        }
      return;
    }
  if (cell->ladder_to > cell->snake_to)
    {
      printf ("[%d]-ladder to %d -> ", cell->number, cell->ladder_to);
    }
  else
    {
      printf ("[%d]-snake to %d -> ", cell->number, cell->snake_to);
    }
}

static void free_cell (void *data)
{
  free ((Cell *) data);
}

static int cell_comp_func (const void *a, const void *b)
{
  const Cell *cell_a = (const Cell *) a;
  const Cell *cell_b = (const Cell *) b;
  if (cell_a->number == cell_b->number)
    {
      return 0;
    }
  return 1;
}

static void *copy_cell (const void *data)
{
  const Cell *src = (const Cell *) data;
  Cell *dst = malloc (sizeof (Cell));
  if (dst)
    {
      dst->number = src->number;
      dst->ladder_to = src->ladder_to;
      dst->snake_to = src->snake_to;
      return dst;
    }
  return NULL;
}

static int is_last_cell (const void *data)
{
  const Cell *cell = (const Cell *) data;
  return cell->number == HUNDRED;
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
  markov_chain->comp_func = cell_comp_func;
  markov_chain->copy_func = copy_cell;
  markov_chain->free_data = free_cell;
  markov_chain->is_last = is_last_cell;
  markov_chain->print_func = print_cell;

  return markov_chain;
}

/**
 * @param argc num of arguments
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int main (int argc, char *argv[])
{
  if (argc != 3)
    {
      printf ("Usage: not good param");
      return EXIT_FAILURE;
    }
  char *p;
  int seed = strtol (argv[1], &p, TEN);
  srand (seed);
  int num_cours = strtol (argv[2], &p, TEN);
  MarkovChain *chain = create_markov_chain ();
  if (fill_database (chain) == 1)
    {
      free_database (&chain);
      printf ("Allocation failure: \n");
      return EXIT_FAILURE;
    }

  MarkovNode *send = chain->database->first->data;
  int counter = 0;

  while (num_cours != counter)
    {
      printf ("Random Walk %d: ", counter + 1);
      generate_tweet (chain, send, SIXTEN);
      counter++;
      printf ("\n");
    }

  free_database (&chain);
  return EXIT_SUCCESS;

}
