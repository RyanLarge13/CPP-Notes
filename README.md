# CPP-Notes

CPP Notes is a console application built with primarily c++.

This application is meant to be a console side app that works seamlessly in conjunction
with

1. [Electron Notes for Desktop](https://github.com/RyanLarge13/Electron-Notes/)
2. [Native Notes for Android](https://github.com/RyanLarge13/Native-Notes/)

This application is still in the making and will primarily target Linux systems.

When finished the console application will allow users to:

1. Save notes locally on their computers file system in a custom folder.
2. Synchronize notes from your local environment to your server environment & vise versa
3. Includes safe and encrypted data handling locally and over network
4. Access your notes from the desktop application, mobile application or terminal

## Compile && Run

Make sure dependencies are first installed on your system

Check out the dependencies section below
[Deps](#Deps)

```
clang++ -o bin/myProg src/main.cpp -lncurses -lcurl
```

Ensure Clang Can Find the Libraries: If the libraries are installed in non-standard locations, you might need to specify the path to them. You can do this using the -L flag to specify the directory containing the libraries and the -I flag for the include directory. For example:

```
clang++ -o bin/myProg src/main.cpp -I/usr/include -L/usr/lib -lncurses -lcurl
```

Adjust /usr/include and /usr/lib to the correct paths on your system

## Deps

- sudo apt-get install libcurl4-openssl-dev
- sudo apt-get install ncurses
