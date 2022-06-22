
<img src="https://user-images.githubusercontent.com/75450615/174878256-fcf0ccbb-9a8e-4c0b-933a-0e5e6637724e.jpeg" width="1400">

## ¿Qué es?


Dune 2022 es un remake del clásico videojuego de estrategia en tiempo real (RTS) Dune 2000, lanzado originalmente para Windows en 1998 y luego portado a la consola PlayStation de Sony en el año 2000. Mediante esta adaptación es posible disputar partidas en línea, ya que el producto incluye un programa *servidor* cuya finalidad es la de mantener el juego corriendo y aceptar a los clientes que deseen jugar.

### ¿Cómo puedo saber si puedo correr el juego?

Por el momento el juego se encuentra disponible para computadoras personales con sistemas operativos basados en Debian, así que no deberías tener problemas si contás con una distribución de Linux como Ubuntu, Xubuntu o Linux Mint. Se planea portar el juego a otras plataformas como Windows 10, MacOSX y Virtual Boy en el futuro.


<p align="center">
  <img width="400" src="https://user-images.githubusercontent.com/75450615/174876428-a6147531-4bdd-4770-ac3c-48c33e044653.png">
</p>

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

Para instalarlas es posible utilizar el comando de Linux `apt install` seguido del nombre de la dependencia, y hacer esto con cada una. Puede ser necesario ejecutar estos comandos con permisos de superusuario, en ese caso el comando anterior debe estar precedido por `sudo` (por ejemplo, `sudo apt install libsdl12-dev`).

### ¿Cómo descargo el juego?

