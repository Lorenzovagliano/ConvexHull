## **ConvexHull**
---
This program takes 2D coordinates inputted through .txt files and returns the convex hull of these coordinates.
A convex hull is a fundamental concept in geometry and computational geometry. It refers to the smallest convex polygon or polyhedron that encompasses a set of points in a Euclidean space (such as two-dimensional or three-dimensional space). In simpler terms, the convex hull of a set of points is the shape formed by "wrapping" a rubber band around those points in such a way that the band remains stretched and convex.
---
---
### How to use it
Before compiling the program, you need to have GCC, GNU Make, and the SFML graphics library installed, which is mandatory for the program to work. Before installing anything, to update the package information, use the command:

    $ sudo apt-get update
To install SFML, after opening a terminal in the project directory, use the command:

    $ make install
If preferred over `make install`, you can use the command:

    $ sudo apt-get install libsfml-dev
To compile and run the program, use the following command:

    $ make run INPUT=<input file> VIDEO=<yes or no>
- Replace `<input file>` with the full name of the text file containing the input Points, This file should be located in the root of the project. Its format should be like this:
  PointX PointY
  PointX PointY
  PointX PointY
  PointX PointY

  Example:
  4 9
  30 34
  21 5
  11 1

  You may add as many lines as you want.
- Replace `<yes or no>` with either "yes" or "no", indicating whether you want to generate the algorithm's visualization or not.
