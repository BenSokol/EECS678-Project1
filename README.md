# EECS678-Project1
[![Build Status](https://travis-ci.com/BenSokol/EECS678-Project1.svg?branch=master)](https://travis-ci.com/BenSokol/EECS678-Project1) [![Codacy Badge](https://api.codacy.com/project/badge/Grade/9be4f74279de46b89bcc44d5cd41d47c)](https://www.codacy.com/app/BenSokol/EECS678-Project1?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=BenSokol/EECS678-Project1&amp;utm_campaign=Badge_Grade) ![GitHub](https://img.shields.io/github/license/BenSokol/EECS678-Project1.svg)

## Project Specification
The project specification is located [here](https://web.archive.org/web/20190925231148/http://www.ittc.ku.edu/~kulkarni/teaching/EECS678/projects/quash.pdf).

## Requirements
|             |          Required          |
| ----------- | -------------------------- |
| OS          | Linux<br>macOS             |
| Compiler    | gcc >= 9<br>clang >= 9     |
| C Version   | c11                        |
| C++ Version | c++2a                      |

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
| --debug               | -d         | Enable debug mode - Additional output to std::cerr                                                                                       |
| --debug-file FILENAME |            | Requires Debug Mode<br>Output debug stream to FILENAME and std::cerr<br>If --debug-no-stderr is enabled, then only output to FILENAME    |
| --debug-no-stderr     |            | Requires Debug Mode<br>Disable output to std::cerr (used to only output debug messages to file)                                          |
| --help                | -h         | Display help, then exit                                                                                                                  |
| --print-env           |            | Prints environment after initializing shell (before giving first prompt)                                                                 |

## License
[MIT](https://github.com/BenSokol/EECS678-Project1/blob/master/LICENSE).
