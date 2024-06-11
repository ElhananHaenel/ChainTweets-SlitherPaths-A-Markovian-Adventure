# Markov-Chain-Text-Generator-and-snakes-and-ladders


## Overview

This project was developed as part of an exercise for a programming workshop. It involves implementing generic programming concepts in C and C++ to create a library for generating tweets and simulating a snakes and ladders game. The core component of the project is the implementation of a Markov chain for generating text.

## Installation

To compile the project, you'll need a C compiler (such as GCC) and a C++ compiler (such as g++). 

## Usage

### Tweet Generator

The tweet generator program reads a corpus of text and generates tweets using a Markov chain model.

#### Usage:
```bash
./tweet_generator <corpus_file>
```

### Snakes and Ladders Simulator

The snakes and ladders simulator generates possible paths for a player to traverse on a snakes and ladders board.

#### Usage:
```bash
./snakes_and_ladders <seed> <number_of_paths>
```

## Markov Chain

The core component of the project is the implementation of a Markov chain, which is used in the tweet generator program. The Markov chain model analyzes a corpus of text and generates new text based on the probability of transitioning from one word to another.


