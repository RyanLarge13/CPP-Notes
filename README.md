📝 CPP Notes

<p align="center">
  <img src="assets/logo.jpg" alt="CPP Notes Logo" width="180">
</p><p align="center">
  <strong>A lightweight terminal-based notes application written in C++.</strong>
</p><p align="center">
  Create, organize, navigate, and eventually synchronize notes without leaving the terminal.
</p>---

«[!IMPORTANT]
CPP Notes is currently under active development.

This project began primarily as a way to learn and experiment with C++. As a result, parts of the codebase are still being reorganized, refactored, documented, and standardized.

Expect bugs, incomplete functionality, and breaking changes while development continues.»

📖 About

CPP Notes is a terminal-based note-taking application built primarily in C++.

The goal is to create a small but capable command-line notes system that can work alongside my other notes applications while providing fast access directly from a terminal.

Rather than being a collection of isolated C++ exercises, CPP Notes is being developed as a real application where I can experiment with concepts such as:

- C++ classes and application architecture
- Filesystem manipulation
- File I/O
- Terminal interfaces
- User input and validation
- Configuration management
- HTTP/network communication
- Cross-platform development
- Error handling
- Local and remote data synchronization

The project currently targets Linux first, with broader platform support being considered as the project matures.

---

✨ Project Goals

CPP Notes is intended to eventually provide:

- 📝 Create and edit notes directly from the terminal
- 📂 Organize notes using folders and nested directories
- 🧭 Navigate a note library from an interactive CLI
- 💾 Store notes locally on the filesystem
- 🔄 Push, pull, and synchronize notes with a remote server
- 🔐 Provide safe handling of locally stored user data
- ⚙️ Maintain user and application configuration
- 🖥️ Work alongside desktop and mobile versions of the notes application
- 🐧 Provide a fast terminal-first workflow on Linux

The long-term goal is to make the terminal client a fully usable member of a larger notes ecosystem rather than simply a standalone C++ experiment.

---

🚧 Current Status

Early Development / Learning Project

CPP Notes is functional in several areas, but it should not currently be considered production-ready.

Some portions of the application are implemented while others are experimental, incomplete, or awaiting refactoring.

Current development work includes:

- Cleaning up and reorganizing the project structure
- Standardizing code formatting
- Improving comments and documentation
- Refactoring older experimental code
- Improving memory/resource management
- Expanding the terminal text editor
- Completing note and folder operations
- Improving configuration handling
- Developing remote synchronization
- Improving error handling and input validation
- Making compilation and installation easier
- Expanding cross-platform support

The repository represents the actual evolution of the project, so older code may not always follow the conventions used by newer portions of the application.

---

🛠️ Technologies

CPP Notes currently makes use of:

Technology| Purpose
C++| Core application
C++ Filesystem| File and directory management
ncurses| Terminal-based text editing/interface
libcurl| HTTP/network communication
Clang / clang++| Primary compiler used during development
Bash| Development/build helper scripts

---

📁 Project Structure

CPP-Notes/
├── assets/
│   └── logo.jpg
│
├── bin/
│   └── myProg
│
├── common/
│   └── *Instance.h
│
├── includes/
│   ├── colors.h
│   ├── configManager.h
│   ├── exceptionHandler.h
│   ├── fileManager.h
│   ├── httpHandler.h
│   ├── ioHandler.h
│   ├── localAuth.h
│   ├── stateManager.h
│   └── validator.h
│
├── src/
│   ├── main.cpp
│   └── resources/
│       └── colors.cpp
│
├── .gitignore
├── clang-format
├── notes.sh
└── README.md

«The current directory structure is temporary and will likely change as the application is refactored.»

---

📝 Notes and Storage

CPP Notes stores notes as files on the local filesystem.

The application currently recognizes notes using the custom:

.wn

file extension.

Folders are represented using normal filesystem directories, allowing the application to navigate and organize notes using the host operating system's filesystem.

