# Indore Metro Management System

## Overview

The Indore Metro Management System is a comprehensive application designed for managing metro stations, routes, and helpline information. It provides functionalities for users to find the shortest path between stations and for administrators to manage the metro network and helplines. This system is built using C++ and leverages file I/O for persistent data storage.

## Features

### For Users

- **User Login and Registration:**
  - Secure login with username and password.
  - Register new users and store credentials.

- **Find Shortest Path:**
  - Calculate the shortest path between two metro stations using Dijkstra's algorithm.
  - Display the total distance and estimated travel charge based on distance.

- **View Important Numbers:**
  - Display helpline numbers for specific services at a given metro station.
  - Services include Police, Fire, Ambulance, and Cleaners.

- **Unregister Account:**
  - Option to unregister from the system, removing user data.

- **Logout:**
  - Logout from the user session and end the program.

### For Admins

- **Admin Login:**
  - Secure login with admin credentials.

- **Manage Helplines:**
  - Update helpline numbers for various services at any station.
  - Add helplines for new stations as they are introduced to the network.

- **Manage Edges:**
  - Add new routes (edges) between stations with specified distances.
  - Remove existing routes.
  - Update distances for existing routes.

- **Logout:**
  - Logout from the admin session and end the program.

### Additional Features

- **Display Available Stations:**
  - List all available metro stations to users upon their first login.

## Technologies Used

- **Programming Language:**
  - C++

- **Data Structures:**
  - `map` and `unordered_map` for storing user credentials, admin credentials, helplines, and graph edges.
  - `vector` for representing adjacency lists in the graph.
  - `set` for implementing priority queues in Dijkstra's algorithm.

- **File I/O:**
  - `ifstream` and `ofstream` for loading and saving user data, admin credentials, helpline information, and graph edges from/to text files.

- **Algorithm:**
  - Dijkstra's algorithm for finding the shortest path between two stations.

## Files

- `users.txt`: Stores user credentials (username and password).
- `admin.txt`: Stores admin credentials (username and password).
- `helplines.txt`: Contains helpline numbers for each station and service.
- `edges.txt`: Defines the connections and distances between metro stations.

## How to Run

1. Compile the C++ program using a suitable compiler:
   ```sh
   g++ -o metro_system metro_system.cpp
