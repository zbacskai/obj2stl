# obj2stl

3d mesh file converter. Currently supported formats/features:

- obj to stl-ascii conversion
- obj to stl-binary conversion
- translation
- rotation
- scale
- point inside modele feature (tests if point is inside any of the objects in the model)

## Latest version

This is a work in progress project, for the latest version:

https://github.com/zbacskai/obj2stl.git

## Usage

./obj-convert --help

usage: ./obj-convert <input file format> <input file> <output file format> <output file>

Allowed options:

  --help                help using the sf
  
  --iff                 only obj supported at the moment
  
  --if                  path to the input file
  
  --off                 stl-ascii or stl-bin supported
  
  --of                  output-file name
  
  --transformations arg transformations in given order separated by /
                        possible options scale=2,1,0/rotate=-23.1,2.1,0.0/
                        translation=1.2,3.1,0.0 
                        where the transformtion order can be pased in any order
                        and the numbers represent x,y,z rotations and x,y,z translations
                        
   --point_in_model arg  Check if given point is inside of object
  
  NOTE: if point_in_model arg is provided the output-file name is still mandatory <will change it in the future>, but the file will not be written.
  
  ## Prerequisites
  
  - Eigen3 library: http://eigen.tuxfamily.org/index.php?title=Main_Page (for linear Algebra)
  - Boost: https://www.boost.org/ (Only for command line parsing and may be removed in future)
  
## Known issues
  
At this stage of the project only simple regression test has been implementes. Go to test directory and run the shell script after the code has compiled. 
A Google Test based solution will be introduced in the future. For further info see: https://github.com/zbacskai/obj2stl/issues

## Build Environment

Code has been compiled on (lsb_release -a & uname -a):

Distributor ID:	Ubuntu
Description:	Ubuntu 20.04.1 LTS
Release:	20.04
Codename:	focal

#52-Ubuntu SMP Thu Sep 10 10:58:49 UTC 2020 x86_64 x86_64 x86_64 GNU/Linux
