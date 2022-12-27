#ifndef DRAW_SYSTEM_H
#define DRAW_SYSTEM_H

#include <SFML/Graphics.hpp>

class Game_System;
class Board_Component;
class Pathfinding;

namespace SFML_Wrapper
{
	class CoreApplication;
}

class Drawing 
{
private:	
	SFML_Wrapper::CoreApplication *App;
	sf::Text VersionNumber;

public:
	Drawing();
	
	void init(SFML_Wrapper::CoreApplication *TheApp, Game_System &Game);
	
	sf::Font font;
	
	void Line(sf::Vector2f a, sf::Vector2f b, sf::Color ColorToDraw);

	void Line(sf::Vector2f a, sf::Vector2f b, sf::Color ColorToDraw, sf::Transform Transformation);

	void Line(sf::Vector2f a, sf::Vector2f b, sf::Color ColorToDraw, float Thickness);

	void Line(sf::Vector2f a, sf::Vector2f b, sf::Color ColorToDraw, float Thickness, sf::Transform Transformation);

	void Line(sf::Vector2f a, sf::Vector2f b, sf::Color aColorToDraw, sf::Color bColorToDraw, float Thickness);
	
	void Rect(sf::Vector2f Position, float Height, float Width, sf::Color ColorToDraw);

	void Rect(sf::Vector2f Position, float Height, float Width, sf::Color ColorToDraw, sf::Transform Transformation);
	
	void circle(sf::Vector2f Position, float Radius, sf::Color Color);

	void circle(sf::Vector2f Position, float Radius, sf::Color Color, sf::Transform Transformation);

	//==================================

	sf::Texture TileTextures, UnitTextures;

	sf::VertexArray TileVertex;

	std::vector<sf::Sprite> UnitSprites;

	sf::Sprite MiscTileSprite;

	void setupTileVertexArrayPositions(Board_Component &Board);
	
	void drawScene(Game_System &Game);
	
	void drawTiles(Board_Component &Board);

	void drawUnits(Game_System &Game);

	void drawUnit(Game_System &Game, int PlayerIndex, int ArmyIndex);

	void drawMovementOverlays(Game_System &Game);

	void drawExperience(Game_System &Game, int PlayerIndex, int ArmyIndex);

	sf::IntRect handleUnitLocationOnSpriteSheet(int ArmySize);

	void debugDrawPaths(Pathfinding &Pathfinder, Board_Component &BoardObject);
};
 
#endif 