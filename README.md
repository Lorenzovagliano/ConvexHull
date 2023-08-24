## **ConvexHull**
---
This program takes 2D coordinates inputted through .txt files and returns the convex hull of these coordinates and the performance of different convex hull algorithms.
---
### A convex hull is a fundamental concept in geometry and computational geometry. It refers to the smallest convex polygon or polyhedron that encompasses a set of points in a Euclidean space (such as two-dimensional or three-dimensional space). In simpler terms, the convex hull of a set of points is the shape formed by "wrapping" a rubber band around those points in such a way that the band remains stretched and convex.
---
![2023-08-24 10-29-24](https://github.com/Lorenzovagliano/ConvexHull/assets/111889654/3cb7bc67-16c1-4fb6-89fd-d81116120da4)
![2023-08-24 10-32-45](https://github.com/Lorenzovagliano/ConvexHull/assets/111889654/548bcf3f-271b-46f3-bb69-ff7d491da330)
![Captura de tela 2023-08-24 103538](https://github.com/Lorenzovagliano/ConvexHull/assets/111889654/78be8dff-7f0a-4e1a-98ac-2a8f2123dea2)
---
### Warning: This program has not yet been completely translated from portuguese to english.
---
### **Tools** 
---
- Language: [C++ 11](https://cplusplus.com/doc/)
- Library: [SFML](https://www.sfml-dev.org/)
- Compiler: [GCC](https://gcc.gnu.org/)
- Build: [Make](https://www.gnu.org/software/make/)
---
### How to use it
Before compiling the program, you need to have GCC, GNU Make, and the SFML graphics library installed, which is mandatory for the program to work. Before installing anything, to update the package information, use the command:

    sudo apt-get update
To install SFML, after opening a terminal in the project directory, use the command:

    make install
If preferred over `make install`, you can use the command:

    sudo apt-get install libsfml-dev
To compile and run the program, use the following command:

    make run INPUT=<input file> VIDEO=<sim or nao>
- Replace `<input file>` with the full name of the text file containing the input Points, This file should be located in the root of the project. Its format should be like this:
  
  PointX PointY
  <p>
  PointX PointY
  <p>
  PointX PointY
  <p>
  PointX PointY
  <p>
  <p>
- Example:
  <p>
  4 9
  <p>
  30 34
  <p>
  21 5
  <p>
  11 1
  <p>
  <p>
  You may add as many lines as you want.
- Replace `<sim or nao>` with either "sim" or "nao", indicating whether you want to generate the algorithm's visualization or not.

### **Removing object files and executable**
---
    make clean
