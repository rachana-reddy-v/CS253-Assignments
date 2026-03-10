# Memory-Efficient Versioned File Indexer Using C++

## Overview

The *Memory-Efficient Versioned File Indexer* is a C++ application designed to process and analyze large text-based datasets while maintaining strict memory constraints. The system incrementally reads large files using a *fixed-size buffer* and constructs a *word-level index* that stores the frequency of each unique word.

The program avoids loading the entire file into memory, making it suitable for handling large datasets efficiently. It also supports *multiple versions of datasets*, allowing users to perform analytical queries such as word frequency lookup, top-K frequent words retrieval, and comparison of word frequencies across different dataset versions.

This project demonstrates strong *object-oriented design, disciplined memory management, and the use of important C++ programming concepts* including inheritance, polymorphism, templates, and exception handling.

---

# Features

### Memory-Efficient File Processing
- Uses a fixed-size buffer (256 KB – 1024 KB) to read files incrementally.
- The entire file is never loaded into memory.
- Memory usage depends only on the number of unique words, not file size.

### Word-Level Indexing
- Builds a mapping from each unique word to its frequency.
- Words are contiguous sequences of alphanumeric characters.
- Word matching is case-insensitive.

### Version Management
- Each dataset is indexed as a distinct version.
- Versions are identified using user-provided names.
- Supports multiple versions within the same execution.
- Queries can operate on single versions or compare two versions.

---

# Supported Queries

### Word Count Query
Returns the frequency of a specific word in a given version.

### Top-K Query
Displays the K most frequent words in a specified version.

### Difference Query
Computes the difference in frequency of a word between two versions.

---

# System Architecture

The program follows a *modular object-oriented architecture*, where each class has a clearly defined responsibility.

## Classes

### BaseReader
Abstract base class that defines the interface for reading files. It declares virtual functions for opening a file, reading fixed-size chunks, and closing the file.

### BufferedFileReader
Derived from the `BaseReader` class. Implements file reading using a fixed-size buffer and reads the input file incrementally in chunks.

### Tokenizer
Responsible for extracting words from raw buffer data. It converts characters to lowercase, identifies alphanumeric sequences as words, and handles tokens that are split across buffer boundaries.

### VersionIndex
Represents the word-frequency index for a specific version of a dataset. It maintains a mapping between words and their frequencies and supports operations such as adding words, retrieving counts, computing differences between versions, and retrieving the top-K most frequent words.

### Query
Abstract base class representing a generic query. It defines the interface for executing queries and retrieving the query type.

### WordQuery
Derived from the `Query` class. Executes a query that returns the frequency of a specified word in a given version.

### TopKQuery
Derived from the `Query` class. Executes a query that retrieves the top-K most frequent words from a specified version.

### DiffQuery
Derived from the `Query` class. Executes a query that computes the difference in frequency of a word between two versions.

### ComparePairs
A template comparator structure used for ordering word-frequency pairs when computing the top-K most frequent words.

---

# Prerequisites

Ensure you have the following installed:

- **C++ Compiler (g++)**
- **Git**

```bash
sudo apt-get update
sudo apt-get install -y build-essential git
```

---

# Compilation and Execution

## Clone the Repository

```bash
git clone https://github.com/rachana-reddy-v/CS253-Assignments.git
```

```
cd CS253-Assignments/OOPs-Assignment
```

---

## Compile the Program

```bash
g++ -std=c++17 -O2 file_indexer.cpp -o analyzer
```

---

# Run the Program

## Word Query

```bash
./analyzer --file test_logs.txt --version v1 --buffer 256 --query word --word error
```

Example Output

```
Query: word
Version: v1
Word: error
Count: 605079
Buffer Size (KB): 256
Execution Time (s): 0.843523
```

---

## Top-K Query

```bash
./analyzer --file test_logs.txt --version v1 --buffer 256 --query top --top 10
```

Example Output

```
Query: top
Top-10 words in version v1:
1. devops 1209558
2. debug 605150
3. error 605079
4. info 604266
5. warning 604149
6. orderservice 484437
7. paymentservice 484078
8. authservice 483842
9. searchservice 483162
10. userservice 483125
Buffer Size (KB): 256
Execution Time (s): 0.848306
```

---

## Difference Query

```bash
./analyzer --file1 test_logs.txt --version1 v1 --file2 verbose_logs.txt --version2 v2 --buffer 256 --query diff --word error
```

Example Output

```
Query: diff
Word: error
Difference (v2 - v1): -495377
Buffer Size (KB): 256
Execution Time (s): 1.70038
```

---

# Usage

1. Compile the program using the provided g++ command.
2. Run the executable `analyzer` with the required command-line arguments.
3. Provide the input file(s), version name(s), buffer size, and query type.

---

# Memory Constraints

The program guarantees that:

- Memory usage is independent of file size.
- The entire dataset is never loaded into memory.
- Processing occurs incrementally using a fixed-size buffer.
- Memory usage grows only with the number of unique words.

---

# Acknowledgements

The **Memory-Efficient Versioned File Indexer** was developed by **Rachana Reddy V (Roll No. 240822)**, Department of Computer Science and Engineering, **Indian Institute of Technology Kanpur**.

Special thanks to **Prof. Indranil Saha** and the **teaching assistants of the CS253 course** for their guidance and support throughout the course.
