# Markov Chain Text Generator

## Overview

This project involves creating a text generator based on Markov Chains, a fundamental concept in Natural Language Processing (NLP). The goal is to generate new sentences or "tweets" based on a given corpus of existing tweets.


## Introduction

### Background

Natural Language Processing (NLP) is a critical field in computer science, focusing on the interaction between computers and human languages. Common applications include virtual assistants like Google Assistant, Siri, and Alexa. In this project, we explore another aspect of NLP: generating new sentences based on an existing corpus using Markov Chains.

### Markov Chains

A Markov Chain is a statistical model that represents a sequence of possible events, where the probability of each event depends only on the state attained in the previous event. For text generation, each word in a sentence is a state, and the next word is chosen based on the probability distribution derived from the input corpus.

## Project Structure

The project consists of several stages:
1. Learning Phase (Part A): Read the input corpus and build a data structure to store the words and their frequencies.
2. Input Phase (Part B): Process the input file and populate the data structure.
3. Output Phase (Part C): Generate new sentences based on the Markov Chain model.


#### Generating Tweets

Use the Markov Chain to generate new sentences by starting with a random word and following the chain based on the stored frequencies until a sentence-ending word is reached.


## Compilation and Execution

### Compilation

To compile the project, use the provided makefile or the following command:

```bash
gcc -o tweets_generator tweets_generator.c linked_list.c markov_chain.c
```

### Execution

Run the executable with the required parameters:

```bash
./tweets_generator <seed> <number_of_tweets> <path_to_corpus> [<words_to_read>]
```

Example:

```bash
./tweets_generator 454545 100 path-to-file/text_corpus.txt 30
```


