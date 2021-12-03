// This is the main SDL include file
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <unistd.h>

/*source from Eike Anderson: ~ https://brightspace.bournemouth.ac.uk/d2l/le/content/67405/viewContent/465014/View */
typedef struct RGBcolour	// structure containing color values
{
	Uint8 r;
	Uint8 g;
	Uint8 b;
}
RGBcolour;

typedef struct Window	// structure containing the position, dimensions and the color of the window
{
	int winPosX;
	int winPosY;
	int winWidth;
	int winHeight;
	RGBcolour Color;	// I use the color of the window later for the menu background
}
Window;
/*source from Eike Anderson */

typedef struct canvas	// structure containing the position, dimensions and the color of the drawing area
{
	int x;	// x position in the window
	int y;	// y position in the window
	int width;
	int height;
	RGBcolour Color;
}
canvas;

typedef struct brush	// this structure represents the drawing tool that is being used
{
	int x;	// x position in the window
	int y;	// y position in the window
	int radius;	// radius of the circle that is drawn in case the brush Type is no2 (empty circle)
	// the width and height are always equal (the brush is a square) but I put both values in case I ever wanted to create another type of brush
	int width;	// width of the rectangle that is drawn in case the brush Type is no1 (filled rectangle)
	int height;	// height of the rectangle that is drawn in case the brush Type is no1 (filled rectangle)
	RGBcolour Color;	// holds the current color of the drawing tool (brush)
	int brushType;	// 1 for square and 2 for circle
}
brush;

typedef struct button	// this structure holds the values needed to draw a button
{
	// I am using it as a template for drawing every button
	int x;	// x position in the window
	int y;	// y position in the window
	int width;
	int height;
	bool isSelected;	// holds the state of the button
	// these are the colors of the border of the button
	RGBcolour BorderColorUnselected;
	RGBcolour BorderColorSelected;
	// these are the colors of the inside of the button
	RGBcolour ColorUnselected;
	RGBcolour ColorSelected;
}
button;

void initialiseColor(RGBcolour *Color, int r, int g, int b)
{
	// this function put the values r, g, b in the Color struct (by reference)
	Color->r = r;
	Color->g = g;
	Color->b = b;
}

void initializeCanvas(canvas *c, int x, int y, int w, int h, RGBcolour Color)
{
	// this function put the given in the Canvas struct (by reference)
	c->x = x;
	c->y = y;
	c->width = w;
	c->height = h;
	c->Color = Color;
}

void initializeBrush(brush *b, int x, int y, int r, int width, int height, RGBcolour Color, int brushType)
{
	// this function put the given in the Brush struct (by reference)
	b->x = x;
	b->y = y;
	b->radius = r;
	b->Color = Color;
	b->width = width;
	b->height = height;
	b->brushType = brushType;
}

void initialiseButton(button *btn, int x, int y, int width, int height, bool isSelected, RGBcolour BorderCU, RGBcolour BCS, RGBcolour CU, RGBcolour CS)
{
	// this function put the given in the Button struct (by reference)
	btn->x = x;
	btn->y = y;
	btn->width = width;
	btn->height = height;
	btn->isSelected = isSelected;
	btn->BorderColorUnselected = BorderCU;
	btn->BorderColorSelected = BCS;
	btn->ColorSelected = CS;
	btn->ColorUnselected = CU;
}

/*source from Eike Anderson: ~ https://brightspace.bournemouth.ac.uk/d2l/le/content/67405/viewContent/466210/View */
struct RGBcolour getIPixelColour(SDL_Surface *img, int x, int y)
{
	// this function return the color values (as an RGBcolor struct) of the pixel with the position (x,y) in the surface img
	struct RGBcolour pcol;
	Uint32 pixel, *pixels = (Uint32*) img->pixels;
	/*should really test if the coordinates are valid */
	SDL_GetRGB(pixels[x + y *img->w], img->format, &(pcol.r), &(pcol.g), &(pcol.b));
	return pcol;
}
/*source from Eike Anderson */

/*source based on Eike Anderson: ~ https://brightspace.bournemouth.ac.uk/d2l/le/content/67405/viewContent/466210/View */
void Icircle(SDL_Surface *img, int x0, int y0, int radius, int xmin, int xmax, int ymin, int ymax, RGBcolour col)
{
	int x = radius, y = 0, error = 1 - radius;
	Uint32 pixel = SDL_MapRGB(img->format, col.r, col.g, col.b);
	Uint32 *pixels = (Uint32*) img->pixels;
	while (x >= y)
	{ /*1 draw call for each octant - ensure coordinates are valid before drawing */

		//if(x>=0 && x >= xmin && x <= xmax && y>=0 &&y >= ymin && y <= ymax)        
		if (x0 >= xmin && x0 <= xmax && y0 >= ymin && y0 <= ymax)
		{
			if ((x + x0) >= xmin && (x + x0) < xmax && (y + y0) >= ymin && (y + y0) < ymax)
				pixels[(x + x0) + (y + y0) *img->w] = pixel;
			/*draw point in octant 1 if coordinate is valid */
			if ((y + x0) >= xmin && (y + x0) < xmax && (x + y0) >= ymin && (x + y0) < ymax)
				pixels[(y + x0) + (x + y0) *img->w] = pixel;
			/*draw point in octant 2 if coordinate is valid */
			if ((-x + x0) >= xmin && (-x + x0) < xmax && (y + y0) >= ymin && (y + y0) < ymax)
				pixels[(-x + x0) + (y + y0) *img->w] = pixel;
			/*draw point in octant 3 if coordinate is valid */
			if ((-y + x0) >= xmin && (-y + x0) < xmax && (x + y0) >= ymin && (x + y0) < ymax)
				pixels[(-y + x0) + (x + y0) *img->w] = pixel;
			/*draw point in octant 4 if coordinate is valid */
			if ((-x + x0) >= xmin && (-x + x0) < xmax && (-y + y0) >= ymin && (-y + y0) < ymax)
				pixels[(-x + x0) + (-y + y0) *img->w] = pixel;
			/*draw point in octant 5 if coordinate is valid */
			if ((-y + x0) >= xmin && (-y + x0) < xmax && (-x + y0) >= ymin && (-x + y0) < ymax)
				pixels[(-y + x0) + (-x + y0) *img->w] = pixel;
			/*draw point in octant 6 if coordinate is valid */
			if ((x + x0) >= xmin && (x + x0) < xmax && (-y + y0) >= ymin && (-y + y0) < ymax)
				pixels[(x + x0) + (-y + y0) *img->w] = pixel;
			/*draw point in octant 7 if coordinate is valid */
			if ((y + x0) >= xmin && (y + x0) < xmax && (-x + y0) >= ymin && (-x + y0) < ymax)
				pixels[(y + x0) + (-x + y0) *img->w] = pixel;
			/*draw point in octant 8 if coordinate is valid */
		}
		y++; /*increment y coordinate */
		if (error < 0)
		{
			error += 2 *y + 1;
		}
		else
		{
			x--;
			error += 2 *(y - x) + 1;
		}
	}
}
/*source based on Eike Anderson ends here */

void FRectangle(SDL_Surface *img, int x0, int y0, int width, int height, int xmin, int xmax, int ymin, int ymax, RGBcolour col)
{
	// this function draws a filled rectangle on the surface img sent by reference
	// the rectangle's center is (x0,y0), and the color is col (RGBcolor)
	// the rectangle is restricted by values xmin, xmax, ymin, ymax to stop if from drawing outside these values (usefull when drawing ojn the canvas)
	int x = 0, y = 0;
	Uint32 pixel = SDL_MapRGB(img->format, col.r, col.g, col.b);
	Uint32 *pixels = (Uint32*) img->pixels;
	for (x = x0 - width / 2; x < x0 + width / 2 && x < img->w; x++)
	{
		if (x >= 0 && x >= xmin && x <= xmax)
			for (y = y0 - height / 2; y < y0 + height / 2 && y < img->h; y++)
			{
				if (y >= 0 && y >= ymin && y <= ymax)
				{
					pixels[x + y *img->w] = pixel;
				}
			}
	}
}

