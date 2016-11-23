Zadanie1
Program wczytuje bitmap� o nazwie texture.bmp i wy�wietla j� nak�adaj�c efekty:
- przeskalowanie 3:1 w poziomie, 1:2 w pionie
- gradient jednej sk�adowej koloru (niebieski) od prawego g�rnego rogu (pg) do lewego dolnego rogu (ld), 
w spos�b proporcjonalny do odleg�o�ci geometrycznej od tych wierzcho�k�w 
(np. kolor = kolor * odleg�o��(bie��cy piksel, ld) / odleg�o��(ld, pg)).

Zadanie2
Program wczytuje bitmap� o nazwie texture.bmp, wy�wietla j� i animuje nak�adaj�c efekty:
- liniowa interpolacja przesuni�tych wzgl�dem siebie obrazk�w
- skr�cenie wierzcho�k�w na kt�rych rozci�gni�ty jest obrazek

Oba programy nale�y skompilowa�, a nast�pnie w katalogu, w kt�rym zosta� utworzony plik wykonywalny umie�ci�
shadery ("fragmentShader.cg" i "vertexShader.cg") oraz bitmap�, kt�r� chcemy wy�wietli� o nazwie "texture.bmp".
Przyk�adowe bitmapy zawarte s� w folderze z kodem �r�d�owym.