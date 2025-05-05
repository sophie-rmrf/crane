# 🏗️ A C++ Build System

Crane is a build system for C++ with which you can quickly create projects, avoiding unnecessary complexity (like CMake). 

Crane can also install `g++` compiler for you. 

## How to use

You need the `config.crane.json` file to work with Crane.

Example of that file:

```json
{
    "source_files": ["main.cpp"],

    "profiles": {
        "release": {
            "optimize_level": 3,
            "output_name": "a.exe"
        },
        "debug": {
            "optimize_level": 0,
            "output_name": "a.exe",
            "define": ["DEBUG"]
        }
    }
}
```
