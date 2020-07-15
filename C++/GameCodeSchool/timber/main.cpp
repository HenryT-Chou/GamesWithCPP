#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <math.h>
#include <sstream>
#define PI 3.14159265

using namespace sf;

//Declare a function
void updateBranches(int seed);
//capitalizing the first letter to show that this is a global variable
const int Num_branches = 6;
//creatung an array of objects (6 objects stored in and array called branches)
Sprite branches[Num_branches];

//Where is the player/branch? (L/R)
//scoped enumerations
enum class side { LEFT, RIGHT, NONE };
side branchPositions[Num_branches];

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
    messageText.setString("Press Enter To Start! Use Arrow Keys to Play!");
    scoreText.setString("Score = 0");

    //Adjust font Size
    messageText.setCharacterSize(70);
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

    // Prepare the 6 branches
    Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");

    //Set the texture for each branch (similar thing can be done for clouds)
    for (int i=0; i<Num_branches; i++)
    {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000,-2000);
        //Set the Spirte's origin to center of screen
        //that way we can manipulate it without changeing position again
        branches[i].setOrigin(220,20);
    }

    //Prepare the player sprite
    Texture texturePlayer;
    texturePlayer.loadFromFile("graphics/player.png");
    Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(580,720);

    //Define player starting location
    //this will be used later to see if the player collides with branch
    side playerSide = side::LEFT;

    //Prep the player gravestone
    Texture textureRIP;
    textureRIP.loadFromFile("graphics/rip.png");
    Sprite spriteRIP;
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(600,860);

    //Prep the axe to be used by the player
    Texture textureAxe;
    textureAxe.loadFromFile("graphics/axe.png");
    Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(700,830);

    //line the axe up with the tree, by making it float
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    //prep the graphics for the log to be "chopped off"
    Texture textureLog;
    textureLog.loadFromFile("graphics/log.png");
    Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteAxe.setPosition(810,720);

    //set aside some variables to manipulate the log and make it fly off screen
    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;

    //create variables to control player input
    bool acceptInput = false;

	while (window.isOpen())
	{
		        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::KeyReleased && !paused)
            {
                //start listening for keypresses again
                acceptInput = true;

                //hide the axe
                spriteAxe.setPosition(2000,spriteAxe.getPosition().y);
            }
        }

		/*
		****************************************
		Handle the players input
		****************************************
		*/
        // detects if a key is released and sets the game up to recieve another input
        // this prevents key spam from affecting the game. 

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
            timeRemaining = 6;

            //clear the screen at the start of the game
            for (int i = 1; i < Num_branches; i++)
            {
                branchPositions[i] = side::NONE;
            }

            //hide the gravestone at start
            spriteRIP.setPosition(675, 2000);    

            //Move player next to the tree
            spritePlayer.setPosition(580,720);

            // after screen is cleared, we can now accept player input
            acceptInput = true;         
        }

        //what to do if we can accept player input
        if (acceptInput)
        {
            //When the RIGHT ARROW key is pressed
            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                //move the player to the right
                playerSide = side::RIGHT;
                score++;

                //increase available game time
                timeRemaining = timeRemaining + (2/score) + .15;

                //move the axe with the player
                spriteAxe.setPosition(AXE_POSITION_RIGHT,spriteAxe.getPosition().y);

                spritePlayer.setPosition(1200,720);

                //update branch locations
                updateBranches(score);

                //set the log to fly off to the left
                spriteLog.setPosition(810,720);
                logSpeedX = -5000;
                logActive = true;

                acceptInput = false;
            }
            //When the LEFT ARROW key is pressed
            if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                //move the player to the left
                playerSide = side::LEFT;
                score++;

                //increase available game time
                timeRemaining = timeRemaining + (2/score) + .15;

                //move the axe with the player
                spriteAxe.setPosition(AXE_POSITION_LEFT,spriteAxe.getPosition().y);

                spritePlayer.setPosition(580,720);

                //update branch locations
                updateBranches(score);

                //set the log to fly off to the left
                spriteLog.setPosition(810,720);
                logSpeedX = 5000;
                logActive = true;

                acceptInput = false;
            }
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

                //Change the onscreen text - font is all caps
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

            //update branch sprites
            for (int i = 0; i < Num_branches; i++)
            {
                float height = i * 150;
                if (branchPositions[i] == side::LEFT)
                {
                    //Move the sprite to the left as well
                    branches[i].setPosition(610, height);

                    //flip the sprite around
                    branches[i].setRotation(180);
                }
                else if (branchPositions[i] == side::RIGHT)
                {
                    //Move the sprite to the right as well
                    branches[i].setPosition(1330, height);

                    //flip the sprite around
                    branches[i].setRotation(0);
                }
                else
                {
                    //hide the branches
                    branches[i].setPosition(3000,height);
                }
                
            }

            //logic for having a log fly off screen

            if (logActive)
            {
                spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()), spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));

                //has the log gone off the screen?
                if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000)
                {
                    logActive = false;
                    spriteLog.setPosition(810,720);
                }
            }

            //handle logic for player death, getting hit by branch
            if (branchPositions[5] == playerSide)
            {
                //end the game
                paused = true;
                acceptInput = false;

                //draw teh gravestone
                spriteRIP.setPosition(525,760);

                //hide player sprite
                spritePlayer.setPosition(2000,660);

                //tell the player how they died and position that message
                messageText.setString("YOU WERE SQUISHED BY A BRANCH!!");
                FloatRect textRect = messageText.getLocalBounds();

                messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

            }

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

        //Draw branches
        for (int i = 0; i < Num_branches; i++)
        {
            window.draw(branches[i]);
        }
        
        // Draw the tree on the screen
        window.draw(spriteTree);

        // Draw the player/axe/flying log/gravestone
        window.draw(spritePlayer);
        window.draw(spriteAxe);
        window.draw(spriteLog);
        window.draw(spriteRIP);

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

void updateBranches(int seed)
{
    //move brances down
    for (int j = Num_branches-1; j > 0; j++)
    {
        branchPositions[j] = branchPositions [j-1];
    }

    //fill in the branches randomly as old ones move down
    srand((int)time(0)+seed);
    //generate a number between 1-4 and store it in
    int r = (rand() % 5);
    switch (r)
    {
        case 0:
            branchPositions[0] = side::LEFT;
            break;

        case 1:
            branchPositions[0] = side::RIGHT;
            break;

        default:
            branchPositions[0] = side::NONE;
            break;
        
    }
}