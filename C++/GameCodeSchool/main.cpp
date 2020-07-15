#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <math.h>
#include <sstream>
#define PI 3.14159265

using namespace sf;

int main()
{
	// Create a video mode object
	VideoMode vm(1920, 1080);

	// Create and open a window for the game
	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);

	// Make the Background
    // Create a texture to hold a graphic on the GPU
	Texture textureBackground;
	// Load a graphic into the texture
	textureBackground.loadFromFile("graphics/background.png");
    // Create a sprite
	Sprite spriteBackground;
	// Attach the texture to the sprite
	spriteBackground.setTexture(textureBackground);
	// Set the spriteBackground to cover the screen
	spriteBackground.setPosition(0, 0);

    // Make the Tree Sprite
    Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");
    Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(810,0);

    // Prepare the bee
    Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");
    Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0,800);

    // Check if the bee is moving
    bool beeActive = false;

    // Define how fast the bee can and will fly
    float beeSpeed = 0.0f;

    // Prepare and Make 1 cloud texture and 3 sprites
    // First "Texture" and "Sprite" are classes that create 
    // the objects spriteCloud1,2,3 and textureCloud
    // loading textures takes up system resources
    Texture textureCloud; 
    textureCloud.loadFromFile("graphics/cloud.png");
    Sprite spriteCloud1;
    Sprite spriteCloud2;
    Sprite spriteCloud3;
    spriteCloud1.setTexture(textureCloud);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud3.setTexture(textureCloud);

    // Set the positions of each of the clouds the cloud
    spriteCloud1.setPosition(0,0);
    spriteCloud2.setPosition(0,150);
    spriteCloud3.setPosition(0,300);

    // Check if cloud is on screen
    bool cloud1Active = false;
    bool cloud2Active = false;
    bool cloud3Active = false;

    // Set how fast clouds move in number of pixels

    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;

    // Variables to control internal clock
    Clock clock;

    // Visual Time Bar
    RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    //Vector2f is an obejct with 2 values
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(Color::Red);
    timeBar.setPosition((1920/2) - timeBarStartWidth / 2, 980);

    Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;


    // Track if user wants game to be paused
    bool paused = true;

    // Adding Text /  HUD
    int score = 0;
    sf::Text messageText;
    sf::Text scoreText;

    //Selecting a font from SMFL libraries create an object called font from class Font
    Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");
    
    //Set the font type to the 2 types of messages we are going to display
    messageText.setFont(font);
    scoreText.setFont(font);

    //Create our message
    messageText.setString("Press Enter To Start!");
    scoreText.setString("Score = 0");

    //Adjust font Size
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);

    //Set font color
    messageText.setFillColor(Color::White);
    scoreText.setFillColor(Color::White);

    //Position text in window
    //create an obect: textRect(angle) and fit the message in that local boundry
    //define the top, left, and rt margins,
    // set the score text to a fixed location.
    FloatRect textRect = messageText.getLocalBounds();

    messageText.setOrigin(textRect.left+textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

    scoreText.setPosition(20,20);


	while (window.isOpen())
	{
		
		/*
		****************************************
		Handle the players input
		****************************************
		*/

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
        
        //This code start/stops the game when the "Enter" key is pressed
        if (Keyboard::isKeyPressed(Keyboard::Return))
        {
            paused = false;

            //reset the time and score after enter key is pressed
            score = 0;
            timeRemaining = 5;
        }

		/*
		****************************************
		Update the scene
		****************************************
		*/
        //if not paused, or if pause = false
        if (!paused)
        {
            // Keep track (measure) time by restarting clock every frame
            Time dt = clock.restart();

            //Change the remaining time
            timeRemaining = timeRemaining - dt.asSeconds();
            //change the time bar size every second
            timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

            if (timeRemaining <= 0.0f)
            {
                //Pause the game
                paused = true;

                //Change the onscreen text
                messageText.setString("Game Over, You're out of Time!");

                //Reposition the text based on it's new size
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

            }

            // Setup bee starting location on screen
            if (!beeActive)
            {
                // Define random bee speed
                srand((int)time(0) );
                beeSpeed = (rand() % 200) + 200;
                // define random starting height of the bee
                srand((int)time(0) * 10);
                float height = (rand() % 1350) + 500;
                spriteBee.setPosition(2000, height);
                beeActive = true;
            }
            else
            // Move the bee
            {
                spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);
                // Has the bee reached the right hand edge of the screen?
                if (spriteBee.getPosition().x < -100)
                {
                    // Set it up ready to be a whole new cloud next frame
                    beeActive = false;
                }
            }

            if (!cloud1Active)
            {
                // How fast is the cloud
                srand((int)time(0) * 10);
                cloud1Speed = (rand() % 200);
                // How high is the cloud
                srand((int)time(0) * 10);
                float height = (rand() % 150);
                spriteCloud1.setPosition(-200, height);
                cloud1Active = true;
            }
            else
            {
                spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()), spriteCloud1.getPosition().y);
                // Has the cloud reached the right hand edge of the screen?
                if (spriteCloud1.getPosition().x > 1920)
                {
                    // Set it up ready to be a whole new cloud next frame
                    cloud1Active = false;
                }
            }

            if (!cloud2Active)
            {
                // How fast is the cloud
                srand((int)time(0) * 20);
                cloud2Speed = (rand() % 200);
                // How high is the cloud
                srand((int)time(0) * 20);
                float height = (rand() % 300) - 150;
                spriteCloud2.setPosition(-200, height);
                cloud2Active = true;
            }
            else
            {
                spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y);
                // Has the cloud reached the right hand edge of the screen?
                if (spriteCloud2.getPosition().x > 1920)
                {
                    // Set it up ready to be a whole new cloud next frame
                    cloud2Active = false;
                }
            }

            if (!cloud3Active)
            {
                // How fast is the cloud
                srand((int)time(0) * 30);
                cloud3Speed = (rand() % 200);
                // How high is the cloud
                srand((int)time(0) * 30);
                float height = (rand() % 450) - 150;
                spriteCloud3.setPosition(-200, height);
                cloud3Active = true;
            }
            else
            {
                spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()), spriteCloud3.getPosition().y);
                // Has the cloud reached the right hand edge of the screen?
                if (spriteCloud3.getPosition().x > 1920)
                {
                    // Set it up ready to be a whole new cloud next frame
                    cloud3Active = false;
                }
            } 

            //Upate the score of the game after each loop
            std::stringstream ss;
            ss << "Score = " << score;
            scoreText.setString(ss.str());

        }//End of if(!paused) statement
        

		/*
		****************************************
		Draw the scene
		****************************************
		*/

		// Clear everything from the last frame
		window.clear();

		// Draw our game scene here
		window.draw(spriteBackground);

        // Draw the clouds
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);

        // Draw the tree on the screen
        window.draw(spriteTree);

        // Draw the bee
        window.draw(spriteBee);

        //Draw the time bar after the tree so the bar is before the tree
        window.draw(timeBar);

        // Draw the score
        window.draw(scoreText);
        if (paused)
        {
            // Draw message only when paused
            window.draw(messageText);
        }

		// Show everything we just drew
		window.display();


	}

	return 0;
}