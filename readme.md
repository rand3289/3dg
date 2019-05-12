# 3dg
3dg is a tiny 3D graph visualization program to be used with anaglyph (red/blue) glasses.  It is written in C++ using Simple Directmedia Layer library.  Current version uses 285 lines of code.  Runs on linux and most likely on anything else that supports SDL.  Current version displays a random graph only.  You can rotate the graph around 2 axis by pressing arrow keys.  Escape or q-quits.

Future versions will display graphs stored in a dot file format if you pass one on the command line. 
https://en.wikipedia.org/wiki/DOT_(graph_description_language) 
It supports graph node labels.

###TODO:
Current projection is a hack
Zoom in/out does not work