void Irectagle(SDL_Surface *img, int x0, int y0, int width, int height, RGBcolour col)
{
	// this function draws the contour of a rectangle on the surface img sent by reference
	// the rectangle's center is (x0,y0), and the color is col (RGBcolor)
	int x = 0, y = 0;
	Uint32 pixel = SDL_MapRGB(img->format, col.r, col.g, col.b);
	Uint32 *pixels = (Uint32*) img->pixels;

	for (x = x0 - width / 2; x < x0 + width / 2 && x < img->w; x++)
	{
		if (x == x0 - width / 2 || x == x0 + width / 2 - 1)
			for (y = y0 - height / 2; y < y0 + height / 2 && y < img->h; y++)
				pixels[x + y *img->w] = pixel;
		else
			for (y = y0 - height / 2; y < y0 + height / 2 && y < img->h; y++)
				if (y == y0 - height / 2 || y == y0 + height / 2 - 1)
					pixels[x + y *img->w] = pixel;
	}
}

void drawButton(SDL_Surface *img, button *myBtn, Window *myW)
{
	// this function draws the button with the required colors specific to the "isSelected" state    
	if (myBtn->isSelected)
	{
		FRectangle(img, myBtn->x, myBtn->y, myBtn->width, myBtn->height, 0, myW->winWidth, 0, myW->winHeight, myBtn->ColorSelected);
		Irectagle(img, myBtn->x, myBtn->y, myBtn->width, myBtn->height, myBtn->BorderColorSelected);
	}
	else
	{
		FRectangle(img, myBtn->x, myBtn->y, myBtn->width, myBtn->height, 0, myW->winHeight, 0, myW->winHeight, myBtn->ColorUnselected);
		Irectagle(img, myBtn->x, myBtn->y, myBtn->width, myBtn->height, myBtn->BorderColorUnselected);
	}
}

void addText(SDL_Surface *img, int x, int y, int w, int h, RGBcolour col, TTF_Font *font, char text[20])
{
	// this function adds text to the surface given by reference 
	// it uses TTF fonts
	SDL_Color textColor = { col.r, col.g, col.b};
	SDL_Surface *textS = TTF_RenderText_Solid(font, text, textColor);
	SDL_Rect textLocation = { x, y, w, h};	
    // location of the text in the window
	SDL_BlitSurface(textS, NULL, img, &textLocation);	// blits the text at the correct possition in surface img using the give position and sizes
	SDL_FreeSurface(textS);	// frees the text surface 
}

void colourPalette(SDL_Surface *img, int x0, int y0)
{
	// this is a very rought color pallet 
	// draws colors gradually on the surface given by reference
	RGBcolour col;
	col.r = 0;
	col.g = 0;
	col.b = 0;
	int x = 0, y = 0;
	Uint32 pixel = SDL_MapRGB(img->format, (col.r), (col.g), (col.b));
	Uint32 *pixels = (Uint32*) img->pixels;
	//draws the 2 quadrants on the left side of the colors pallete
	for (col.r = 0; col.r < 252; col.r = col.r + 3)
	{
		for (col.g = 0; col.g < 252; col.g = col.g + 3)
		{
			for (col.b = 0; col.b < 252; col.b = col.b + 3)
			{
				pixel = SDL_MapRGB(img->format, col.r, col.g, col.b);
				pixels[x + x0 + (y + y0) *img->w] = pixel;
				x++;
			}
			x = 0;
			y++;
		}
		y = 84;
	}
	y = 0;
	x = 84;	// shifts the second set of quadrants to the right
	//draws the other 2 quadrants on the right side of the colors pallete
	for (col.g = 0; col.g < 252; col.g = col.g + 3)
	{
		for (col.b = 0; col.b < 252; col.b = col.b + 3)
		{
			for (col.r = 0; col.r < 252; col.r = col.r + 3)
			{
				pixel = SDL_MapRGB(img->format, col.r, col.g, col.b);
				pixels[x + x0 + (y + y0) *img->w] = pixel;
				x++;
			}
			x = 84;
			y++;
		}
		y = 84;
	}
}

int checkButton(int x, int y, button *btn1, button *btn2, button *btn3, button *btn4, button *btn5, button *btn6, button *btn7, button *btn8, button *btn9, button *btn10, button *btn11, button *btn12, button *btn13, button *btn14, button *btn15, button *btn16, button *btn17, button *btn18)
{
	// checks if the position of the mouse id in the area of any buttons
	if (x >= btn1->x - btn1->width / 2 && x <= btn1->x + btn1->width / 2 && y >= btn1->y - btn1->height / 2 && y <= btn1->y + btn1->height / 2)
		return 1;
	if (x >= btn2->x - btn2->width / 2 && x <= btn2->x + btn2->width / 2 && y >= btn2->y - btn2->height / 2 && y <= btn2->y + btn2->height / 2)
		return 2;
	if (x >= btn3->x - btn3->width / 2 && x <= btn3->x + btn3->width / 2 && y >= btn3->y - btn3->height / 2 && y <= btn3->y + btn3->height / 2)
		return 3;
	if (x >= btn4->x - btn4->width / 2 && x <= btn4->x + btn4->width / 2 && y >= btn4->y - btn4->height / 2 && y <= btn4->y + btn4->height / 2)
		return 4;
	if (x >= btn5->x - btn5->width / 2 && x <= btn5->x + btn5->width / 2 && y >= btn5->y - btn5->height / 2 && y <= btn5->y + btn5->height / 2)
		return 5;
	if (x >= btn6->x - btn6->width / 2 && x <= btn6->x + btn6->width / 2 && y >= btn6->y - btn6->height / 2 && y <= btn6->y + btn6->height / 2)
		return 6;
	if (x >= btn7->x - btn7->width / 2 && x <= btn7->x + btn7->width / 2 && y >= btn7->y - btn7->height / 2 && y <= btn7->y + btn7->height / 2)
		return 7;
	if (x >= btn8->x - btn8->width / 2 && x <= btn8->x + btn8->width / 2 && y >= btn8->y - btn8->height / 2 && y <= btn8->y + btn8->height / 2)
		return 8;
	if (x >= btn9->x - btn9->width / 2 && x <= btn9->x + btn9->width / 2 && y >= btn9->y - btn9->height / 2 && y <= btn9->y + btn9->height / 2)
		return 9;
	if (x >= btn10->x - btn10->width / 2 && x <= btn10->x + btn10->width / 2 && y >= btn10->y - btn10->height / 2 && y <= btn10->y + btn10->height / 2)
		return 10;
	if (x >= btn11->x - btn11->width / 2 && x <= btn11->x + btn11->width / 2 && y >= btn11->y - btn11->height / 2 && y <= btn11->y + btn11->height / 2)
		return 11;
	if (x >= btn12->x - btn12->width / 2 && x <= btn12->x + btn12->width / 2 && y >= btn12->y - btn12->height / 2 && y <= btn12->y + btn12->height / 2)
		return 12;
	if (x >= btn13->x - btn13->width / 2 && x <= btn13->x + btn13->width / 2 && y >= btn13->y - btn13->height / 2 && y <= btn13->y + btn13->height / 2)
		return 13;
	if (x >= btn14->x - btn14->width / 2 && x <= btn14->x + btn14->width / 2 && y >= btn14->y - btn14->height / 2 && y <= btn14->y + btn14->height / 2)
		return 14;
	if (x >= btn15->x - btn15->width / 2 && x <= btn15->x + btn15->width / 2 && y >= btn15->y - btn15->height / 2 && y <= btn15->y + btn15->height / 2)
		return 15;
	if (x >= btn16->x - btn16->width / 2 && x <= btn16->x + btn16->width / 2 && y >= btn16->y - btn16->height / 2 && y <= btn16->y + btn16->height / 2)
		return 16;
	if (x >= btn17->x - btn17->width / 2 && x <= btn17->x + btn17->width / 2 && y >= btn17->y - btn17->height / 2 && y <= btn17->y + btn17->height / 2)
		return 17;
	if (x >= btn18->x - btn18->width / 2 && x <= btn18->x + btn18->width / 2 && y >= btn18->y - btn18->height / 2 && y <= btn18->y + btn18->height / 2)
		return 18;
	// checks if the position of the mouse id in the area of the color palette
	if (x <= 188 && x >= 20 && y <= 358 && y >= 190)
		return -1;
	return 0;	// returns 0 as a default case
}

