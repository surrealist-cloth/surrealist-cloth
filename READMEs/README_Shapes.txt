My project structure uses some of the ideas in the Terrain lab, 
in particular the notion that there is a function that creates a 
tesselated triangleStrip of size num_rows and num_cols by 
querying functions which return the position and normal 
in cartesian coordinates of each given a row and column.

In this case, subclasses of OpenGLShape
Cube, Cone, Cylinder and Sphere each manage a TriangleStrip 
class which builds the std::vector<GLfloat> of position and 
normal data to be displayed. 
As needed, each of Cube, Cone, Cylinder and Sphere also create 
classes which represent planes of different types. These include
Rectangle, Disk, Tube, ConeShell, and SphereShell.

These classes are all subclasses of Mesh, a class that
has a getPointData(row, col) method, which, given a row and column
index on the plane the class represents (e.g. spherical coordinates if a 
Sphere class), will return the position and normal data of that point
in 3d cartesian coordinates. This method is called when Mesh classes
are passed into TriangleStrip.
Subclasses of Mesh can also take two matrixes, in order to transform their
position coordinates and their normal coordinates, to allow for translation,
rotation and scaling of these meshes as needed, before data is
to an std::vector<GLfloat> representing a shape in 3d cartesian space.