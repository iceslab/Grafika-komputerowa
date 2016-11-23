Zadanie1
Program wczytuje bitmapê o nazwie texture.bmp i wyœwietla j¹ nak³adaj¹c efekty:
- przeskalowanie 3:1 w poziomie, 1:2 w pionie
- gradient jednej sk³adowej koloru (niebieski) od prawego górnego rogu (pg) do lewego dolnego rogu (ld), 
w sposób proporcjonalny do odleg³oœci geometrycznej od tych wierzcho³ków 
(np. kolor = kolor * odleg³oœæ(bie¿¹cy piksel, ld) / odleg³oœæ(ld, pg)).

Zadanie2
Program wczytuje bitmapê o nazwie texture.bmp, wyœwietla j¹ i animuje nak³adaj¹c efekty:
- liniowa interpolacja przesuniêtych wzglêdem siebie obrazków
- skrêcenie wierzcho³ków na których rozci¹gniêty jest obrazek

Oba programy nale¿y skompilowaæ, a nastêpnie w katalogu, w którym zosta³ utworzony plik wykonywalny umieœciæ
shadery ("fragmentShader.cg" i "vertexShader.cg") oraz bitmapê, któr¹ chcemy wyœwietliæ o nazwie "texture.bmp".
Przyk³adowe bitmapy zawarte s¹ w folderze z kodem Ÿród³owym.