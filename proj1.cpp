//////////////////////////////////////////////////////////// 
// Headers 
//////////////////////////////////////////////////////////// 
#include "stdafx.h" 
#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 
#pragma comment(lib,"opengl32.lib") 
#pragma comment(lib,"glu32.lib") 
   
#include "SFML/Graphics.hpp" 
#include "SFML/OpenGL.hpp" 
#include <iostream> 

#include <math.h>
#include "BouncingTriangle.h"
#include "BouncingBox.h"
#include "BouncingThing.h"
#include "Multiside.h"
#include "CollisionDetection.h"
   
// Name : Mihas Konopelko
// Login : C00157576
// Optimization implemented :
// - Bounding Circles
// - Pair Redundacy
//
// Bugs : 
// SAT is not accurate as it could be, yet it works (comment out bounding circle code to test)





   
//////////////////////////////////////////////////////////// 
/// Entry point of application 
//////////////////////////////////////////////////////////// 
int main() 
{ 
	// Windows Size
	int screenWidth = 800;
	int screenHeight = 800;

	int totalTriangles =10;
	int totalBoxes =10;
	int totalMultisides =5;

	int id_count = 0;				// To set id

				


	std::vector<BouncingThing*> things;

    sf::RenderWindow App(sf::VideoMode(screenWidth, screenHeight, 32), "SFML OpenGL");   

	#pragma region BouncingTriangles
	for(int i = 0; i < totalTriangles; i++)
	{
		things.push_back(new BouncingTriangle(rand() % (screenWidth),rand() % (screenHeight),10+rand() %20,rand() %180,0.05,id_count));
		things.back()->launch(.04f +((float)i/(float)50)  ,rand() %180);
		id_count++;
	}
	#pragma endregion

	#pragma region BouncingBoxes
	for(int i = 0; i < totalBoxes; i++)
	{
		things.push_back(new BouncingBox(rand() % (screenWidth),rand() % (screenHeight),10+rand() %20,rand() %180,0.05,id_count));
		things.back()->launch(.04f +((float)i/(float)50),rand() %180);
		id_count++;
	}
	#pragma endregion

	#pragma region BouncingMultisides
	for(int i = 0; i < totalMultisides; i++)
	{
		things.push_back(new BouncingMultiside(rand() % (screenWidth),rand() % (screenHeight),3+rand()%9, 25+rand() %10,rand() %180, 0.05, id_count));
		things.back()->launch(.04f +((float)i/(float)50),rand() %180);
		id_count++;
	}
	#pragma endregion



	CollisionDetection::instance(screenWidth,screenHeight);


    //prepare OpenGL surface for HSR 
    glClearDepth(1.f); 
    glClearColor(0.0f, 0.0f, 0.0f, 0.f); //background colour
    glEnable(GL_DEPTH_TEST); 
    glDepthMask(GL_TRUE); 
   
    //// Setup a perspective projection & Camera position 
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity(); 
     
    //set up a 3D Perspective View volume
    //gluPerspective(90.f, 1.f, 1.f, 300.0f);//fov, aspect, zNear, zFar 
 
    //set up a  orthographic projection same size as window
    //this mease the vertex coordinates are in pixel space
    glOrtho(0,screenWidth,0,screenHeight,0,1); // use pixel coordinates
   

	// Launch There

   
       
    // Start game loop 
    while (App.isOpen()) 
    { 
        // Process events 
        sf::Event Event; 
        while (App.pollEvent(Event)) 
        { 
            // Close window : exit 
            if (Event.type == sf::Event::Closed) 
                App.close(); 
   
            // Escape key : exit 
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape)) 
                App.close(); 
             
    
        } 
           
        //Prepare for drawing 
        // Clear color and depth buffer 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
   
        // Apply some transformations 
        glMatrixMode(GL_MODELVIEW); 
        glLoadIdentity(); 
         
		  
		std::vector<BouncingThing*>::iterator mainIt = things.begin();
		std::vector<BouncingThing*>::iterator subIt = things.begin();
		
		for (;mainIt!=things.end();++mainIt)
		{
			// Check for collisions is first
			for (;subIt!=things.end();++subIt)
			{
				// Checks so that there is no check on self
				if (subIt!=mainIt)
				{
					// Checks for bounding spheres, pair check and SAT
					CollisionDetection::instance()->CollisionCheck(*subIt,*mainIt);
				}

			}

			subIt=things.begin();

			// Checks Borders
			CollisionDetection::instance()->BorderCollision(*mainIt);

			(*mainIt)->update();
			(*mainIt)->draw();
		}

		mainIt = things.begin();

		// Clears collisison list
		for (;mainIt!=things.end();mainIt++)
			(*mainIt)->ClearCollideList();
		

        // Finally, display rendered frame on screen 
        App.display(); 
    } 
   
    return EXIT_SUCCESS; 
}

