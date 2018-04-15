# Doorbell

Proyecto de un timbre, donde al pulsar reproduce una melodia y envia una notificacion por medio de un requerimiento a un archivo php en un servidor, el cual dispara las notificaciones. Tambien publica un mensaje en un servidor broker.

En la carpeta nodemcu, se encuentra el codigo para cargar en el nodemcu.
![alt text](https://raw.githubusercontent.com/gsampallo/doorbell/master/timbre.png "Esquematico")

En la carpeta python, se encuentra el archivo demo para recibir los mensajes del servidor broker.

Los archivos para imprimir la caja estan aca: https://www.thingiverse.com/thing:2839333

[![Video](https://img.youtube.com/vi/MVk2qMhyCGs/0.jpg)](https://www.youtube.com/watch?v=MVk2qMhyCGs)


UPDATE 2018-04-15

Agregue un control de debounce para el botón, dado que a veces sonaba dos veces sin motivo.