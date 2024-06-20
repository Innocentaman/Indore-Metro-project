# Indore Metro Route Planner

## Overview
This project implements a route planner for the Indore Metro system using C++. It allows users to find the shortest path between two metro stations, display the metro map, and list all stations available. The program uses Dijkstra's algorithm for pathfinding based on real distances between stations in Indore.

## Features
- **Graph Representation**: Metro stations are represented as vertices connected by edges with real distances.
- **Shortest Path Calculation**: Uses Dijkstra's algorithm to compute the shortest path between two stations based on kilometers.
- **Interactive User Interface**: Allows users to input source and destination stations dynamically.

## Usage
1. **Display Metro Map**: Shows all stations and their connections with distances.
2. **Find Shortest Path**: Enter source and destination stations to find the shortest route and display interchanges.
3. **List All Stations**: Prints a list of all metro stations available.

## Dependencies
- C++ compiler supporting C++11 or later.

## Contributors
- Aman Vyas

## Example Usage
```bash
$ ./metro

         Indore Metro Route Planner

Choose an option:
1. Display Metro Map
2. Find Shortest Path
3. List All Stations
4. Exit

Enter your choice: 2

Enter source station: Vijay Nagar
Enter destination station: Geeta Bhawan

Shortest Path from Vijay Nagar to Geeta Bhawan: Vijay Nagar  -> Bhawarkua  -> Regal Square  -> Rajwada  -> Indore Junction  -> Vijay Nagar Square  -> Palasia  -> Geeta Bhawan     16 km
Interchanges: Vijay Nagar  -> Indore Junction  -> Palasia  
```

This README file provides an overview of the Indore Metro Route Planner project, detailing its features, usage instructions, dependencies, and contributors.
