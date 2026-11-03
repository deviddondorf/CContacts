# CContacts
A lightweight, CLI-based phone-/contactbook written in C.

## Features
Very basic handling like:
- Viewing the contacts in a list
- Adding new contacts
- Deleting a contact
- And a Quit option easier than vim

## Requirements (Dependencies)
It uses SQLite3-Dev

```bash
sudo apt update
sudo apt install libsqlite3-dev build-essential
```

## Install
If you use the code and what to make it run:
```bash
gcc main.c -o ccontacts -lsqlite3
./ccontacts
```

If you use the .deb Package from the Release (Recommended: Requires Debian)
```bash
sudo apt install ./ccontacts.deb
```
you can then run it with the command:
```bash
ccontacts 
```