Por el momento es posible descargar el juego a través de su repositorio de GitHub (https://github.com/Lautaro-De-Lucia/TDP-Dune2022). Para esto debemos clonar el repositorio mediante el siguiente comando de consola:

`$ git clone https://github.com/Lautaro-De-Lucia/TDP-Dune2022` <br>

### ¿Cómo instalo el juego?

Una vez descargados los archivos e instaladas las dependencias se procede a la etapa de "buildeo", para la cual se utilizará CMake (herramienta multiplataforma de generación de código) y Make (aplicación nativa de entornos Linux). Primero, creamos un directorio dentro del directorio "root" de los archivos descargados. Luego, desde ese directorio, vamos a construir con CMake y luego obtener los ejecutables mediante el comando "make all". Todos los pasos se encuentran detallados en orden a continuación, asumiendo que nos encontramos en el root del proyecto descargado:

`$ mkdir build` <br>
`$ cd build` <br>
`$ cmake -S ../ -B .` <br>
`$ make all` <br>

Ahora ya resulta posible correr el juego. El directorio donde se ejecutó ese último comando "make all" ahora contiene dos ejecutables, `server` y `client`. Para correr el `server` debemos indicarle por línea de comando el parámetro del *service name* (por ejemplo, http-alt), y una vez ejecutado se quedará corriendo esperando jugadores que quieran jugar. Por otro lado, si lo que queremos es jugar, debemos utilizar el archivo ejecutable del `client`. Para correr el juego, es necesario primero tener el juego corriendo en un servidor en alguna computadora. Conociendo la dirección ip de este servidor, nos será posible unirnos a una partida hosteada por el servidor ejecutando el programa `client` con dos argumentos por línea de comando: primero se le indica el *host name* (la dirección de ip del servidor), y luego se le especifica el *service name* (http-alt). Para correr el servidor y el cliente en una misma computadora, es posible utilizar `localhost` como *host name*. A continuación se ilustra con un ejemplo de ejecución del programa en una misma máquina:

- Dejamos corriendo el servidor
`$ ./server http-alt` <br>
- Abrimos un cliente (en una terminal aparte)
`$ ./client localhost http-alt` <br>

### ¿Cómo se juega?

Se inicia el juego uniendose a la partida del servidor mediante las intrucciones detalladas más arriba. Se selecciona "Jugar" en la pantalla de inicio, y luego nos encontramos con la siguiente pantalla:

<p align="center">
  <img width="600" src="https://user-images.githubusercontent.com/75450615/174887546-3f78a0d5-6877-4388-887c-964ab49daa9e.png">
</p>


#### Pantalla de selección de casa

Nos será posible elegir entre tres casas, o tres facciones distintas, toda basadas en las casas ficticias creadas por Frank Herbert en sus novelas de Dune. Es posible jugar sólo contra enemigos, o con aliados. Para esto último, los dos jugadores deben elegir la misma facción. Distintas facciones poseen distintos edificios y distintas unidades.

### Inicio del juego

<p align="center">
  <img width="400" src="https://user-images.githubusercontent.com/75450615/174889424-eba884dc-2f52-4482-8392-091c9c620986.png">
</p>

Una vez escogida una casa y comenzado el juego, nos encontramos con una pantalla como la de arriba. Primero se mencioan los distintos tipos de terreno:

- arena
- roca
- duna
- colina

En la **arena** podemos mover nuestras unidades, pero no podemos construir edificios. Las **dunas** tiene un efecto similar sólo que las unidades se mueven más lento a través de ella. Para construir edificios y cuarteles, debemos encontrar una buena ubicación con terreno rocoso, ya que la **roca** es el único terreno lo suficientemente sólido para construir sobre él. Las **colinas** funcionan como barreras naturales, ya que ninguna unidad puede atravesarla.

#### Objetivo del juego

Cada facción comienza la partida con un centro de construcción, cuya destrucción significa el fin del juego para esa facción. Nuestro *objetivo* es entonces *defender el centro de construcción*, ayudandonos de distintos edificios que nos permitan crear distintas unidades, a la vez que intentamos *destruir el centro de construcción de cada facción opuesta*.

#### Construcción de los edificios

Para construir edificios es necesario contar con *energía* y *especia*, la moneda corriente en el mundo de Dune. Si bien se cuenta con cierta cantidad de especia inicial, si la agotamos construyendo edificios y unidades no podremos generarla nuevamente. Para esto, se deben crear unidades **cosechadoras**, que se encargan de transportar la especia de lugares en el mapa con arena especiada hacía la refinería. Un buen consejo es empezar construyendo una **fábrica pesada**, donde pueden entrenarse estas unidades **cosechadoras** que nos conseguirán más especia para poder seguir construyendo edificios y entrenando unidades de combate. Para construir, se hace *click izquierdo* en el ícono con el edificio que queremos construir, y luego click izquierdo en el lugar del mapa que nos resulte conveniente, siempre y cuando haya una formación rocoso bajo suyo. Ahora que tenemos edificios, podemos emezar a crear unidades.

<p align="center">
  <img width="600" src="https://user-images.githubusercontent.com/75450615/175114388-1fd3d5fa-5ec2-4fd6-8fe1-182b3a7490e6.jpg">
</p>

#### Creacion de Unidades

Cada unidad tiene un costo en especia y un edificio creador. Las unidades de **infantería** se crean desde el **cuartel**. Los **vehículos** se crean en la **fábrica ligera** y la **fábrica pesada**. Cuanto más edificios creadores para una unidad, menos tiempo esta tarda en crearse. Sin embargo, hay solo una unidad creadora de la que emergen las unidades. Para designar el creador seleccionamos el edificio y damos *click derecho* (la selección del edificio puede ser con un *click izquierdo* sobre el edificio o mediante una *selección* de mouse, manteniendo *click izquierdo* desde un punto del mapa hasta otro, definiendo de esta manera un rectángulo donde se seleccionan las unidades y/o edificios en nuestro poder). Existen unidades en los que el creador no será suficiente, y necesitamos tener una estructura **palacio** en pie para poder crearlas, como por ejemplo la **infantería sardaukar**.

<p align="center">
  <img width="400" src="https://user-images.githubusercontent.com/75450615/175114794-d48a411c-3efd-4115-bb52-3600e057a8d6.jpg">
</p>

#### Movimiento de unidades 

Una vez creadas las unidades, podemos desplazarlas de un punto del mapa a otro. Para hacer esto, debemos primero seleccionarlas y luego hacemos *click derecho* en algún punto del mapa, y nuestras unidades inciaran un trayecto hasta ese punto. Si en este punto se encuentra una **colina**, o hacemos click sobre una unidad o edificio aliado, nuestras unidades seleccionadas se moverán a la primera *posición libre del mapa más cercana* a ese punto.

<p align="center">
  <img width="400" src="https://user-images.githubusercontent.com/75450615/175114144-f3d67eb3-08d8-4318-ad0c-5baf73eafcbd.jpg">
</p>

#### Cosechadoras

Ya sabemos como crear unidades *cosechadoras* luego de construir una *fábrica pesada*. Para ponerlas a trabajar una vez que aparezcan junto a la fábrica, necesitamos primero una **refinería**, que es el edificio donde las cosechadoras depositan la especia colectada. Luego de construir la **refinería**, seleccionamos una o mas *cosechadoras* y luego hacemos click derecho en algún lugar del mapa con arena con especia, y automáticamente estas unidades iniciaran un ciclo de colectar especia y depositarla en la refinería, hasta agotar los recursos naturales de esa región del mapa.

<p align="center">
  <img width="400" src="https://user-images.githubusercontent.com/75450615/175112064-b6afb0f8-6863-46cb-81ff-50bf652c397f.jpg">
</p>

#### Combate

Dentro de las unidades tenemos aquellas que se pueden utilizar para dañar otras unidades y edificios enemigos, como los rápidos **trikes** y los pesados **devastadores**. Para atacar al enemigo, debemos seleccionar nuestras unidades de ataque y luego hacemos *click derecho* sobre alguna unidad o edificio enemigo. Nuestras unidades ahora iniciaran un trayecto hasta estar a una distancia adecuada y luego comienzan a efectuar daño a los edificios o unidades enemigas que se encuentren en las proximidades del punto seleccionado. Cabe destacar que nuestras unidades no sólo atacan a la unidad enemiga seleccionada, sino también a cualquier unidad (o edificio) cercano a esa unidad.

#### Fin del juego

El juego termina cuando sólo queda un centro de construcción en pie, y la facción a la que pertenezca es declarada **ganadora** de la partida.

