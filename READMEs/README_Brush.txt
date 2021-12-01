NOTE: This is a first submission, and I am intending to submit another version slightly
later today.


My Brush base class does implement the makeMask method.
All the base class method does is make an array of the right size for the mask.

Understanding the radius of the mask was somewhat challenging,
since the mask origin is at the top right of the "circle" where the paint should be.

I decided that the canvas2D should just have one brush member variable,
and did not want to initialize it in the initializer list since I needed to have
a switch statement to figure out which type of brush the canvas2D starts out with.

My switchBrush() helper method means I don't really use the m_brushType variable.
I still set the m_brushType variable to the correct brush type in case I might use
this variable in future projects, etc, though I could have also decided to delete it.
