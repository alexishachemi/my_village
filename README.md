

# Technologies

Project in C, Raylib

# Goals

- Generate mini map of a small world + village

# Inputs

- configurated tilemap
- parameters
- Pre-generated structures/sections ? (opt)

## Outputs

- Image?
- File '.mvm' / '.csv' or other related to the tileset creator (mvm = myvillagemap)

# Usage

via the use of a lib in a bin, or the included generator bin for output files

program help :
```
usage ./myVillage -c CONFIG -t TILEMAP -o OUTPUT_TYPE -f OUTPUT_FILE
    -c CONFIG : path to the configuration file
    -t TILEMAP : path to the tilemap file
    -o OUTPUT_TYPE : type of output file (image, mvm, csv)
    -f OUTPUT_FILE : path to the output file
```

An amazing loading bar will show you the progress of the generation

# Extras

## Tileset Creator

Allows to create and configure a tilemap to generate with it
- Relationships wfc + auto-tiling
- 'Types' of tiles :
    - Terrain params (i.e. grass + tree)
    - Props params (i.e. table + lamp)
- Collections for thematic based buildings (i.e. farmers, blacksmiths, etc)
...

# Technical

WFC + BSP + CSP (terrain, buildings, interior)

```c
typedef struct {



} tile_t;
```

```c
typedef struct {

    

} prop_t;
```