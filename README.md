# tp-2020-1c-SOVID

## Enunciado y pruebas

* [enunciado](/doc/delibird-v1.4.pdf)
* [pruebas](/doc/delibird-pruebas.pdf)

## Develop

Para importar el proyecto (en Eclipse), seguir los [siguientes pasos](https://www.youtube.com/watch?v=PgrORWmUxkI) para cada uno de los módulos.

## Build

```bash
$ git clone https://github.com/ggallici/TP-SO-2020-c1
$ cd TP-SO-2020-c1
$ chmod u+x build.sh
$ ./build.sh
$ source ~/.bashrc
```

## Run

Cada módulo debe iniciarse desde su raiz ya que sus respectivas configuraciones se acceden mediante rutas relativas.

```bash
#broker
$ cd broker
$ ./build/broker

#game-card
$ cd game-card
$ ./build/game-card

#team
$ cd team
$ ./build/team

#game-boy
$ cd game-boy
#envío de un mensaje
$ ./build/game-boy GAMECARD NEW_POKEMON Pikachu 1 9 3 1
#envío de varios
$ ./new_pokemons_varios.sh
```
