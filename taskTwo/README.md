# Zweite Aufgabe
In der zweiten Aufgabe sollte ich ein Programm für den ESP32S schreiben, das den Distanzsensor verwendet. Sobald ein Objekt eine bestimmte Nähe unterschreitet, soll das Programm automatisch ein Foto aufnehmen und auf einer SD-Karte speichern.

Da die Kamera aufgrund der in Aufgabe 1 beschriebenen Probleme nicht zuverlässig funktioniert, wird das Foto an dieser Stelle durch eine simulierte Datei ersetzt.

## Probleme und Lösungsideen bei der Umsetzung der Aufgabe

Beim Zugriff auf den VL53L8CX traten wiederholt folgende Fehlermeldungen auf:

```
E (xxx) i2c.master: I2C transaction timeout detected  
E (xxx) i2c.master: probe device timeout. Please check if xfer_timeout_ms and pull-ups are correctly set up
```

Ich konnte das I2C-Problem bisher nicht endgültig lösen. Um es weiter zu untersuchen, müsste ich:
- Das I2C-Konzept besser verstehen
- Dokumentation zum ESP32S3-Pinout und zum Sensor genauer recherchieren.
