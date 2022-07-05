
<img src="https://user-images.githubusercontent.com/75450615/175116440-0fb9283c-a1e5-4299-8b14-4e5891e6c16e.jpeg" width="1400">

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

Nos será posible elegir entre tres casas, o tres facciones distintas, todas basadas en las casas ficticias creadas por Frank Herbert en sus novelas de Dune. Distintas facciones poseen edificios y unidades de distintos colores (rojo para Harkonnen, azul para Atreides y verde para Ordos).

### Inicio del juego

<p align="center">
  <img width="1200" src="https://user-images.githubusercontent.com/75450615/174889424-eba884dc-2f52-4482-8392-091c9c620986.png">
</p>

Una vez escogida una casa y comenzado el juego, nos encontramos con una pantalla como la de arriba. Primero, se mencioan los distintos tipos de terreno que podemos observar en esa primera pantalla:

- arena
- roca
- duna
- colina

En la **arena** podemos mover nuestras unidades, pero no podemos construir edificios. Las **dunas** tiene un efecto similar sólo que las unidades se mueven más lento a través de ella. Para construir edificios y cuarteles, debemos encontrar una buena ubicación con terreno rocoso, ya que la **roca** es el único terreno lo suficientemente sólido para construir sobre él. Las **colinas** funcionan como barreras naturales, ya que ninguna unidad puede atravesarla.

#### Objetivo del juego

Cada facción comienza la partida con un centro de construcción, cuya destrucción significa el fin del juego para esa facción. Nuestro *objetivo* es entonces *defender el centro de construcción*, ayudandonos de distintos edificios que nos permitan crear distintas unidades, a la vez que intentamos *destruir el centro de construcción de cada facción opuesta*.

#### Construcción de los edificios

Para construir edificios es necesario contar con *energía* y *especia*, la moneda corriente en el mundo de Dune. Para conseguir energía tendremos que construir **trampas de aire**. Si bien se cuenta con cierta cantidad de especia inicial, si la agotamos construyendo edificios y unidades no podremos generarla nuevamente. Para esto, se deben crear unidades **cosechadoras**, que se encargan de transportar la especia de lugares en el mapa con arena especiada hacía la refinería. Un buen consejo es empezar construyendo una **fábrica pesada**, donde pueden entrenarse estas unidades **cosechadoras** que nos conseguirán más especia para poder seguir construyendo edificios y entrenando unidades de combate. Si colocamos el mouse por encima de los íconos de los distintos edificios, podemos obtener información acerca de los puntos de vida, los beneficios que otorga, y sus costos en especia y/o energía. En la imagen de abajo se puede observar un ejemplo de esta característica:

<p align="center">
  <img width="400" src="https://user-images.githubusercontent.com/75450615/177233059-9d2413ba-763a-4fea-9b8e-3626c743290b.png">
</p>

Para construir, se hace *click izquierdo* en el ícono con el edificio que queremos construir, y luego click izquierdo en el lugar del mapa que nos resulte conveniente, siempre y cuando haya una formación rocosa bajo suyo. Una vez que construímos un edificio, debemos esperar un tiempo para poder construir otro. Los tiempos de espera pueden ser visualizados mediante la colorización de los íconos de los distintos tipos de edificio (los colores varían según la facción escogida: verde para Ordos, azul para Atreides y rojo para Harkonnen). Si el ícono de un edificio se encuentra colorizado, significa que debemos esperar una cantidad de tiempo que es proporcional a la colorización del ícono. Se observa en la imagen de abajo un ícono en proceso de decolorizacion de una **trampa de aires** para el caso de haber escogido la casa Harkonnen:

<p align="center">
  <img width="400" src="https://user-images.githubusercontent.com/75450615/177225771-b2175a60-f6f3-46d2-bf74-86f2f50b74b8.png">
</p>

