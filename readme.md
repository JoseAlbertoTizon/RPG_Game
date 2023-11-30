# RPG_Game

##This Project was made for our college exam. Our objective was to make a RPG game evolving graph and binarie tree, we used C++ 17 and SFML 2.6.1. librarie´s.

Table of Contents
=================
<!--ts-->
* [About](#About)
* [Installation](#installation)
* [How to Use](#How_to_use)
    * [Prerequisites](#prerequisites)
    * [Local files](#local-files)
    * [Remote files](#remote-files)
<!--te-->

## About

This game revolves around a character navigating on vertices of a graph. Initially, the character has no edges connecting to neighbour vertices. However, as the character progresses,
defeats enemies, and earns money, it can purchase edges for desired vertices. It's important to note that each edge comes with a cost.

The graph algorithms come into play when implementing Dijkstra's algorithm to find the shortest path. The character follows this algorithm to navigate based on the shortest possible
distance between two points, considering the constructed edges.

The tree aspect becomes relevant when the character enters the pause screen by pressing the down button. A tree with boosts appears, and the character can purchase these boosts using the same
money used to buy edges. This allows the character to increase speed and damage to overcome the upcoming 10 waves of enemies and ultimately win the game.

## How to Use

### Prerequisites

Before using this project, ensure you have the following prerequisite installed:

- [SFML Library](https://www.sfml-dev.org/) - Make sure to install the SFML library to enable graphics.

- We did this on windows and the IDE was CLion(2023.2.2 version), so we are going to show steps that we have done:
  -After downloading SFML, extract the paste and put on C´s directories

### Local Files

If you would like to have local background files used in the game presentation, follow these steps:

1. Place your background files in the `...` directory.

2. Ensure that the filenames are correctly referenced in your code.

### Remote Files

??