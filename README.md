# EECS678-Project1
[![Build Status](https://travis-ci.com/BenSokol/EECS678-Project1.svg?branch=master)](https://travis-ci.com/BenSokol/EECS678-Project1) [![Codacy Badge](https://api.codacy.com/project/badge/Grade/9be4f74279de46b89bcc44d5cd41d47c)](https://www.codacy.com/app/BenSokol/EECS678-Project1?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=BenSokol/EECS678-Project1&amp;utm_campaign=Badge_Grade) ![GitHub](https://img.shields.io/github/license/BenSokol/EECS678-Project1.svg)

## Project Specification
The project specification is located [here](https://web.archive.org/web/20190925231148/http://www.ittc.ku.edu/~kulkarni/teaching/EECS678/projects/quash.pdf).

## Feature status
See [here](https://github.com/BenSokol/EECS678-Project1/blob/master/FEATURES.md).

## Requirements
|             |          Prefered          |          Required          |
| ----------- | -------------------------- | -------------------------- |
| OS          |                            | Linux<br>macOS             |
| Compiler    | gcc >= 9<br>clang >= 9     | gcc >= 8<br>clang >= 7     |
| C Version   |                            | c11                        |
| C++ Version | c++2a                      | c++17                      |

## To compile
```bash
# Clone project
git clone https://github.com/BenSokol/EECS678-Project1 quash

# Change directories into project folder
cd quash

# Checkout submodules
git submodule update --init --recursive

# Compile project using GNU Make
make
```
For compilation help, see [this](https://github.com/BenSokol/build-tools) page.

## Usage
```bash
./quash [FLAGS]
```

### Flags
| Flag                  | Short Flag | Description                                                                                                                              |
|:----------------------|:-----------|:-----------------------------------------------------------------------------------------------------------------------------------------|
| --help                | -h         | Print help (usage) message and exit                                                                                                      |
| --debug               | -d         | Enable debug mode - Additional output to std::cerr and logs/Log \time since epocH\>.log                                                  |
| --debug-file          | -df        | Enable debug mode - Additional output to logs/Log \time since epocH\>.log                                                                |
| --debug-stderr        | -de        | Enable debug mode - Additional output to std::cerr                                                                                       |
| --help                | -h         | Display help, then exit                                                                                                                  |
| --print-env           |            | Prints environment after initializing shell (before giving first prompt)                                                                 |

#### Debug
Debug mode has 2 levels of verbosity.
```bash
./quash          # Normal mode - no debug output
./quash -d       # Verbosity level 1
./quash -d -d    # Verbosity level 2
./quash -df -df  # Verbosity level 2 (output only to file)
```

## License
[MIT](https://github.com/BenSokol/EECS678-Project1/blob/master/LICENSE).
