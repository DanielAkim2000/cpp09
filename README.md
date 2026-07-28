*This project has been created as part of the 42 curriculum by demane.*

# CPP Module 09

## Description

**CPP Module 09** applies the STL to three practical problems: looking up historical Bitcoin prices, evaluating Reverse Polish Notation, and sorting with the Ford–Johnson (merge-insertion) algorithm while comparing container performance.

This module contains three exercises:

| Exercise | Focus |
|----------|--------|
| **ex00 — Bitcoin Exchange** | `std::map` database; closest lower date exchange rate |
| **ex01 — Reverse Polish Notation** | Stack-based RPN calculator (`std::stack`) |
| **ex02 — PmergeMe** | Ford–Johnson sort on `std::vector` and `std::deque` with timing |

### Overview

- **btc** loads `data.csv`, then evaluates an input file of `date | value` lines (value in `[0, 1000]`).
- **RPN** evaluates expressions with tokens separated by spaces (`+ - * /` only; no parentheses).
- **PmergeMe** sorts a sequence of positive integers with the same algorithm on two containers and prints both timings.

## Instructions

### Requirements

- A C++ compiler supporting **C++98**
- `make`
- For ex00: a CSV database file (`data.csv`) next to the binary / as expected by the program

### Compilation

```bash
cd ex00   # or ex01 / ex02
make
make clean / fclean / re
```

Flags: `-Wall -Wextra -Werror -std=c++98`.

### Execution

```bash
# ex00
./btc input.txt

# ex01
./RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"

# ex02
./PmergeMe 3 5 9 7 4
./PmergeMe `shuf -i 1-100000 -n 3000 | tr "\n" " "`
```

## Classes & components

| Class | Role |
|-------|------|
| `BitcoinExchange` | Loads rates into `std::map<std::string, float>`; evaluates input lines |
| `Date` helpers | Leap years / days-per-month validation |
| `RPN` | Parses tokens, uses a stack, reports errors on invalid expressions |
| `PmergeMe` | Dual-container Ford–Johnson implementation; displays before/after and µs timings |

### Containers used

| Exercise | Container(s) |
|----------|----------------|
| ex00 | `std::map` |
| ex01 | `std::stack` |
| ex02 | `std::vector` and `std::deque` |

## Resources

### Documentation & references

- [std::map](https://en.cppreference.com/w/cpp/container/map)
- [std::stack](https://en.cppreference.com/w/cpp/container/stack)
- [std::vector](https://en.cppreference.com/w/cpp/container/vector) / [std::deque](https://en.cppreference.com/w/cpp/container/deque)
- [Reverse Polish notation](https://en.wikipedia.org/wiki/Reverse_Polish_notation)
- [Merge-insertion sort (Ford–Johnson)](https://en.wikipedia.org/wiki/Merge-insertion_sort)
- [gettimeofday](https://man7.org/linux/man-pages/man2/gettimeofday.2.html) — timing
- 42 subject: *CPP Module 09*

### Use of AI

AI assistance was used for:

- Drafting and formatting this `README.md` according to the 42 curriculum requirements
- Clarifying wording and structuring documentation sections

No AI-generated code was submitted as a substitute for personal implementation. The C++ sources were written and validated by the student.

---

*42 Paris — Common Core*