This keeps local storage intentionally simple while the application continues to evolve.

---

⌨️ Terminal Editor

CPP Notes includes an experimental terminal text editor built with ncurses.

The editor currently provides the basic foundation for entering note content directly inside the application rather than requiring an external editor.

The editor is still under development and is expected to gain significantly more functionality over time.

---

🌐 Synchronization

One of the larger goals of CPP Notes is synchronization between local notes and a remote notes server.

The project already contains early HTTP handling built around libcurl, with the eventual goal of supporting operations such as:

Local Notes
     │
     ├── Push ─────────► Remote Server
     │
     ├── Pull ◄───────── Remote Server
     │
     └── Sync ◄────────► Remote Server

Synchronization is currently a work in progress and should not yet be relied upon for important data.

---

🔗 Notes Ecosystem

CPP Notes is intended to eventually work alongside other clients for the same notes system:

- Electron Notes — Desktop client
- Native Notes — Android/mobile client
- CPP Notes — Terminal client

The idea is to eventually allow the same notes to be accessed from a graphical desktop application, a mobile device, or directly from a terminal.

---

🚀 Building the Project

Requirements

You will currently need:

- A C++ compiler such as "clang++"
- "libcurl"
- "ncurses"
- "pkg-config"
- A Linux or Unix-like development environment

Debian / Ubuntu

Install the primary dependencies with:

sudo apt update
sudo apt install clang libcurl4-openssl-dev libncurses-dev pkg-config

---

Compile

From the project root:

mkdir -p bin

clang++ src/main.cpp -o bin/myProg \
    $(pkg-config --cflags --libs libcurl) \
    -lncurses

Depending on your system and library locations, additional include or library paths may be required.

For example:

clang++ src/main.cpp \
    -o bin/myProg \
    -I/usr/include \
    -L/usr/lib \
    -lcurl \
    -lncurses

---

Run

After compiling:

./bin/myProg

---

🗺️ Roadmap

The exact roadmap is still evolving, but major goals include:

- [x] Basic terminal application
- [x] Local filesystem navigation
- [x] Custom note files
- [x] Folder creation
- [x] Basic ncurses text input
- [x] Configuration-management foundation
- [x] HTTP/libcurl foundation
- [ ] Complete note editing
- [ ] Complete file CRUD operations
- [ ] Complete folder CRUD operations
- [ ] Reliable push/pull synchronization
- [ ] Conflict-aware synchronization
- [ ] Improved local authentication
- [ ] Secure handling of sensitive configuration
- [ ] Improved terminal UI
- [ ] Better error reporting
- [ ] Automated build system
- [ ] Tests
- [ ] Refactored project structure
- [ ] Consistent formatting and documentation
- [ ] Easier installation
- [ ] Broader platform support

---

🧪 Why This Project Exists

CPP Notes is both an application and a learning project.

It gives me a place to learn C++ by solving problems that exist in a real program instead of limiting that learning to isolated examples.

That means the repository also documents my progression with the language.

Some early design decisions will be replaced. Some code will be rewritten. Interfaces will change. Better approaches will replace things that worked well enough while I was learning them.

That evolution is part of the project.

My long-term intention is to gradually turn CPP Notes from an experimental learning project into a clean, reliable, fully functioning terminal application.

---

🤝 Contributions

CPP Notes is primarily a personal learning project, but suggestions, bug reports, and constructive feedback are welcome.

Because the architecture is still changing significantly, large contributions may be difficult to integrate until the project becomes more stable.

---

⚠️ Disclaimer

CPP Notes is currently experimental software.

Do not use the application as the only storage location for important information. Features involving file manipulation, authentication, networking, and synchronization are still under development.

---

👨‍💻 Author

Developed by Ryan Large.

Built as part of an ongoing journey into C++, terminal application development, filesystem programming, and lower-level software development.

---

<p align="center">
  <strong>CPP Notes</strong><br>
  Notes without leaving the terminal.
</p>