Una vez que tenemos al ícono con un como el último en la figura de arriba, podemos hacer *click izquierdo* en el edificio y colocarlos en algún punto válido del mapa. Podemos saber que edificio estamos a punto de colocar ya que se puede visualizar un "fantasma" del edificio sobre el puntero del mouse, como podemos notar en la imagen de abajo, donde estamos por colocar una **fábrica pesada** junto al **centro de construcción** luego de haber hecho *click izquierdo* sobre su ícono:

<p align="center">
  <img width="400" src="https://user-images.githubusercontent.com/75450615/177226554-4f9d766e-67a8-4e91-9bf8-74fcceb1f95d.png">
</p>

Los tiempos de construcción y de entrenamiento se ven extendidos en caso de haber una escasez de *energía*, siendo estos proporcionales a la deuda energética. Ahora que tenemos edificios, podemos empezar a crear unidades.

<p align="center">
  <img width="400" src="https://user-images.githubusercontent.com/75450615/175114388-1fd3d5fa-5ec2-4fd6-8fe1-182b3a7490e6.jpg">
</p>

#### Creacion de Unidades

Cada unidad tiene un costo en especia y un edificio creador. Las unidades de **infantería** se crean desde el **cuartel**. Los **vehículos** se crean en la **fábrica ligera** y la **fábrica pesada**. Cuanto más edificios creadores para una unidad, menos tiempo esta tarda en crearse. Sin embargo, según el tipo de unidad, existe solamente un edificio creador del cual emergen las unidades. Identificamos al edificio creador de unidades como aquel con un marcador de color blanco encima, como se puede apreciar en la figura de abajo:

<p align="center">
  <img width="400" src="https://user-images.githubusercontent.com/75450615/177223950-3b04154f-0f1b-4e82-8a6f-c57ece1ff9c1.png">
</p>

El primer edificio creador de algún tipo de unidad es designado por defecto, pero si queremos apuntar otro edificio del mismo tipo como creador, seleccionamos el edificio y damos *click derecho* (la selección del edificio puede ser con un *click izquierdo* sobre el edificio o mediante una *selección* de mouse, manteniendo *click izquierdo* desde un punto del mapa hasta otro, definiendo de esta manera un rectángulo donde se seleccionan las unidades y/o edificios que se encuentran en su interior y nos pertenecen). Existen unidades en los que el creador no será suficiente, y necesitamos tener una estructura **palacio** en pie para poder crearlas, como es el caso de la **infantería sardaukar** y el **tanque devastador**.

<p align="center">
  <img width="400" src="https://user-images.githubusercontent.com/75450615/175114794-d48a411c-3efd-4115-bb52-3600e057a8d6.jpg">
</p>

De manera similar a los edificios, podemos obtener información acerca de cada tipo de unidad manteniendo el mouse por encima de su ícono:

<p align="center">
  <img width="400" src="https://user-images.githubusercontent.com/75450615/177233435-94f45630-9148-4499-b7e7-934e98962ccf.png">
</p>

La velocidad de creación de unidades se ve afectada por la deuda energética de manera similar a los edificios: cuanto mayor sea, mayores serán los tiempos de espera entre el entrenamiento de dos unidades. Podemos de todas formas *encolar* unidades haciendo múltiples clicks en el ícono de la unidad que queremos crear, y en este caso se iniciará el entrenamiento de una nueva unidad una vez termine el entrenamiento de otra unidad. Notamos el tiempo faltante para la creación de una unidad mediante la barra que tenemos al lado de cada edificio creador:

<p align="center">
  <img width="400" src="https://user-images.githubusercontent.com/75450615/177224539-fa0861a1-16ec-434f-aec3-b1f2d6c2b4fc.png">
</p>

