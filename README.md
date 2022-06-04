
# Fiszki / Flashcards 🌐

📌 Simple flashcards concept based learning app written in C with the use of GTK layout 
designed in Glade for the subject "Introduction to programming in C" 
at the University of Wrocław. Project contains C app and a makefile script.

[![GTK Version](https://img.shields.io/badge/gtk-Gtk%E2%80%933.0-orange)](https://docs.gtk.org/gtk3/)
![GitHub top language](https://img.shields.io/github/languages/top/jakubcieslik99/fiszki)
![GitHub repo size](https://img.shields.io/github/repo-size/jakubcieslik99/fiszki)
[![Beerware License](https://img.shields.io/badge/license-Beerware-green)](https://en.wikipedia.org/wiki/Beerware)


## Features

- Running flashcards test with uploaded .csv list
- Counting points and showing progress bar
- Showing list of mistakes at the end


## Screenshots

<img src="https://i.ibb.co/Np6G8Pj/fiszki-1.png" width="800">


## Run Locally

ℹ️ Instructions for running on Linux based system:

- Install all needed packages

```bash
  apt get update
  apt install make gcc libgtk-3-dev
```

- Clone the project and navigate to the project directory

```bash
  git clone https://github.com/jakubcieslik99/fiszki.git
  cd fiszki/app
```

- Run makefile script and start the app

```bash
  make
  ./fiszki
```

ℹ️ The word list must be in "**.csv**" format, where words and their translations are separated by "**;**"
## Languages

🔤 Available languages: **PL**


## Feedback

If you have any feedback, please reach out to me at ✉️ contact@jakubcieslik.com


## Authors

- [@jakubcieslik99](https://www.github.com/jakubcieslik99)


## License

⚖️ [Beerware](https://en.wikipedia.org/wiki/Beerware)

