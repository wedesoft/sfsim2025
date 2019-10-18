#include "plane.h"


// Project 3D points into a 2D coordinate system defined by the plane.
list_t plane_coordinates(plane_t plane, list_t vectors) {
  list_t result = make_list();
  vector_t u = orthogonal1(plane.normal);
  vector_t v = orthogonal2(plane.normal);
  for (int i=0; i<vectors.size; i++) {
    vector_t d = vector_subtract(get_vector(vectors)[i], plane.point);
    append_coordinate(&result, coordinate(inner_product(d, u), inner_product(d, v)));
  };
  return result;
}

// Backproject 2D coordinates on a plane to 3D points.
list_t plane_points(plane_t plane, list_t coordinates) {
  list_t result = make_list();
  vector_t u = orthogonal1(plane.normal);
  vector_t v = orthogonal2(plane.normal);
  for (int i=0; i<coordinates.size; i++) {
    coordinate_t coordinate = get_coordinate(coordinates)[i];
    append_vector(&result, vector(plane.point.x + coordinate.u * u.x + coordinate.v * v.x,
                                  plane.point.y + coordinate.u * u.y + coordinate.v * v.y,
                                  plane.point.z + coordinate.u * u.z + coordinate.v * v.z));
  };
  return result;
}
