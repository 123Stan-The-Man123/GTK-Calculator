# GTK-Calculator

https://github.com/user-attachments/assets/d758ffde-cc08-4ed0-8b05-732f21c84920

This is a calculator I've written using C and the GTK-4 library. It takes an infix expression, parses it into an RPN/postfix expression and evaluates it. It has the basic functions, and more can be added if needed.

# Dependencies
* Linux
* libgtk-4-dev
* Make
* gcc

# Installation
In the cloned directory, run ./install-script.sh and follow the instructions.

# Desktop File
I have also added a desktop file in src. The install script will automatically place it in ~/.local/share/applications/ so that your application selector can see it. 
