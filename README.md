# 🏗️ A C++ Build System

Crane is a build system for C++ with which you can quickly create projects, avoiding unnecessary complexity (like CMake). 

Crane can also install `g++` compiler for you. 

## How to use

If you do not have a compiler, run the `crane setup` command. (cmd must be run as Administrator)

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

To run Crane projects, you must use the `crane run <profile>` command, where `<profile>` is the name of the profile in the configuration. For example, `crane run debug` or `crane run release`.
