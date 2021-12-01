The ray scene is in charge of generating the camera to world transformation matrices, the object space to world space matrices (applied to the normals of shapes), as well as tracing the rays and computing the lighting equation.
I have other classes that manage the equations for intersections and normals for cones, cubes, spheres and cylinders.

My intersect( glm::vec4 &P, glm::vec4 &d) functions and return a vec2 of floats, where each float represents a value for t. T = 0 if the ray (described as a position P and a direction d) does not intersect with the shape. I then use these functions to find the closest positive t point between the ray and the object. This is done in the coordinate space of the shape (where the origin is at the center of the shape).

Also note that these intersect functions take in vec4s for P and d. However, if the implicit shape is on a 2d plane (like the RayDisc class), it works with just two of the values. For RayDisc, this would be the x and z values of P and d.

I decided not to interleave the ray intersection calculations into my shape classes, because I found that I needed to have logic between these classes that would require other classes (e.g. the cone and cylinder both need disk intersection and normal calculation). Additionally, given the way the Scene and SceneParser work, we never actually use the subclasses of OpenGLShape that describe the primitives, but are rather given a Primitive Enum and a cumulative translation matrix.

Understanding how to compute theta_w from the aspect ratio, which cancels out the need for k, was particularly helpful.

There are some bugs in my code. In ctc_insect the cones on the center left of the screen does not show up. It has been rotated 90 degrees, and the bug seems to affect cones that are rotated from around 68 to 110 degrees. I think this has to do with the way I compute intersections in my cone.

I also have an issue computing normals.

I plan to fix these issues and submit again.