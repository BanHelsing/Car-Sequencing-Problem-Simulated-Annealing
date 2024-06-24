# Proyecto-Inteligencia-Artificial

## Como ejecutar el código
Primero se debe navegar a la carpera que este lo contiene.

Luego se ejecuta el comando ```cd build``` para navegar a la carpeta que contiene el Makefile.

Finalmente usando ```make``` el compilador construirá el ejecutable .out

Se puede usar el comando ```make clean``` para despejar el archivo creado.

Una vez se tenga "main", se puede correr con ```./main <iteraciones> <temperatura inicial> <recalentamientos> <decrecimiento> <instancias>```

Específicamente para cada uno de estos parametros:

1. iteraciones: Indica la cantida de pasadas que se le dará a la solución, este número será multiplicado por la cantidad de elementos que hay en la solución.

2. temperatura inicial: Indica la temperatura inicial al inicio de simulated annealing.

3. recalentamientos: La cantidad de recalentamientos que se usarán durante simulated annealing.

4. decrecimiento: Un número entre 0 y 1 representando el valor por el cual la temperatura se multiplicará, los experimentos se realizaron con 0.99.

5. instancias: String que puede tomar como valor "all" o "single". De ser el primero se evaluarán todos los problemas dentro de la carpeta indicada en la varible ```path``` dentro del código. De ser single se evaluará solo la instancia indicada en ```filename```.

Cuando el códio termine de ejecutarse, se creará un log correspondiente a cada instanca en formato .csv, este contiene las iteraciones con su respectivo fitness, y si es que se realizó movimiento o no. También en el header se encuentra información adicional de la ejecuión, como su tiempo, fitness inicial y final, y otros.

## analytics.py

Como extra, existe tambien ```analytics.py```, que al ejecutarse creará un gráfico del último log que exista dentro de la carpeta logs.

## clean.py

La mayoría de las instancias tenían errores, como trailing whitespace, así como lineas vacías al final. Para lidiar con esto se usó ```clean.py``` para limpiarlas.