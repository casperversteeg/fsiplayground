#!/bin/bash

rm *.msh

gmsh geo/fluid.geo -2 -o fluid.msh
gmsh geo/solid.geo -2 -o solid.msh
gmsh geo/domain.geo -2 -o domain.msh
