# Erste Aufgabe
In der ersten Aufgabe sollte ich für einen ESP32S ein Programm schreiben, welches beim Start eine vorhandene SD-Karte sowie die Kamera konfiguriert und anschließend ein Foto mit der Kamera aufnimmt und auf die SD-Karte speichert.

## Probleme und Lösungsideen bei der Umsetzung der Aufgabe
Leider ist die Implementierung nicht vollständig gelungen. Derzeit erhalte ich eine Fehlermeldung beim Erzeugen eines Fotos mit der Kamera:

``` Serial
17:09:47.120 -> Kamera Initialisieren
17:09:47.190 -> E (1148) cam_hal: cam_dma_config(301): frame buffer malloc failed
17:09:47.190 -> E (1149) cam_hal: cam_config(389): cam_dma_config failed
17:09:47.229 -> E (1149) gdma: gdma_disconnect(238): no peripheral is connected to the channel
17:09:47.229 -> E (1155) camera: Camera config failed with error 0xffffffff
17:09:47.229 -> Kamera Initialisierung ist Fehlgeschlagen
17:09:47.229 -> -1
```


Leider konnte ich bisher keine Lösung für das Problem finden. Ich habe folgende Lösungswege ausprobiert:

- Validierung der Library-Konnektivität mit dem Kameramodul
- Aktualisierung / Downgrade der ESP32-Version
- Aktualisierung / Downgrade der Library-Version
- Internetrecherche


Das Problem scheint momentan darin zu liegen, dass die Kamera nicht über die richtigen Pins an das Modul angeschlossen ist. Es könnte aber auch sein, dass das Board oder die Kamera beschädigt ist, was den Datenfluss verhindert. Vielleicht könnte ich dieses Problem auch mit einer Beschreibung der Pins und des Kameramoduls weiter untersuchen, allerdings konnte ich bisher keine hilfreichen Online-Dokumentationen zu diesem Thema finden. Ich konnte keine korrekte Board-Dokumentation finden und auch nicht herausfinden, wie das Kameramodul heißt. Auf dem Modul konnte ich keine Typenbezeichnung erkennen.
