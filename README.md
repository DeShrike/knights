# Knights

See Numberphile videos:

https://www.youtube.com/watch?v=UiX4CFIiegM

https://www.youtube.com/watch?v=VgmDuBCayPw

All code was written by me, without the use of AI/LLM.

## Dependencies

This program uses libpng.

To install on Debian based systems:

```console
sudo apt-get install libpng-dev
```

## Quick start

Clone this repo and then do:

```console
cd knights
make
./knights Knight-Knight 256 output.png
```

The first parameter defines the opponents.

Possible opponents are:

Knight, Dromedary, Alfil, Antilope, Dabbaba, Wazir, Zebra, Ferz, Manis, Sipius, Xoch.

The maximum opponents is currently 20.

The second parameter is the size of the output image.

256 = 256 x 256 pixels.

Should be an even number not greater than 2048.

## Pieces

### Knight

```
.......
..X.X..
.X...X.
...#...
.X...X.
..X.X..
.......
```

### Alfil

```
.......
.X...X.
.......
...#...
.......
.X...X.
.......
```

### Dromedary

```
.........
....X....
.........
.........
.X..#..X.
.........
.........
....X....
.........
```

### Antilope

```
...........
..X.....X..
.X.......X.
...........
...........
.....#.....
...........
...........
.X.......X.
..X.....X..
...........
```

### Wazir

```
.....
..X..
.X#X.
..X..
.....
```

### Zebra

```
.........
..X...X..
.X.....X.
.........
....#....
.........
.X.....X.
..X...X..
.........
```

### Ferz

```
.....
.X.X.
..#..
.X.X.
.....
```

### Mantis

```
.........
...X.X...
.........
.X.....X.
....#....
.X.....X.
.........
...X.X...
.........
```

### Sipius

```
...........
...X...X...
...........
.X.......X.
...........
.....#.....
...........
.X.......X.
...........
...X...X...
...........
```

### Xoch

```
...........
...X...X...
...........
.X.X...X.X.
...........
.....#.....
...........
.X.X...X.X.
...........
...X...X...
...........
```

## To try

Examples from the Numberphile video:

```console
./knights Knight-Knight 1024 out.png
```

![Knight-Knight](examples/Knight-Knight.png)

```console
./knights Knight-Knight-Knight 1024 out.png
```

![Knight-Knight-Knight](examples/Knight-Knight-Knight.png)

```console
./knights Knight-Knight-Knight-Knight 1024 out.png
```
![Knight-Knight-Knight-Knight](examples/Knight-Knight-Knight-Knight.png)

```console
./knights Knight-Knight-Knight-Knight-Knight 1024 out.png
```

![Knight-Knight-Knight-Knight-Knight](examples/Knight-Knight-Knight-Knight-Knight.png)

```console
./knights Alfil-Dromedary 1024 out.png
```

![Alfil-Dromedary](examples/Alfil-Dromedary.png)

```console
./knights Knight-Antilope 1024 out.png
```

![Knight-Antilope](examples/Knight-Antilope.png)

```console
./knights Knight-Dabbaba-Wazir-Wazir 1024 out.png
```
This one is different from the image shown in the video. I think they did something wrong.

![Knight-Dabbaba-Wazir-Wazir](examples/Knight-Dabbaba-Wazir-Wazir.png)

```console
./knights Knight-Zebra 1024 out.png
```

![Knight-Zebra](examples/Knight-Zebra.png)

```console
./knights Wazir-Ferz-Wazir-Ferz 1024 out.png
```

![Wazir-Ferz-Wazir-Ferz](examples/Wazir-Ferz-Wazir-Ferz.png)

A few new ones:

```console
./knights Knight-Xoch-Xoch 1024 out.png
```

![Knight-Xoch-Xoch](examples/Knight-Xoch-Xoch.png)

```console
./knights Mantis-Sipius-Xoch 1024 out.png
```

![Mantis-Sipius-Xoch](examples/Mantis-Sipius-Xoch.png)

```console
./knights Xoch-Ferz-Wazir 1024 out.png
```

![Xoch-Ferz-Wazir](examples/Xoch-Ferz-Wazir.png)

```console
./knights Alfil-Mantis-Wazir 1024 out.png
```

![Alfil-Mantis-Wazir](examples/Alfil-Mantis-Wazir.png)

```console
./knights Dabbaba-Knight-Zebra 1024 out.png
```

![Dabbaba-Knight-Zebra](examples/Dabbaba-Knight-Zebra.png)

```console
./knights Zebra-Ferz-Wazir 1024 out.png
```

![Zebra-Ferz-Wazir](examples/Zebra-Ferz-Wazir.png)

```console
./knights Antilope-Knight-Sipius 1024 out.png
```

![Antilope-Knight-Sipius](examples/Antilope-Knight-Sipius.png)

```console
./knights Dromedary-Mantis-Knight 1024 out.png
```

![Dromedary-Mantis-Knight](examples/Dromedary-Mantis-Knight.png)

```console
./knights Knight-Dabbaba-Antilope-Wazir 1024 out.png
```

![Knight-Dabbaba-Antilope-Wazir](examples/Knight-Dabbaba-Antilope-Wazir.png)

```console
./knights Knight-Dromedary-Alfil-Wazir 1024 out.png
```

![Knight-Dromedary-Alfil-Wazir](examples/Knight-Dromedary-Alfil-Wazir.png)

```console
./knights Xoch-Xoch-Mantis-Xoch-Xoch 1024 out.png
```

![Xoch-Xoch-Mantis-Xoch-Xoch](examples/Xoch-Xoch-Mantis-Xoch-Xoch.png)

