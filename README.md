# Knight

See Numberphile videos:

https://www.youtube.com/watch?v=UiX4CFIiegM

https://www.youtube.com/watch?v=VgmDuBCayPw

## Dependancies

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

The first parameter defines the players.

Possible players are:

Knight, Leaper, Alfil, Antilope, Dabbaba, Wazir, Zebra and Ferz.

Examples:

- Knight-Knight
- Knight-Alvil-Wazir-Knight
- Wazir-Ferz-Alfil-Alfil
- Dabbaba

The maximum players is currently 20.

The second parameter is the size of the output image.

256 = 256 x 256 pixels.

Should be an even number not greater than 2048.