struct RGBcolour mixColors1(RGBcolour col1, RGBcolour col2, RGBcolour col)
{
	// creates a new color every time it is called
	// it is used to give a gradient effect between 2 colors
	// returns an RGBstruct    	
	int division = 100;

	col.r += (col2.r - col1.r) / division;
	col.g += (col2.g - col1.g) / division;
	col.b += (col2.b - col1.b) / division;

	return col;
}

struct RGBcolour mixColors2(RGBcolour col1, RGBcolour col2, RGBcolour col, int iMix)
{
	// creates a new color every time it is called
	// it is used to give a chaotic transition between 2 colors
	// returns an RGBstruct    	
	int division = 100;

	col.r += (col2.r *(iMix) + col1.r *(division - iMix)) / division;
	col.g += (col2.g *(iMix) + col1.g *(division - iMix)) / division;
	col.b += (col2.b *(iMix) + col1.b *(division - iMix)) / division;

	return col;
}

int findFile(char str[500], char text[400], brush *myBrush)
{
	// this function splits the line read from the out.txt file into tokens using strtok()
	int i = 0;
	char temp[11][10];

	/*source based on www.cplusplus.com: ~ http://www.cplusplus.com/reference/cstring/strtok/ */
	char *pchr;
	pchr = strtok(str, " \n");	// the first token holds the name of a file		    
	if (!strcmp(pchr, text))	// checks if the name of the file read in this line is the same with the one we need
	{
		while (pchr != NULL)
		{
			i++;
			strcpy(temp[i], pchr);	// saves the token into substrings		
			fflush(stdout);
			pchr = strtok(NULL, " \n");	//splits the string again
		}
		/*source based on www.cplusplus.com end here */

		// we assign the values for the brush to myBrush (which was sent by reference)
		RGBcolour Col;
		Col.r = atoi(temp[7]);
		Col.g = atoi(temp[8]);
		Col.b = atoi(temp[9]);
		initializeBrush(myBrush, atoi(temp[2]), atoi(temp[3]), atoi(temp[4]), atoi(temp[5]), atoi(temp[6]), Col, atoi(temp[10]));
		return 1;	// returns 1 if the file we want is the same with the one in the line we are reading               
	}
	else
	{
		return 0;	// returns 0 if the file we want is not the same with the one in the line we are reading
	}
}

