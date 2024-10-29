# Terminologies

| Term    | Meaning                                                                                                                                |
| ------- | -------------------------------------------------------------------------------------------------------------------------------------- |
| Terrain | Collection of assets data used to represent the surface of a tile. Also contains data related to its relation with other terrains.     |
| Prop    | Physical object that serves as an occupant of a tile. Only one prop can populate a tile.                                               |
| Tile    | Single world cell. Is composed of a terrain and a prop (optional).                                                                     |
| Chunk   | Collection of tiles constituting a part of the world.                                                                                  |
| Map     | Collection of chunks constituting the entire generated world.                                                                          |
| Biome   | Collection of props and terrain indexes regrouping them around a specific theme. They can contain other biomes to inherit their props. |
