# My Village

**Note**: This readme may not reflect the current state of the project and might be subject to changes as it is still in development. Please see [Milestones](https://github.com/alexishachemi/my_village/milestones?direction=asc&sort=due_date&state=open) for more information about the current and projected state of MyVillage.

---
<br>
MyVillage is a procedural generator that creates a random, natural-looking world containing a village that is also randomly generated.

The world itself is set in a classic fantasy setting with villages composed of buildings such as smithies, inns, shops and more on top of standard villager houses.

The goal of the project is to experiment with multiple procedural generation algorithm and combine them to create a nice looking world.

Some of the algorithms are:

- [Wave Function Collapse](https://en.wikipedia.org/wiki/Wave_function_collapse)
- [Binary Space Partitioning](https://en.wikipedia.org/wiki/Binary_space_partitioning)
- [Constraint Satisfaction Problem](https://en.wikipedia.org/wiki/Constraint_satisfaction_problem)

## Dependencies

- [Raylib](https://www.raylib.com/) (v5.5, fetched automatically)
- [Raygui](https://github.com/raysan5/raygui) (v4.5, fetched automatically)
- [CVEC](https://github.com/alexishachemi/cvec) (any, fetched automatically)
- [Linked](https://github.com/alexishachemi/linked) (any, fetched automatically)

## Inputs

The program needs 2 files provided by the user to run a generation:

- **Config File**: Dictates the settings of the generation such as the size of the map and how many structures to generate.
- **Tileset File**: Dictates which assets to use like terrain, props etc ... And how they interact with each other.

## Outputs

The program can output the generated map in several formats:

- **Rendered image**
- **CSV file**
- **MVM file** (See *Extras*)

## Usage

```
./my_village -t TILESET -o OUTPUT_TYPE -f OUTPUT_FILE
    -t TILESET : path to the tileset file
    -o OUTPUT_TYPE : type of output file (image, mvm, csv)
    -f OUTPUT_FILE : path to the output file
```

## Extras

#### Tileset Creator

To facilitate the creation of tileset files, a GUI application allows you to create and edit tilesets.

#### MVM File

The MVM file is a custom file format that stores the generated map. It can be used to load the map in the future without having to regenerate it.