int main(int argc, char *argv[])
{
	SDL_Texture * texture;	// the texture used for rendering
	// initialise important variables needed in the first while
	int mouseX = 0;	// holds the x position of the mouse in the window
	int mouseY = 0;	// holds the y position of the mouse in the window
	int brushMoved = 0;	// changes when brush has changed its position
	int buttonClicked = 0;	// changes when the buttons with labels (text) are clicked
	int updateTexture = 1;	// changes when an sdl event occures
	int initMix = 0;	// keeps track if the blend (mix) has started
	int iMix = 1;	// holds the id of the current mix
	char inputText[200] = "";	// holds the text theat is written by the user in the textbox
	int go = 1;	// variable used to track when the rendering should stop (exiting the while)

	//
	//initialise menu    
	//
	Window myW;
	brush myBrush;
	canvas myCanvas;
	RGBcolour myColor, blend1, blend2;
	RGBcolour BCU, BCS, CU, CS;	// the colors used for the buttons 
	myW.winPosX = 100;
	myW.winPosY = 100;
	myW.winWidth = 900;
	myW.winHeight = 500;
	initialiseColor(&myColor, 25, 95, 90);	// color of the menu background
	myW.Color = myColor;
	initialiseColor(&myColor, 0, 0, 0);	// the brush colors is black every time the program starts and when a new file is being created
	initializeBrush(&myBrush, 300, 0, 2, 4, 4, myColor, 1);
	initialiseColor(&myColor, 255, 255, 255);	// the canvas is white every time the program starts and when a new file is being created
	initializeCanvas(&myCanvas, 600, 250, 600, 500, myColor);
	initialiseColor(&BCU, 10, 70, 70);	// colors on the inside of the button when it is not selected
	initialiseColor(&BCS, 0, 25, 25);	// colors on the inside of the button when it is selected
	initialiseColor(&CU, 10, 200, 200);	// colors of the contour of the button when it is not selected
	initialiseColor(&CS, 10, 150, 150);	// colors of the contour of the button when it is selected	

	button newBtn, saveBtn, openBtn, textBox, deleteTextBtn, squareBtn, circleBtn, sizeSmallerBtn, sizeBiggerBtn, brushCol, color1Btn, color2Btn, mix1btn, mix2btn, t1btn, t2btn, t3btn, BGCol;
	initialiseButton(&newBtn, 60, 25, 88, 30, false, BCU, BCS, CU, CS);
	initialiseButton(&saveBtn, 150, 25, 88, 30, false, BCU, BCS, CU, CS);
	initialiseButton(&openBtn, 240, 25, 88, 30, false, BCU, BCS, CU, CS);
	initialiseButton(&squareBtn, 150, 120, 20, 20, true, BCU, BCS, CU, CS);
	initialiseButton(&circleBtn, 180, 120, 20, 20, false, BCU, BCS, CU, CS);
	initialiseButton(&sizeSmallerBtn, 150, 150, 20, 20, false, BCU, BCS, CU, CS);
	initialiseButton(&sizeBiggerBtn, 180, 150, 20, 20, false, BCU, BCS, CU, CS);
	initialiseButton(&deleteTextBtn, 274, 77, 20, 20, false, BCU, BCS, CU, CS);
	initialiseColor(&myColor, 255, 255, 255);	// white color used to fill the textbox and the buttons for the color
	initialiseButton(&textBox, 139, 77, 246, 22, false, BCU, BCS, myColor, myColor);
	initialiseButton(&mix1btn, 250, 270, 70, 30, false, BCU, BCS, CU, CS);
	initialiseButton(&mix2btn, 250, 310, 70, 30, false, BCU, BCS, CU, CS);
	initialiseButton(&t1btn, 90, 383, 20, 20, false, BCU, BCS, CU, CS);
	initialiseButton(&t2btn, 115, 383, 20, 20, false, BCU, BCS, CU, CS);
	initialiseButton(&t3btn, 140, 383, 20, 20, false, BCU, BCS, CU, CS);
	initialiseButton(&BGCol, 185, 415, 20, 20, false, BCU, BCS, myColor, myColor);
	initialiseColor(&myColor, 0, 0, 0);	// black color used to fill the button which shows the color of the brush
	initialiseButton(&brushCol, 250, 135, 50, 50, true, BCU, BCS, myColor, myColor);
	// black is preset as the color fo color1Btn and color2Btn
	initialiseButton(&color1Btn, 230, 230, 20, 20, false, BCU, BCS, myColor, myColor);
	initialiseButton(&color2Btn, 270, 230, 20, 20, false, BCU, BCS, myColor, myColor);
	//
	//end initialise menu
	//

	/*source from Eike Anderson: ~https://brightspace.bournemouth.ac.uk/content/enforced/67405-TBC/pointy.c?_&d2lSessionVal=fA1HXzp39pGB8EcvpxIfRs0sY&ou=67405	*/
	/*This is our initialisation phase

	   SDL_Init is the main initialisation function for SDL
	   It takes a 'flag' parameter which we use to tell SDL what systems we are going to use
	   Here, we want to initialise everything, so we give it the flag for this.
	   This function also returns an error value if something goes wrong,
	   so we can put this straight in an 'if' statement to check and exit if need be */
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		/*Something went very wrong in the initialisation, all we can do is exit */
		perror("Whoops! Something went very wrong, cannot initialise SDL :(");
		return -1;
	}

	/*Now we have got SDL initialised, we are ready to create a window! */
	SDL_Window *window = SDL_CreateWindow("Etch-A-Sketch", /*The first parameter is the window title */
		myW.winPosX, myW.winPosY,
		myW.winWidth, myW.winHeight,
		SDL_WINDOW_SHOWN);

	/*The last parameter lets us specify a number of options.
	   Here, we tell SDL that we want the window to be shown and that it can be resized.	   

	   The SDL_CreateWindow function returns an SDL_Window.
	   This is a structure which contains all the data about our window (size, position, etc).
	   We will also need this when we want to draw things to the window.
	   This is therefore quite important we do not lose it! */

	/*The SDL_Renderer is a structure that handles rendering.
	   It will store all of SDL's internal rendering related settings.
	   When we create it we tell it which SDL_Window we want it to render to.
	   That renderer can only be used for this window (yes, this means we can have multiple windows). */
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

	if (window == NULL)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		fflush(stdout);
		exit(EXIT_FAILURE); /*crash out if there has been an error */
	}

	/*We are now preparing for our main loop.
	   This loop will keep going round until we exit from our program by changing the int 'go' to the value false (0).
	   This loop is an important concept and forms the basis of most SDL programs you will be writing.
	   Within this loop we generally do the following things:
	     *Check for input from the user (and do something about it!)
	     *Update our graphics
	     *Draw our graphics
    */

	SDL_Surface * surface2;	// temporary surface
	surface2 = SDL_CreateRGBSurface(0, myW.winWidth, myW.winHeight, 32, 0, 0, 0, 0);	// create surface2 with the specified sizes
	SDL_Surface *surface = SDL_ConvertSurfaceFormat(surface2, SDL_PIXELFORMAT_RGBA8888, 0);	// initialize surface 2 and convert to the specified pixel format        
	SDL_FreeSurface(surface2);	// free the temporary surface as it is not needed anymore

	/*source from Eike Anderson ends here */

	//
	// draw menu for the first time since the program started
	//	

	// draws the canvas (drawing surface)
	FRectangle(surface, myCanvas.x, myCanvas.y, myCanvas.width, myCanvas.height, myCanvas.x - myCanvas.width / 2, myCanvas.x + myCanvas.width / 2, myCanvas.y - myCanvas.height / 2, myCanvas.y + myCanvas.height / 2, myCanvas.Color);
	// draws a rectangle for the menu background
	FRectangle(surface, (myW.winWidth - myCanvas.width) / 2, myW.winHeight / 2, (myW.winWidth - myCanvas.width), myW.winHeight, 0, myW.winWidth, 0, myW.winHeight, myW.Color);
	colourPalette(surface, 20, 190);	// draws the color palette from wich the colors can be chosen
	// adding the buttons
	drawButton(surface, &newBtn, &myW);
	drawButton(surface, &saveBtn, &myW);
	drawButton(surface, &openBtn, &myW);
	drawButton(surface, &squareBtn, &myW);
	FRectangle(surface, squareBtn.x, squareBtn.y, squareBtn.width - 5, squareBtn.height - 5, squareBtn.x - squareBtn.width / 2, squareBtn.x + squareBtn.width / 2, squareBtn.y - squareBtn.height / 2, squareBtn.y + squareBtn.height / 2, squareBtn.BorderColorSelected);
	drawButton(surface, &circleBtn, &myW);
	Icircle(surface, circleBtn.x, circleBtn.y, 7, circleBtn.x - circleBtn.width / 2, circleBtn.x + circleBtn.width / 2, circleBtn.y - circleBtn.height / 2, circleBtn.y + circleBtn.height / 2, circleBtn.BorderColorSelected);
	drawButton(surface, &sizeSmallerBtn, &myW);
	drawButton(surface, &sizeBiggerBtn, &myW);
	drawButton(surface, &deleteTextBtn, &myW);
	drawButton(surface, &textBox, &myW);
	drawButton(surface, &mix1btn, &myW);
	drawButton(surface, &mix2btn, &myW);
	drawButton(surface, &color1Btn, &myW);
	drawButton(surface, &color2Btn, &myW);
	drawButton(surface, &t1btn, &myW);
	drawButton(surface, &t2btn, &myW);
	drawButton(surface, &t3btn, &myW);
	drawButton(surface, &BGCol, &myW);
	drawButton(surface, &brushCol, &myW);

	// adding the text
	/*source based on user kdyz : ~  https://stackoverflow.com/questions/22886500/how-to-render-text-in-sdl2 */
	TTF_Init();	// initialisation function of TFF
	TTF_Font *font = TTF_OpenFont("/usr/share/fonts/sil-padauk/Padauk.ttf", 20);	// initialising the font that is going to be used
	if (!font)	// checking for errors
	{
		printf("TTF_OpenFont: %s\n", TTF_GetError());
	}
	else
	{
		// this is the text used as labels for the buttons        
		addText(surface, 25, 10, 70, 20, BCS, font, "New file");
		addText(surface, 115, 10, 70, 20, BCS, font, "Save file");
		addText(surface, 205, 10, 70, 20, BCS, font, "Open file");
		addText(surface, 230, 256, 70, 20, BCS, font, "Mix 1");
		addText(surface, 230, 297, 70, 20, BCS, font, "Mix 2");
		addText(surface, 145, 135, 70, 20, BCS, font, "-");
		addText(surface, 176, 136, 70, 20, BCS, font, "+");
		addText(surface, 269, 62, 20, 20, BCS, font, "x");
		addText(surface, 85, 370, 100, 20, BCS, font, "1");
		addText(surface, 110, 370, 100, 20, BCS, font, "2");
		addText(surface, 135, 370, 100, 20, BCS, font, "3");
		// this is the text used on the menu background
		addText(surface, 25, 105, 70, 20, BCS, font, "Brush type");
		addText(surface, 25, 135, 70, 20, BCS, font, "Brush size");
		addText(surface, 25, 40, 100, 20, BCS, font, "Name of the artwork");
		addText(surface, 25, 370, 100, 20, BCS, font, "Twin");
		addText(surface, 25, 401, 100, 20, BCS, font, "Canvas color");
		addText(surface, 25, 431, 100, 20, BCS, font, "Be carefull! You will fill");
		addText(surface, 25, 461, 100, 20, BCS, font, "the whole canvas.");
	}

	TTF_CloseFont(font);
	/*source based on user kdyz ends here */
	//
	//end of draw menu
	//

	/*source from Eike Anderson: ~ https://brightspace.bournemouth.ac.uk/content/enforced/67405-TBC/pointy.c?_&d2lSessionVal=fA1HXzp39pGB8EcvpxIfRs0sY&ou=67405 */

	if (!texture)
	{
		fprintf(stderr, "Couldn't create texture: %s\n", SDL_GetError());
		fflush(stderr);
		//continue;
	}

	/*Show the window */

	while (go)
	{
		brushMoved = 0;	// sets the movement of the brush to 0 for every loop
		buttonClicked = 0;	// at the beggining of the loop no button is clicked
		/*Start by clearing what was drawn before */
		/*Set the colour for drawing */
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		/*Clear the entire screen to our selected colour */
		SDL_RenderClear(renderer);
		/*Delays the rendering in order to avoid rendering the texture constantly, because rendering the texture uses to muuch of the CPU */
		SDL_Delay(10);

		/*Here we are going to check for any input events.
		   Basically when you press the keyboard or move the mouse, the parameters are stored as something called an 'event' or 'message'.
		   SDL has a queue of events. We need to check for each event and then do something about it (called 'event handling').
		   The SDL_Event is the data type for the event. */
		SDL_Event incomingEvent;

		/*Here we are drawing the color palette over a bigger rectangle colored like the background
		  It is needed because when the mouse moves on the surface of the color palette 
		  the selected is drawn on top of it in a and we have to get rid of it every time
		  it is rendered.
		  The color box follows the mouse and it gets outside the area of the color palette so we 
		  need to draw first a rectangle to cover a bigger area and then we put the color palette on top of it
		*/
		FRectangle(surface, 107, 261, 205, 205, 0, myW.winWidth, 0, myW.winHeight, myW.Color);
		colourPalette(surface, 20, 190);

		/*SDL_PollEvent will check if there is an event in the queue - this is the program's 'message pump'.
		   If there is nothing in the queue it will not sit and wait around for an event to come along (there are functions which do this,
		   and that can be useful too!). Instead for an empty queue it will simply return 'false' (0).
		   If there is an event, the function will return 'true' (!=0) and it will fill the 'incomingEvent' we have given it as a parameter with the event data */
		while (SDL_PollEvent(&incomingEvent))
		{ /*If we get in here, we have an event and need to figure out what to do with it.
			   For now, we will just use a switch based on the event's type */
			updateTexture = 1;
			if (textBox.isSelected == true)	// if the textBox is selected we check if the event is a text input and if it is we save it in a char variable            
			{ 	/*source from SDL Wiki: ~ https://wiki.libsdl.org/Tutorials/TextInput*/
				SDL_StartTextInput();
				fflush(stdout);

				switch (incomingEvent.type)	// check the type of the event that was captured
				{
					case SDL_QUIT:
						go = 0;	// exiting the while loop
						break;
					case SDL_TEXTINPUT:
						strcat(inputText, incomingEvent.text.text);	// concatenate the last text input to the text char array                                                          
						break;
				}
				/*source from SDL Wiki ends here */
			}
			// if the textbox is not selected we can start checking for the other commands
			switch (incomingEvent.type)
			{
				case SDL_QUIT:
					/*The event type is SDL_QUIT.
					   This means we have been asked to quit - probably the user clicked on the 'x' at the top right corner of the window.
					   To quit we need to set our 'go' variable to false (0) so that we can escape out of the main loop. */
					go = 0;
					break;
					/*source from Eike Anderson ends here*/
				case SDL_MOUSEMOTION:	// in this case the mouse canges its position
					{
						mouseX = incomingEvent.motion.x;	// save the mouse coordinate x 
						mouseY = incomingEvent.motion.y;	// save the mouse coordinate y
					}
					break;
				case SDL_KEYDOWN:	// in this case a key is pressed
					if (textBox.isSelected)	// if the textbox is selected we ignore everything else because it means that we have to get the text only
						break;
					switch (incomingEvent.key.keysym.sym)
					{

						case SDLK_LEFT:	// left key was preseed 
							{ 
								if (myBrush.x > myCanvas.x - myCanvas.width / 2)
								{
									brushMoved = 1;	// we save the fact that the brush was moved
									myBrush.x--;	// we move the brush to the left									
								}
							}
							break;
						case SDLK_RIGHT:	// right key was preseed 
							{ 
								if (myBrush.x < myCanvas.x + myCanvas.width / 2)
								{
									brushMoved = 1;	// we save the fact that the brush was moved
									myBrush.x++;	// we move the brush to the right									
								}
							}
							break;
						case SDLK_a:	// a key was preseed 
							{ 
								if (myBrush.y > 0)
								{
									brushMoved = 1;	// we save the fact that the brush was moved
									myBrush.y--;	// we move the brush up									
								}
							}
							break;
						case SDLK_d:	// d key was preseed 
							{ 
								if (myBrush.y < myCanvas.height)
								{
									brushMoved = 1;	// we save the fact that the brush was moved
									myBrush.y++;	// we move the brush down
								}
							}
							break;
					}
					break;
				case SDL_KEYUP:	// in this case a key was released
					switch (incomingEvent.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							go = 0;	// exiting the while loop
							break;
					}
					break;
				case SDL_MOUSEBUTTONDOWN:	// in this case a mouse button was clicked					
					switch (incomingEvent.button.button)
					{
						case SDL_BUTTON_LEFT:	// in this case a mouse button was clicked
							buttonClicked = 1;	// we track the fact that the left mouse button was clicked
							{
								mouseX = incomingEvent.motion.x;	// we capture the x coordinate of the mouse
								mouseY = incomingEvent.motion.y;	// we capture the y coordinate of the mouse
								// Do something if mouse clicked
								textBox.isSelected = false;	// we unselect the textbox, because another button might have been clicked
								/*We check if the coordinate of the mouse is inside any of the buttons.
								   The variable buttonClicked is changed into 1 when any of the buttons with text was clicked because we would
								   have to readraw the text (labels) of the buttons in this case
								   The buttons are redrawn if their "isSelected" was changed depending on the case
								*/
								int click = checkButton(mouseX, mouseY, &newBtn, &saveBtn, &openBtn, &textBox, &deleteTextBtn, &squareBtn, &circleBtn, &sizeSmallerBtn, &sizeBiggerBtn, &brushCol, &color1Btn, &color2Btn, &mix1btn, &mix2btn, &t1btn, &t2btn, &t3btn, &BGCol);
								if (click >= 1 && click <= 17 && click != 6 && click != 7 && click != 10 && click != 11 && click != 12)
									buttonClicked = 1;
								switch (click)
								{ 						/*
									  First type of buttons
									  Cases 1, 2, 3, 5, 8, 9 behave similarly. When the buttons are clicked we always state them as selected and
									  we redraw them. Later at the mouse button up event we will stste them as unselected. This transition cobined with the colors used to initialise thr buttons
									  will make the button be colored darker when pressed and lighter when released
									*/
									/*									
									  Buttons squareBtn and circleBtn (cases 6 and 7) are a set of radio buttons.
									  When one is clicked it is stated as selected and the other one is stated as unselected (isSelected = false).
									  Both of them are redrawn. One of them will always be selected and colored darker, but never both at the same time 
									  because squareBtn was preselected (isSelected = true) to 
									*/
									/*
									  Buttons t1btn, t2btn and t3btn complete a set similar to radio buttons, but none of them was preselected.
									  When preseing a button its state changes every time and it is redrawn. The other 2 are stated as not selected and 
									  they are redrawnonly when the clicked button becomes true to assure that only one is selected.

									  Cases 10, 11, 12 and 18 act the same between them, but they 4. Button 10 is preselected.
									*/
                                    /*
                                      Cases 13 and 14 are to the prevous one
                                    */
									case 1:
										{
                                            // this is the button for making a new artwork/project/drawing										
											newBtn.isSelected = true;
											drawButton(surface, &newBtn, &myW);
										}
										break;
									case 2:
										{
                                            // this is the button for saving the artwork/project/drawing
											saveBtn.isSelected = true;
											drawButton(surface, &saveBtn, &myW);
										}
										break;
									case 3:
										{
                                            // this is the button for opening an artwork/project/drawing										
											openBtn.isSelected = true;
											drawButton(surface, &openBtn, &myW);
										}
										break;
									case 4:	// in this case the button changes only its state and it is not redrawn
										{
                                            // the textbox is just a "recycled" button
											// we "write" on top of it
											if (textBox.isSelected)
											{
												textBox.isSelected = false;
											}
											else
											{
												textBox.isSelected = true;
											}
										}
										break;
									case 5:
										{
                                            // this is the button for "deleting" the text in the textbox
											if (!deleteTextBtn.isSelected)
											{
												deleteTextBtn.isSelected = true;
											}

											drawButton(surface, &deleteTextBtn, &myW);
											drawButton(surface, &textBox, &myW);	// we redraw the textbox because we want to hide the previous text
										}
										break;
									case 6:
										{
                                            // this is the button for the filled square type of brush
											if (!squareBtn.isSelected)
											{
												squareBtn.isSelected = true;
												myBrush.brushType = 1;
												circleBtn.isSelected = false;
											}
											drawButton(surface, &squareBtn, &myW);
											drawButton(surface, &circleBtn, &myW);
											// We also redraw the image of a square and the one of a circle on the button
											FRectangle(surface, squareBtn.x, squareBtn.y, squareBtn.width - 5, squareBtn.height - 5, squareBtn.x - squareBtn.width / 2, squareBtn.x + squareBtn.width / 2, squareBtn.y - squareBtn.height / 2, squareBtn.y + squareBtn.height / 2, squareBtn.BorderColorSelected);
											Icircle(surface, circleBtn.x, circleBtn.y, 7, circleBtn.x - circleBtn.width / 2, circleBtn.x + circleBtn.width / 2, circleBtn.y - circleBtn.height / 2, circleBtn.y + circleBtn.height / 2, circleBtn.BorderColorSelected);
										}
										break;
									case 7:
										{
                                            // this is the button for the empty circle type of brush
											if (!circleBtn.isSelected)
											{
												circleBtn.isSelected = true;
												myBrush.brushType = 2;
												squareBtn.isSelected = false;
											}
											drawButton(surface, &circleBtn, &myW);
											drawButton(surface, &squareBtn, &myW);
											FRectangle(surface, squareBtn.x, squareBtn.y, squareBtn.width - 5, squareBtn.height - 5, squareBtn.x - squareBtn.width / 2, squareBtn.x + squareBtn.width / 2, squareBtn.y - squareBtn.height / 2, squareBtn.y + squareBtn.height / 2, squareBtn.BorderColorSelected);
											Icircle(surface, circleBtn.x, circleBtn.y, 7, circleBtn.x - circleBtn.width / 2, circleBtn.x + circleBtn.width / 2, circleBtn.y - circleBtn.height / 2, circleBtn.y + circleBtn.height / 2, circleBtn.BorderColorSelected);
										}
										break;
									case 8:
										{
                                            // this is the button for decreasing the size of the brush
											if (!sizeSmallerBtn.isSelected)
												sizeSmallerBtn.isSelected = true;
											drawButton(surface, &sizeSmallerBtn, &myW);
										}
										break;
									case 9:
										{
                                            // this is the button for increasing the size of the brush
											if (!sizeBiggerBtn.isSelected)
												sizeBiggerBtn.isSelected = true;
											drawButton(surface, &sizeBiggerBtn, &myW);
										}
										break;
									case 10:
										{
                                            // this button is used to show the brush color
											if (brushCol.isSelected)
												brushCol.isSelected = false;
											else
											{
												brushCol.isSelected = true;
												color2Btn.isSelected = false;
												color1Btn.isSelected = false;
												BGCol.isSelected = false;
												drawButton(surface, &color1Btn, &myW);
												drawButton(surface, &BGCol, &myW);
												drawButton(surface, &color2Btn, &myW);
											}
											drawButton(surface, &brushCol, &myW);
										}
										break;
									case 11:
										{
                                            // this button is used to pick a color from the palette 
											// its color will be used for the mixing process as the blend1 color
											if (color1Btn.isSelected)
												color1Btn.isSelected = false;
											else
											{
												color1Btn.isSelected = true;
												color2Btn.isSelected = false;
												BGCol.isSelected = false;
												brushCol.isSelected = false;
												drawButton(surface, &color2Btn, &myW);
												drawButton(surface, &BGCol, &myW);
												drawButton(surface, &brushCol, &myW);
											}
											color1Btn.isSelected = true;
											drawButton(surface, &color1Btn, &myW);
										}
										break;
									case 12:
										{
                                            // this button is used to pick a color from the palette 
											// its color will be used for the mixing process as the blend2 color
											if (color2Btn.isSelected)
												color2Btn.isSelected = false;
											else
											{
												color2Btn.isSelected = true;
												color1Btn.isSelected = false;
												BGCol.isSelected = false;
												brushCol.isSelected = false;
												drawButton(surface, &color1Btn, &myW);
												drawButton(surface, &BGCol, &myW);
												drawButton(surface, &brushCol, &myW);
											}
											drawButton(surface, &color2Btn, &myW);
										}
										break;
									case 13:
										{
                                            // this is the button for the first type of mixing
											if (mix1btn.isSelected)
												mix1btn.isSelected = false;
											else
											{
												mix1btn.isSelected = true;
												mix2btn.isSelected = false;
												drawButton(surface, &mix2btn, &myW);
												blend1 = color1Btn.ColorSelected;	// resets the first blending color because it was changed during the process of mixing
												blend2 = color2Btn.ColorSelected;	// resets the second blending color because it was changed during the process of mixing
												initMix = 0;	// resets the mixing process
												iMix = 0;	// resets the mixing id
											}
											drawButton(surface, &mix1btn, &myW);
										}
										break;
									case 14:
										{
                                            // this is the button for the second type of mixing
											if (mix2btn.isSelected)
												mix2btn.isSelected = false;
											else
											{
												mix2btn.isSelected = true;
												mix1btn.isSelected = false;
												drawButton(surface, &mix1btn, &myW);
												blend1 = color1Btn.ColorSelected;	// resets the first blending color because it was changed during the process of mixing
												blend2 = color2Btn.ColorSelected;	// resets the second blending color because it was changed during the process of mixing
												initMix = 0;	// resets the mixing process
												iMix = 0;	// resets the mixing id
											}
											drawButton(surface, &mix2btn, &myW);
										}
										break;
									case 15:
										{
                                            // this is the button for the first "twin"
											if (t1btn.isSelected)
												t1btn.isSelected = false;
											else
											{
												t1btn.isSelected = true;
												t2btn.isSelected = false;
												t3btn.isSelected = false;
												drawButton(surface, &t2btn, &myW);
												drawButton(surface, &t3btn, &myW);
											}
											drawButton(surface, &t1btn, &myW);
										}
										break;
									case 16:
										{
                                            // this is the button for the second "twin"
											if (t2btn.isSelected)
												t2btn.isSelected = false;
											else
											{
												t2btn.isSelected = true;
												t1btn.isSelected = false;
												t3btn.isSelected = false;
												drawButton(surface, &t1btn, &myW);
												drawButton(surface, &t3btn, &myW);
											}
											drawButton(surface, &t2btn, &myW);
										}
										break;
									case 17:
										{
                                            // this is the button for the third "twin"
											if (t3btn.isSelected)
												t3btn.isSelected = false;
											else
											{
												t3btn.isSelected = true;
												t1btn.isSelected = false;
												t2btn.isSelected = false;
												drawButton(surface, &t1btn, &myW);
												drawButton(surface, &t2btn, &myW);
											}
											drawButton(surface, &t3btn, &myW);
										}
										break;
									case 18:
										{
                                            // this is the button for the background color
											if (BGCol.isSelected)
												BGCol.isSelected = false;
											else
											{
												BGCol.isSelected = true;
												color1Btn.isSelected = false;
												color2Btn.isSelected = false;
												drawButton(surface, &color1Btn, &myW);
												drawButton(surface, &color2Btn, &myW);
											}
											drawButton(surface, &BGCol, &myW);
										}
										break;
									case -1:
										{ 								
                                            /*
											   In this case it means that the mouse clicked occured over the area of the palette.
											   We check the buttons for colors to see if any of them is selected and to change their color
											   to the one of the pixel we clicked on and then redraw them
											*/

											RGBcolour tempCol = getIPixelColour(surface, mouseX, mouseY);	//Checks the color of the pixel we clicked on and returns it to tempCol
											if (color1Btn.isSelected)
											{
												blend1 = tempCol;
												// we set the button's color to the one that was selected and we redraw it
												color1Btn.ColorSelected = tempCol;
												color1Btn.ColorUnselected = tempCol;
												drawButton(surface, &color1Btn, &myW);
												initMix = 0;	// resets the mixing process
												iMix = 0;	// resets the mixing id
											}
											if (color2Btn.isSelected)
											{
												blend2 = tempCol;
												// we set the button's color to the one that was selected and we redraw it
												color2Btn.ColorSelected = tempCol;
												color2Btn.ColorUnselected = tempCol;
												drawButton(surface, &color2Btn, &myW);
												initMix = 0;	// resets the mixing process
												iMix = 0;	// resets the mixing id
											}
											if (BGCol.isSelected)
											{
                                                // we set the button's color to the one that was selected and we redraw it                                          
												BGCol.ColorSelected = tempCol;
												BGCol.ColorUnselected = tempCol;
												drawButton(surface, &BGCol, &myW);
												// we change the menu background color to the one we selected
												FRectangle(surface, myCanvas.x, myCanvas.y, myCanvas.width, myCanvas.height, myCanvas.x - myCanvas.width / 2, myCanvas.x + myCanvas.width / 2, myCanvas.y - myCanvas.height / 2, myCanvas.y + myCanvas.height / 2, tempCol);
											}
											if (brushCol.isSelected)
											{
                                                // we set the button's color to the one that was selected and we redraw it                                          
												brushCol.ColorSelected = tempCol;
												brushCol.ColorUnselected = tempCol;
												drawButton(surface, &brushCol, &myW);
												// we change the color of the brudh
												myBrush.Color = tempCol;
											}
										}
										break;
								}
							}
							break;
					}
					break;
				case SDL_MOUSEBUTTONUP:
					{
                        // we save the coordinates of the mouse when it was released
						mouseX = incomingEvent.motion.x;
						mouseY = incomingEvent.motion.y;

						int click = checkButton(mouseX, mouseY, &newBtn, &saveBtn, &openBtn, &textBox, &deleteTextBtn, &squareBtn, &circleBtn, &sizeSmallerBtn, &sizeBiggerBtn, &brushCol, &color1Btn, &color2Btn, &mix1btn, &mix2btn, &t1btn, &t2btn, &t3btn, &BGCol);
						if (click >= 1 && click <= 17 && click != 6 && click != 7 && click != 10 && click != 11 && click != 12)
							buttonClicked = 1;
						/*
						  We check if any the buttons of the first type (see abobe in the SDL_MOUSEBUTTONDOWN event) was selected reviousely.
						  If yes, we change their state and redraw them
						*/
						if (deleteTextBtn.isSelected)
						{
							deleteTextBtn.isSelected = false;
							drawButton(surface, &textBox, &myW);
							drawButton(surface, &deleteTextBtn, &myW);	// redraw the textbox to look like the text was deleted
							strcpy(inputText, "");	// make the inputText char array be empty
						}
						if (newBtn.isSelected)
						{
							newBtn.isSelected = false;
							drawButton(surface, &newBtn, &myW);
							// we redraw the canvas/drawing area and initialise the brush like at the beginning of the program
							FRectangle(surface, myCanvas.x, myCanvas.y, myCanvas.width, myCanvas.height, myCanvas.x - myCanvas.width / 2, myCanvas.x + myCanvas.width / 2, myCanvas.y - myCanvas.height / 2, myCanvas.y + myCanvas.height / 2, myCanvas.Color);
							initialiseColor(&myColor, 0, 0, 0);
							initializeBrush(&myBrush, 300, 0, 2, 4, 4, myColor, 1);
                            initialiseColor(&myColor, 255, 255, 255);
                            BGCol.ColorSelected = myColor;
                            BGCol.ColorUnselected = myColor;
                            drawButton(surface, &BGCol, &myW);
                            // reset blending
                            initMix = 0;
                            blend1 = color1Btn.BorderColorSelected;
                            blend2 = color2Btn.BorderColorUnselected;
						}
						if (saveBtn.isSelected)
						{
							saveBtn.isSelected = false;
							drawButton(surface, &saveBtn, &myW);
							// we declare and initialise the rectangles that will be used to blit the surface that has the drawing
							SDL_Rect rect, destRect;

							rect.x = myCanvas.x - myCanvas.width / 2;
							rect.y = myCanvas.y - myCanvas.height / 2;
							rect.w = myCanvas.width;
							rect.h = myCanvas.height;

							destRect.x = 0;
							destRect.y = 0;
							destRect.w = myCanvas.width;
							destRect.h = myCanvas.height;

							SDL_Surface *saveImg = SDL_CreateRGBSurface(0, rect.w, rect.h, 32, 0, 0, 0, 0);	// temporary surface used as destination of the blited area

							SDL_BlitSurface(surface, &rect, saveImg, &destRect);
							if (strlen(inputText) == 0)	// check for name to not be empty
								SDL_ShowSimpleMessageBox(0, ":(", "Please give the artwork a name", window);
							else
							{
								SDL_Surface *l_loadedSurface = SDL_CreateRGBSurface(0, myCanvas.width, myCanvas.height, 32, 0, 0, 0, 0);
								// we check if there are already other files with the same name
								l_loadedSurface = IMG_Load(inputText);
								if (l_loadedSurface == NULL)
								{
									int error = SDL_SaveBMP(saveImg, inputText);	// checks for errors at saving
									if (error == 0)
									{
										FILE *f = fopen("out.txt", "a");	// we open the text file that we use for saving the data that we need										
										// we append the name of the file and thebrush details to the text file
										fprintf(f, "%s %d %d %d %d %d %d %d %d %d\n", inputText, myBrush.x, myBrush.y, myBrush.radius, myBrush.width, myBrush.height, myBrush.Color.r, myBrush.Color.g, myBrush.Color.b, myBrush.brushType);
										fclose(f);	// we close the text file
										SDL_ShowSimpleMessageBox(0, ":D", "Your artwork was saved!!!", window);
									}
									else
									{
										SDL_ShowSimpleMessageBox(0, ":(", "Could not save your artwork...\nPlease try another name", window);
									}
								}
								else
								{
									SDL_ShowSimpleMessageBox(0, ":(", "There is already another artwork with this name", window);
								}
							}
							SDL_FreeSurface(saveImg);
						}

						if (openBtn.isSelected)
						{
							openBtn.isSelected = false;
							drawButton(surface, &openBtn, &myW);
							if (strlen(inputText) == 0)
							{
								SDL_ShowSimpleMessageBox(0, ":(", "Please write the name of the artwork you want to display", window);
								break;
							}
							char str[500];
							int findF = 0;
							FILE *f = fopen("out.txt", "r");
							if (f == NULL)	// check for name to not be empty
							{
								SDL_ShowSimpleMessageBox(0, ":(", "Could not open file", window);
								return 1;
							}
							else
							{
                                // we read every line/entry in the file to see and we check to see if any of the file names written in it are the same as the one in the textbox
								// we stop when the file we are looking for is found (findf)
								while (fgets(str, 1000, f) != NULL && findF == 0)
								{
									findF = findFile(str, inputText, &myBrush);
								}
								fclose(f);
							}

							if (findF == 0)	// we send a message if the file we are looking for was not found
							{
								SDL_ShowSimpleMessageBox(0, ":(", "You have not named any of your artworks like this", window);
							}
							else
							{
                                // if we found the file we will display it on the drawing area using the blitting method described above
								SDL_Surface *l_loadedSurface = SDL_CreateRGBSurface(0, myCanvas.width, myCanvas.height, 32, 0, 0, 0, 0);
								l_loadedSurface = IMG_Load(inputText);
								if (l_loadedSurface == NULL)	// checks if the surface can be loaded
								{
									printf("Could not load image");
									fflush(stdout);
									SDL_ShowSimpleMessageBox(0, ":(", "Could not load image", window);
								}
								else
								{
									SDL_Rect rect, destRect;
									rect.x = myCanvas.x - myCanvas.width / 2;
									rect.y = myCanvas.y - myCanvas.height / 2;
									rect.w = myCanvas.width;
									rect.h = myCanvas.height;

									destRect.x = 0;
									destRect.y = 0;
									destRect.w = myCanvas.width;
									destRect.h = myCanvas.height;
									SDL_BlitSurface(l_loadedSurface, NULL, surface, &rect);
                                    // reset blending
                                    mix1btn.isSelected = false;
                                    mix2btn.isSelected = false;
                                    drawButton(surface, &mix1btn, &myW);
                                    drawButton(surface, &mix2btn, &myW);
                                    initMix = 0;
                                    // updates brush colour button
                                    blend1 = color1Btn.BorderColorSelected;
                                    blend2 = color2Btn.BorderColorUnselected;
                                    brushCol.ColorSelected = myBrush.Color;
                                    brushCol.ColorUnselected = myBrush.Color;
                                    drawButton(surface, &brushCol, &myW);
									break;                 
								}
							}
						}

						if (sizeSmallerBtn.isSelected)
						{
							sizeSmallerBtn.isSelected = false;
							drawButton(surface, &sizeSmallerBtn, &myW);
							// we decrease the size of the brush
							if (myBrush.width > 2)
							{
								myBrush.radius--;
								myBrush.width--;
								myBrush.height--;
							}
						}

						if (sizeBiggerBtn.isSelected)
						{
							sizeBiggerBtn.isSelected = false;
							drawButton(surface, &sizeBiggerBtn, &myW);
							// we decrease the size of the brush
							if (myBrush.radius < 100)
							{
								myBrush.radius++;
								myBrush.width++;
								myBrush.height++;
							}
						}
					}
					break;
			}
		}

		if ((mix1btn.isSelected || mix2btn.isSelected) && brushMoved)
		{
			if (initMix == 0)	// if the mixing has not started yet we reset the colors for blend1, blend2 and brush
			{
                blend1 = color1Btn.ColorSelected;
				blend2 = color2Btn.ColorSelected;
				myBrush.Color = blend1;
				initMix = 1;				
				iMix++;	// increase the mix id             
			}
			else
			{
				iMix++;	// increase the mix id        
				if (mix1btn.isSelected)
					myBrush.Color = mixColors1(blend1, blend2, myBrush.Color);
				else
				{
					myBrush.Color = mixColors2(blend1, blend2, myBrush.Color, iMix);
				}
			}

			if (iMix == 100)
			{
			 	// we interchange the blend1 and blend2 colors 
				RGBcolour tcol;
				iMix = 0;
				tcol = blend2;
				blend2 = blend1;
				blend1 = tcol;

				if (mix1btn.isSelected)
					myBrush.Color = mixColors1(blend1, blend2, myBrush.Color);
				else
					myBrush.Color = mixColors2(blend1, blend2, myBrush.Color, iMix);
			}
		}

		// We draw the brush and its "twin" (if needed) according to current the brush characteristics
		// twin 1 is the brush mirrored by the vertical axis
		// twin 2 is the brush mirrored by the horizontal axis
		// twin 3 is the brush mirrored by both axis

		if (myBrush.brushType == 1)	// square brush
		{
			FRectangle(surface, myBrush.x, myBrush.y, myBrush.width, myBrush.height, myCanvas.x - myCanvas.width / 2, myCanvas.x + myCanvas.width / 2, myCanvas.y - myCanvas.height / 2, myCanvas.y + myCanvas.height / 2, myBrush.Color);

			if (t1btn.isSelected)	// twin 1 
			{ 
				FRectangle(surface, myW.winWidth + 300 - myBrush.x, myBrush.y, myBrush.width, myBrush.height, myCanvas.x - myCanvas.width / 2, myCanvas.x + myCanvas.width / 2, myCanvas.y - myCanvas.height / 2, myCanvas.y + myCanvas.height / 2, myBrush.Color);
			}
			if (t2btn.isSelected)	// twin 2
			{
				FRectangle(surface, myBrush.x, myW.winHeight - myBrush.y, myBrush.width, myBrush.height, myCanvas.x - myCanvas.width / 2, myCanvas.x + myCanvas.width / 2, myCanvas.y - myCanvas.height / 2, myCanvas.y + myCanvas.height / 2, myBrush.Color);
			}
			if (t3btn.isSelected)	// twin 3
			{
				FRectangle(surface, myW.winWidth + 300 - myBrush.x, myW.winHeight - myBrush.y, myBrush.width, myBrush.height, myCanvas.x - myCanvas.width / 2, myCanvas.x + myCanvas.width / 2, myCanvas.y - myCanvas.height / 2, myCanvas.y + myCanvas.height / 2, myBrush.Color);
			}
		}

		if (myBrush.brushType == 2)	// circle brush
		{
			Icircle(surface, myBrush.x, myBrush.y, myBrush.radius, myCanvas.x - myCanvas.width / 2, myCanvas.x + myCanvas.width / 2, myCanvas.y - myCanvas.height / 2, myCanvas.y + myCanvas.height / 2, myBrush.Color);

			if (t1btn.isSelected)	// twin 1 
			{ 
				Icircle(surface, myW.winWidth + 300 - myBrush.x, myBrush.y, myBrush.radius, myCanvas.x - myCanvas.width / 2, myCanvas.x + myCanvas.width / 2, myCanvas.y - myCanvas.height / 2, myCanvas.y + myCanvas.height / 2, myBrush.Color);
			}
			if (t2btn.isSelected)	// twin 2
			{
				Icircle(surface, myBrush.x, myW.winHeight - myBrush.y, myBrush.radius, myCanvas.x - myCanvas.width / 2, myCanvas.x + myCanvas.width / 2, myCanvas.y - myCanvas.height / 2, myCanvas.y + myCanvas.height / 2, myBrush.Color);
			}
			if (t3btn.isSelected)	// twin 3
			{
				Icircle(surface, myW.winWidth + 300 - myBrush.x, myW.winHeight - myBrush.y, myBrush.radius, myCanvas.x - myCanvas.width / 2, myCanvas.x + myCanvas.width / 2, myCanvas.y - myCanvas.height / 2, myCanvas.y + myCanvas.height / 2, myBrush.Color);
			}
		}

		// if the mouse coordinates are in the area of the palette we get the color of the pixel and we draw a small box next to the cursor
		// the box has black outline and is filled with the color from that pixel
		if (mouseX >= 20 && mouseX <= 187 && mouseY >= 190 && mouseY <= 357)
		{
			RGBcolour tempCol = getIPixelColour(surface, mouseX, mouseY);
			FRectangle(surface, mouseX + 11, mouseY - 11, 20, 20, 0, myW.winWidth, 0, myW.winHeight, tempCol);
			tempCol.r = 0;
			tempCol.g = 0;
			tempCol.b = 0;
			Irectagle(surface, mouseX + 11, mouseY - 11, 20, 20, tempCol);
		}

		// we rewrite the text on the surface if any of the buttons with labels was clicked or if the textbox is selected
		// when we write in the textbox we have to update the text every time
		if (buttonClicked || textBox.isSelected)
		{ /*source based on user kdyz : ~  https://stackoverflow.com/questions/22886500/how-to-render-text-in-sdl2 */
			TTF_Init();
			TTF_Font *font = TTF_OpenFont("/usr/share/fonts/sil-padauk/Padauk.ttf", 20);
			if (!font)
			{
				printf("TTF_OpenFont: %s\n", TTF_GetError());
			}
			else
			{
				if (buttonClicked)
				{
					addText(surface, 25, 10, 70, 20, BCS, font, "New file");
					addText(surface, 115, 10, 70, 20, BCS, font, "Save file");
					addText(surface, 205, 10, 70, 20, BCS, font, "Open file");
					addText(surface, 230, 256, 70, 20, BCS, font, "Mix 1");
					addText(surface, 230, 297, 70, 20, BCS, font, "Mix 2");
					addText(surface, 145, 135, 70, 20, BCS, font, "-");
					addText(surface, 176, 136, 70, 20, BCS, font, "+");
					addText(surface, 269, 62, 20, 20, BCS, font, "x");
					addText(surface, 85, 370, 100, 20, BCS, font, "1");
					addText(surface, 110, 370, 100, 20, BCS, font, "2");
					addText(surface, 135, 370, 100, 20, BCS, font, "3");
				}
				if (textBox.isSelected)
				{
					addText(surface, 25, 62, 70, 20, BCS, font, inputText);
				}
			}
			TTF_CloseFont(font);
			/*source based on user kdyz ends here */
		}
		/*source based on Eike Anderson:~ https://brightspace.bournemouth.ac.uk/content/enforced/67405-TBC/pointy.c?_&d2lSessionVal=fA1HXzp39pGB8EcvpxIfRs0sY&ou=67405*/
		/*Draw our graphics */

		/*Start by clearing what was drawn before */
		/*Set the colour for drawing */
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		/*Clear the entire screen to our selected colour */
		SDL_RenderClear(renderer);

		if (updateTexture)
		{
			texture = SDL_CreateTextureFromSurface(renderer, surface);
			updateTexture = 0;
		}
		/*This tells the renderer to actually show its contents to the screen. */
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	/*If we get outside the main loop, it means our user has requested we exit. */

	/*Our cleanup phase */

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
	/*source from Eike Anderson ends here */

	return 0;
}