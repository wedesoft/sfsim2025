#include "plane.h"


// Project 3D points into a 2D coordinate system defined by the plane.
list_t *plane_coordinates(plane_t plane, list_t *vectors)
{
  list_t *result = make_list();
  vector_t u = orthogonal1(plane.normal);
  vector_t v = orthogonal2(plane.normal);
  for (int i=0; i<vectors->size; i++) {
    vector_t d = vector_difference(get_vector(vectors)[i], plane.point);
    append_coordinate(result, coordinate(inner_product(d, u), inner_product(d, v)));
  };
  return result;
}
