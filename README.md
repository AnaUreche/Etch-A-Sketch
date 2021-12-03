# Sketch - Virtual Etch-A-Sketch #

Sketch is a virtual representation of the famous Etch-A-Sketch.

![image](https://user-images.githubusercontent.com/78536620/144669398-720de0ec-9e18-4d42-83d8-fb8daedb6c0e.png)

1. To move the brush you press “a” for up, “d” for down, left key for left and right key for right.
2. New file button creates a new file.
3. Save file saves the file with the name written by the user in the textbox.
4. Open file button opens the file with the name written in the textbox.
5. You can write in the textbox after clicking on it once.
6. To delete what you just wrote you can press the “x” button on the right of the textbox.
7. You can choose which brush you want to use by clicking on one of them.
8. You can resize your brush by clicking on the “-” and “+” buttons.
9. The big button filled with black is the current colour of the brush. You can change it by choosing a colour from the colour palette.
10. To choose a colour of the palette you can just click on it.
11. The 2 small black buttons represent the colours you can use with the mix buttons. You can pick the colour for them by selecting the button and then the colour. To get the other colour you just do the same thing for the other small black button.
12. Mix 1 blends the colours selected above. The result is more beautiful if the colours you chose are from the same quadrant of the colour palette.
13. Mix 2 uses the selected colours for a more spontaneous transition,
14. The twin buttons 1, 2, 3 create a twin brush which moves along with your brush. If you do not want to use the twin mode anymore you can click on
it to unselect it.
15. You can change the colour of the canvas by clicking on the small white button at the bottom of the menu and then selecting a colour. However, it
will fill the entire canvas so you might lose your work. Also, the background colour and the brush colour can be both selected and then they will
change the colour at the same time, leaving the whole canvas with the same colour.


Results
-------

file:///home/s5216592/Documents/ProgrammingAssignments/Etch-A-Sketch/SampleResults/out7.bmp![image](https://user-images.githubusercontent.com/78536620/144681774-232f47ee-41cb-408e-bef1-1d3fd53174e5.png)

file:///home/s5216592/Documents/ProgrammingAssignments/Etch-A-Sketch/SampleResults/test9![image](https://user-images.githubusercontent.com/78536620/144681842-7ef60198-13ee-414f-81b6-81b5c2e57c98.png)


Developed and tested on Linux
-----------------------------

build: Sketch
	./Sketch

Sketch: Sketch.c
	clang Sketch.c -l SDL2 -l SDL2_image -l SDL2_ttf -o Sketch

Dependencies
------------

SDL2