En la imagen de arriba se observa una **fábrica ligera** en el medio del entrenamiento de un **trike**. Mientras un edificio esté entrenando algún tipo de unidad, nos informará de que tipo es la unidad en cuestión mediante un ícono ubicado arriba de la barra que nos indica el progreso. Supongamos que queremos crear una **cosechadora** una vez que termine el entrenamiento del **trike**. Para esto, le damos click al ícono de la **cosechadora** mientras la **fábrica ligera** está ocupada con el **trike**, y automáticamente la fábrica iniciará la construcción de una **cosechadora** una vez terminada la construcción del **trike**. Debajo tenemos una imagen de lo que veríamos en la pantalla en este escenario:

<p align="center">
  <img width="400" src="https://user-images.githubusercontent.com/75450615/177225041-423f1470-fda4-4503-ac33-7c34dc602077.png">
</p>

#### Movimiento de unidades 

Una vez creadas las unidades, podemos desplazarlas de un punto del mapa a otro. Para hacer esto, debemos primero seleccionarlas y luego hacemos *click derecho* en algún punto del mapa, y nuestras unidades inciaran un trayecto hasta ese punto. Si en este punto se encuentra una **colina**, o hacemos click sobre una unidad o edificio aliado, nuestras unidades seleccionadas se moverán a la primera *posición libre del mapa más cercana* a ese punto.

<p align="center">
  <img width="400" src="https://user-images.githubusercontent.com/75450615/175114144-f3d67eb3-08d8-4318-ad0c-5baf73eafcbd.jpg">
</p>

#### Cosechadoras

Ya sabemos como crear unidades **cosechadoras** luego de construir una **fábrica pesada**. Para ponerlas a trabajar una vez que aparezcan junto a la fábrica, necesitamos primero una **refinería**, que es el edificio donde las cosechadoras depositan la especia colectada. Luego de construir la **refinería**, seleccionamos una o mas **cosechadoras** y luego hacemos click derecho en algún lugar del mapa con arena con especia, y automáticamente estas unidades iniciaran un ciclo de colectar especia y depositarla en la refinería, hasta agotar los recursos naturales de esa región del mapa.

<p align="center">
  <img width="400" src="https://user-images.githubusercontent.com/75450615/175112064-b6afb0f8-6863-46cb-81ff-50bf652c397f.jpg">
</p>

Cabe destacar que cada jugador cuenta con una capacidad máxima de especia, una vez alcanzada no podremos seguir agregando. Para aumentear nuestra capacidad máxima de especia que podemos colectar, debemos construir **silos**, pequeños depósitos de especia que podemos ubicar en cualquier punto del mapa.

<p align="center">
  <img width="400" src="https://user-images.githubusercontent.com/75450615/177231477-a20ba8cc-ba79-4f5b-9263-5c9c5f5c14b3.png">
</p>

En la imagen de arriba se observan varios **silos** junto a una **refineria** y a una unidad **cosechadora**.

#### Combate

Dentro de las unidades tenemos aquellas que se pueden utilizar para dañar otras unidades y edificios enemigos, como los rápidos **trikes** y los pesados **devastadores**. Para atacar al enemigo, debemos seleccionar nuestras unidades de ataque y luego hacemos *click derecho* sobre alguna unidad o edificio enemigo. Nuestras unidades ahora iniciaran un trayecto hasta estar a una distancia adecuada y luego comienzan a efectuar daño a los edificios o unidades enemigas que se encuentren en las proximidades del punto seleccionado. Cabe destacar que nuestras unidades no sólo atacan a la unidad enemiga seleccionada, sino también a cualquier unidad (o edificio) cercano a esa unidad.

#### Fin del juego

El juego termina cuando sólo queda un centro de construcción en pie, y la facción a la que pertenezca es declarada **ganadora** de la partida. Para cerrar un cliente sólo debemos clickear en la cruz que se encuentra en la esquina superior derecha de la ventana, y para cerrar el servidor debemos ingresar "q" en su respectiva terminal.

![image](https://user-images.githubusercontent.com/75450615/177231841-b01540a0-58f6-4cbf-84fd-49910e6f6d4b.png)

