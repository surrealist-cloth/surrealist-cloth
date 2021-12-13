#include "EmptyShape.h"
#include <vector>

EmptyShape::EmptyShape()
{

}

std::vector<Shape *> EmptyShape::getChildren()
{
    return std::vector<Shape*>();
}

void EmptyShape::generateShape()
{

}
