# Persistent Reverse Shell

## About

This is a malware prototype written in C with a version for Linux and Windows. It tries to open a reverse shell in multiple ways according to system utils.

- Linux and Windows version.
- Runs in the background.
- Has a basic level of persistency.

### Linux

It gets somewhat persitent throw cronjobs, systemctl services and by backdooring ~/.bashrc file

Compile with:
```
gcc main.c -o rev_shell
```

### Windows
Coming soon...


#### Enjoy the code and give suggestions.