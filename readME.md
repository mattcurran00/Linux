# Linux Spotlight

This is a lightweight, simple enough application launcher for Linux machines. This particular program was developed on a Linux Mint machine.
The main reason for making this was my usage of my work provided Mac. The spotlight search became one of my most used shortcuts on the machine, and I kept 
reaching for it every time I opened up my personal laptop.

This little project is written in C, as off the back of a module I took in first year, I never really got to go back to C in work or in other modules. Alas, here we are. 
I made use of a library "GTK 3". GTK provides a graphical interface. 

## Features

* Search installed Linux applications
* Case-insensitive application search
* Display up to 10 matching applications
* Launch applications directly from the search results
* Launch the selected result using `Enter`
* Close the launcher using `Escape`
* Simple GTK-based graphical interface
* Uses the system's application metadata through GLib/GIO

## Technologies

* **C**
* **GTK 3** — graphical user interface
* **GLib** — data structures, memory management and utility functions
* **GIO** — discovering and launching installed applications
* **GCC** — compilation
* **Make** — build automation

## Project Structure

```text
Linux/
├── src/
│   ├── main.c
│   ├── launcher.c
│   ├── launcher.h
│   ├── search.c
│   └── search.h
├── Makefile
└── README.md
```

### Architecture

The project separates the graphical interface from the search functionality.

**`main.c`**

Responsible for creating the `GtkApplication` and starting the application.

**`launcher.c`**

Responsible for the GTK interface, including:

* Creating the launcher window
* Handling user input
* Displaying search results
* Handling keyboard events
* Launching applications

**`launcher.h`**

Defines the `Launcher` structure and exposes the launcher functionality required by other source files.

**`search.c`**

Contains the application search logic. It receives the available applications and a search query and returns matching applications.

**`search.h`**

Defines the public interface for the search module.

## Building

### Dependencies

On Debian/Ubuntu-based distributions, install the required development packages with:

```bash
sudo apt install build-essential libgtk-3-dev
```

### Build

Clone the repository and build using Make:

```bash
git clone git@github.com:mattcurran00/Linux.git
cd Linux
make
```

### Run

```bash
make run
```

Or run the compiled executable directly:

```bash
./spotlight
```

### Clean

Remove the compiled executable:

```bash
make clean
```

## How It Works

When the application starts, it uses GIO to retrieve the applications registered with the Linux desktop environment.

```c
launcher->applications = g_app_info_get_all();
```

When text is entered into the search field, the query is passed to the search module.

The search module:

1. Iterates through the available applications
2. Filters applications that should not be displayed
3. Retrieves their display names
4. Performs a case-insensitive substring search
5. Returns matching applications

The launcher then converts those results into GTK list rows.

Selecting a result or pressing `Enter` launches the application using:

```c
g_app_info_launch()
```

## Current Limitations

This project is currently intentionally simple. The search algorithm performs basic substring matching rather than fuzzy matching or relevance-based ranking.

For example:

```text
fire → Firefox
```

will match because `"fire"` occurs within `"Firefox"`.

However, more advanced searches such as typo-tolerant or fuzzy matching are not currently supported.

## Planned Improvements

Some planned improvements include:

* [ ] Fuzzy search
* [ ] Search result ranking
* [ ] Application icons
* [ ] Better memory/resource cleanup
* [ ] Unit tests for the search functionality
* [ ] Configuration options
* [ ] Improved visual styling
* [ ] Global keyboard shortcut that is not manually configured
* [ ] More robust error handling
* [ ] Support for additional Linux desktop environments

## What I'm Learning

This project, as mentioned before, was mostly a reason to learn lower lever Linux development. Its a branch out from the simple apps we made in first year, 
and most importantly, it gave me a reason to dig back into C and better my understanding of it. 

Areas explored so far include:

* C project organisation
* Header files and compilation units
* Separation of concerns
* Static functions and encapsulation
* Manual memory management
* Pointers and structures
* GTK event-driven programming
* GLib data structures
* GIO application discovery and launching
* Linux desktop application integration
* Makefiles and build automation

## Thank You!
As of now, this is so, so simple that I don't expect anyone to get any use out of it. However, wherever this goes, if you have any reason to contact about this program, please go ahead:
* Email: mattcurran1212@gmail.com / C23418016@mytudublin.ie (DO NOT USE post May 2027)