![cover](https://user-images.githubusercontent.com/75450615/174878256-fcf0ccbb-9a8e-4c0b-933a-0e5e6637724e.jpeg)

## ¿Qué es?

Dune 2022 es un remake del clásico videojuego de estrategia en tiempo real (RTS) Dune 2000, lanzado originalmente para Windows en 1998 y luego portado a la consola PlayStation de Sony. 

## ¿Cómo puedo saber si puedo correr el juego?

Por el momento el juego se encuentra disponible para computadoras personales con sistemas operativos basados en Debian, así que no deberías tener problemas si contás con una distribución de Linux como Ubuntu, Mint o Xubuntu. Se planea portar el juego a otras plataformas como Windows 10, MacOSX y Virtual Boy.

![supported_distros](https://user-images.githubusercontent.com/75450615/174876428-a6147531-4bdd-4770-ac3c-48c33e044653.png)

Es necesario también instalar las siguientes dependencias:

- cmake
- libsdl2-dev
- libsdl2-ttf-dev
- libsdl2-image-dev
- qt5-default
- qtbase5-dev
- qtmultimedia5-dev
- gstreamer1.0-plugins-good
- libqt5multimedia5-plugins

Para instalarlas es posible utilizar el comando de Linux "apt install" seguido del nombre de la dependencia, y hacer esto con cada una. Puede ser necesario ejecutar estos comandos con permisos de superusuario, en ese caso el comando anterior debe estar precedido por "sudo" (por ejemplo, sudo apt install libsdl12-dev).

#### ¿Cómo descargo el juego?

Por el momento es posible descargar el juego a través de su repositorio de GitHub (https://github.com/Lautaro-De-Lucia/TDP-Dune2022). Para esto debemos clonar el repositorio mediante el siguiente comando de consola:

`$ git clone https://github.com/Lautaro-De-Lucia/TDP-Dune2022` <br>

#### ¿Cómo instalo el juego?

Una vez descargados los archivos e instaladas las dependencias se procede a la etapa de "buildeo", para la cual se utilizará CMake (herramienta multiplataforma de generación de código) y Make (aplicación nativa de entornos Linux). Primero, creamos un directorio dentro del directorio "root" de los archivos descargados. Luego, desde ese directorio, vamos a construir con CMake y luego obtener los ejecutables mediante el comando "make all". Todos los pasos se encuentran detallados en orden a continuación, asumiendo que nos encontramos en el root del proyecto descargado:

`$ mkdir build` <br>
`$ cd build` <br>
`$ cmake -S ../ -B .` <br>
`$ make all` <br>

Ahora ya resulta posible correr el juego. El directorio donde se ejecutó ese último comando "make all" ahora contiene dos ejecutables, `server` y `client`. Para correr el `server` debemos indicarle por línea de comando el parámetro del "service name" (por ejemplo, http-alt), y una vez ejecutado se quedará corriendo esperando jugadores que quieran jugar. Por otro lado, el `cliente` también se ejecuta el
