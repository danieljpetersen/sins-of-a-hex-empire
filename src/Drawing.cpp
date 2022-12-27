#include "Drawing.h"
#include "Application.h"
#include "Constants.h"
#include "Game.h"
#include <math.h>
#include <sstream>

Drawing::Drawing()
{
	App = nullptr;
}

void Drawing::init(SFML_Wrapper::CoreApplication *TheApp, Game_System &Game)
{
	App = TheApp;

	font.loadFromFile("calibri.ttf");
    TileTextures.loadFromFile("Tiles.png");
    UnitTextures.loadFromFile("Units.gif");

	VersionNumber.setCharacterSize(12);
	VersionNumber.setFont(font);
	VersionNumber.setPosition(0, 0);
	VersionNumber.setString("Version 0.2");
	VersionNumber.setColor(sf::Color::White);

	TileVertex.clear();
	TileVertex.setPrimitiveType(sf::Quads);

	UnitSprites.resize(Game.Board.NUMBER_OF_TILES);
	for (unsigned int i = 0; i < Game.Board.NUMBER_OF_TILES; i++)
	{
		UnitSprites[i].setTexture(UnitTextures);
	}
	MiscTileSprite.setTexture(TileTextures);

	this->setupTileVertexArrayPositions(Game.Board);
}

//==================================

void Drawing::Line(sf::Vector2f a, sf::Vector2f b, sf::Color ColorToDraw)
{	
	sf::VertexArray lines(sf::Lines, 2);	
	lines[0].position = a;
	lines[0].color = ColorToDraw;
	lines[1].position = b;
	lines[1].color = ColorToDraw;

	App->MainWindow.draw(lines);
}

//==================================/

void Drawing::Line(sf::Vector2f a, sf::Vector2f b, sf::Color ColorToDraw, sf::Transform Transformation)
{	
	sf::VertexArray lines(sf::Lines, 2);	
	lines[0].position = a;
	lines[0].color = ColorToDraw;
	lines[1].position = b;
	lines[1].color = ColorToDraw;

	App->MainWindow.draw(lines, Transformation);
}

//==================================
 
void Drawing::Line(sf::Vector2f a, sf::Vector2f b, sf::Color ColorToDraw, float Thickness)
{	
	sf::Vector2f Normal(a.y - b.y, b.x - a.x);
	float Length = std::sqrt(Normal.x * Normal.x + Normal.y * Normal.y);
	if (Length != 0.f)
		Normal /= Length;
	
	Normal = Normal * Thickness;
	Normal = Normal / 2.0f;

	sf::VertexArray Line(sf::Quads);
	sf::Vertex Vert[4] =
	{
		sf::Vertex(a - Normal, ColorToDraw),
		sf::Vertex(b - Normal, ColorToDraw),
		sf::Vertex(b + Normal, ColorToDraw),
		sf::Vertex(a + Normal, ColorToDraw)
	};

	Line.append(Vert[0]);
	Line.append(Vert[1]);
	Line.append(Vert[2]);
	Line.append(Vert[3]);

	App->MainWindow.draw(Line);
}

//==================================
 
void Drawing::Line(sf::Vector2f a, sf::Vector2f b, sf::Color ColorToDraw, float Thickness, sf::Transform Transformation)
{
	sf::Vector2f Normal(a.y - b.y, b.x - a.x);
	float Length = std::sqrt(Normal.x * Normal.x + Normal.y * Normal.y);
	if (Length != 0.f)
		Normal /= Length;
	
	Normal = Normal * Thickness;
	Normal = Normal / 2.0f;

	sf::VertexArray Line(sf::Quads);
	sf::Vertex Vert[4] =
	{
		sf::Vertex(a - Normal, ColorToDraw),
		sf::Vertex(b - Normal, ColorToDraw),
		sf::Vertex(b + Normal, ColorToDraw),
		sf::Vertex(a + Normal, ColorToDraw)
	};

	Line.append(Vert[0]);
	Line.append(Vert[1]);
	Line.append(Vert[2]);
	Line.append(Vert[3]);

	App->MainWindow.draw(Line, Transformation);
}

//==================================

void Drawing::Line(sf::Vector2f a, sf::Vector2f b, sf::Color aColorToDraw, sf::Color bColorToDraw, float Thickness)
{
	sf::Vector2f Normal(a.y - b.y, b.x - a.x);
	float Length = std::sqrt(Normal.x * Normal.x + Normal.y * Normal.y);
	if (Length != 0.f)
		Normal /= Length;
	
	Normal = Normal * Thickness;
	Normal = Normal / 2.0f;

	sf::VertexArray Line(sf::Quads);
	sf::Vertex Vert[4] =
	{
		sf::Vertex(a - Normal, aColorToDraw),
		sf::Vertex(b - Normal, aColorToDraw),
		sf::Vertex(b + Normal, bColorToDraw),
		sf::Vertex(a + Normal, bColorToDraw)
	};

	Line.append(Vert[0]);
	Line.append(Vert[1]);
	Line.append(Vert[2]);
	Line.append(Vert[3]);

	App->MainWindow.draw(Line);
}

//==================================
 
void Drawing::Rect(sf::Vector2f Position, float Height, float Width, sf::Color ColorToDraw)
{
	sf::Vertex rect[4] =
	{
		sf::Vertex(Position, ColorToDraw),
		sf::Vertex(sf::Vector2f(Position.x + Width, Position.y), ColorToDraw),
		sf::Vertex(sf::Vector2f(Position.x + Width, Position.y + Height), ColorToDraw),
		sf::Vertex(sf::Vector2f(Position.x, Position.y + Height), ColorToDraw)
	};

	App->MainWindow.draw(rect, 4, sf::Quads);
}

//==================================

void Drawing::Rect(sf::Vector2f Position, float Height, float Width, sf::Color ColorToDraw, sf::Transform Transformation)
{
	sf::Vertex rect[4] =
	{
		sf::Vertex(Position, ColorToDraw),
		sf::Vertex(sf::Vector2f(Position.x + Width, Position.y), ColorToDraw),
		sf::Vertex(sf::Vector2f(Position.x + Width, Position.y + Height), ColorToDraw),
		sf::Vertex(sf::Vector2f(Position.x, Position.y + Height), ColorToDraw)
	};

	App->MainWindow.draw(rect, 4, sf::Quads, Transformation);
}

//==================================
 
void Drawing::circle(sf::Vector2f Position, float Radius, sf::Color Color)
{
	sf::CircleShape Test;
	Test.setFillColor(Color);
	Test.setPosition(Position);
	Test.setRadius(Radius);
	
	App->MainWindow.draw(Test);
}

//==================================
 
void Drawing::circle(sf::Vector2f Position, float Radius, sf::Color Color, sf::Transform Transformation)
{
	sf::CircleShape Test;
	Test.setFillColor(Color);
	Test.setPosition(Position);
	Test.setRadius(Radius);
	
	App->MainWindow.draw(Test, Transformation);
}

//==================================

void Drawing::setupTileVertexArrayPositions(Board_Component &Board)
{
	TileVertex.clear();
	TileVertex.resize(Board.NUMBER_OF_TILES * 4);

	int Index = 0;
	for (unsigned int i = 0; i < TileVertex.getVertexCount(); i = i + 4)
	{
		sf::Vector2f Position1 = Board.Tiles[Index].getPosition();
		sf::Vector2f Position2(Position1.x + TILE_WIDTH, Position1.y);
		sf::Vector2f Position3(Position1.x + TILE_WIDTH, Position1.y + TILE_HEIGHT);
		sf::Vector2f Position4(Position1.x, Position1.y + TILE_HEIGHT);
				
		sf::Vertex Vert[4] = 
		{
			sf::Vertex(Position1),
			sf::Vertex(Position2),
			sf::Vertex(Position3),
			sf::Vertex(Position4)
		};

		TileVertex[i].position = Vert[0].position;
		TileVertex[i + 1].position = Vert[1].position;
		TileVertex[i + 2].position = Vert[2].position;
		TileVertex[i + 3].position = Vert[3].position;

		sf::Vector2f SpriteOffset(Position1.x + 10, Position1.y + 4);
		UnitSprites[Index].setPosition(SpriteOffset);
		Index++;
	}
}

//==================================

void Drawing::drawScene(Game_System &Game)
{
	drawTiles(Game.Board);
	drawUnits(Game);
	App->MainWindow.draw(VersionNumber);
	//debugDrawPaths(Game.Pathfinder, Game.Board);
}
 
//==================================

void Drawing::drawTiles(Board_Component &Board)
{
	int TileIndex = 0, VertexIndex=0, TileType = 0, TileOwner = 0;
	if (GlobalAppFunctions.UpdateTiles)
	{
		for (unsigned int i = 0; i < Board.NUMBER_OF_TILES; i++)
		{
			TileIndex = i;
			VertexIndex = TileIndex * 4;

			TileType = Board.Tiles[TileIndex].getType();
			TileOwner = Board.Tiles[TileIndex].getOwner();

			const int NO_PLAYER = -1;
			const int PLAYER_1 = 0;
			const int PLAYER_2 = 1;
			const int PLAYER_3 = 2;
			const int PLAYER_4 = 3;

			float xTopLeft = 0, yTopLeft = 0;

			//find the proper tile on sprite sheet
			if (TileType == WATER)
			{
				xTopLeft = 0;
				yTopLeft = 0;
			}
	
			else if (TileType == LAND)
			{
				if (TileOwner == NO_PLAYER)
				{
					xTopLeft = 50;
					yTopLeft = 0;
				}
				
				else if (TileOwner == PLAYER_1)
				{
					xTopLeft = 0;
					yTopLeft = 120;
				}

				else if (TileOwner == PLAYER_2)
				{
					xTopLeft = 50;
					yTopLeft = 120;
				}

				else if (TileOwner == PLAYER_3)
				{
					xTopLeft = 100;
					yTopLeft = 120;
				}

				else if (TileOwner == PLAYER_4)
				{
					xTopLeft = 150;
					yTopLeft = 120;
				}
			}

			else if (TileType == PORT)
			{			
				if (TileOwner == NO_PLAYER)
				{
					xTopLeft = 100;			
					yTopLeft = 0;			
				}

				else if (TileOwner == PLAYER_1)
				{
					xTopLeft = 0;			
					yTopLeft = 160;			
				}

				else if (TileOwner == PLAYER_2)
				{
					xTopLeft = 50;			
					yTopLeft = 160;			
				}

				else if (TileOwner == PLAYER_3)
				{
					xTopLeft = 100;			
					yTopLeft = 160;			
				}

				else if (TileOwner == PLAYER_4)
				{
					xTopLeft = 150;			
					yTopLeft = 160;			
				}
			}

			else if (TileType == CITY)
			{
				yTopLeft = 40;

				//find the x value to get the proper color
				if (TileOwner == NO_PLAYER)
				{
					xTopLeft = 150;			
					yTopLeft = 0;			
				}

				if (TileOwner == PLAYER_1)
					xTopLeft = 0;
					
				if (TileOwner == PLAYER_2)
					xTopLeft = 50;
					
				if (TileOwner == PLAYER_3)
					xTopLeft = 100;
				
				if (TileOwner == PLAYER_4)
					xTopLeft = 150;
			}

			else if (TileType == CAPITAL)
			{
				yTopLeft = 80;
				
				//find the x value to get the proper color
				if (TileOwner == PLAYER_1)
					xTopLeft = 0;			

				if (TileOwner == PLAYER_2)
					xTopLeft = 50;
					
				if (TileOwner == PLAYER_3)
					xTopLeft = 100;
					
				if (TileOwner == PLAYER_4)
					xTopLeft = 150;
			}

			sf::Vector2f TexCoords1(xTopLeft, yTopLeft);
			sf::Vector2f TexCoords2(xTopLeft + TILE_WIDTH, yTopLeft);
			sf::Vector2f TexCoords3(xTopLeft + TILE_WIDTH, yTopLeft + TILE_HEIGHT);
			sf::Vector2f TexCoords4(xTopLeft, yTopLeft + TILE_HEIGHT);
					
			TileVertex[VertexIndex].texCoords = TexCoords1;
			TileVertex[VertexIndex + 1].texCoords = TexCoords2;
			TileVertex[VertexIndex + 2].texCoords = TexCoords3;
			TileVertex[VertexIndex + 3].texCoords = TexCoords4;
		}
	}

	App->MainWindow.draw(TileVertex, &TileTextures);
	GlobalAppFunctions.UpdateTiles = false;
}

void Drawing::drawUnits(Game_System &Game)
{
	for (unsigned int PlayerIndex = 0; PlayerIndex < Game.Players.size(); PlayerIndex++)
	{
		for (unsigned int ArmyIndex = 0; ArmyIndex < Game.Players[PlayerIndex].Units.size(); ArmyIndex++)
		{	
			if (Game.Board.isValidTile(Game.Players[PlayerIndex].Units[ArmyIndex].getTile()))
				if (Game.Players[PlayerIndex].Units[ArmyIndex].getIsUnitActive())
				{
					this->drawUnit(Game, PlayerIndex, ArmyIndex);
					this->drawExperience(Game, PlayerIndex, ArmyIndex);
				}
		}
	}
	
	//if a unit is selected, render all the shit that goes along with that
	drawMovementOverlays(Game);
}

void Drawing::drawUnit(Game_System &Game, int PlayerIndex, int ArmyIndex)
{
	//this is incredibly old code that worked under some different assumptions on where data was located...  it still works, but since it's old and also since I sucked a lot more at coding back then, it's a lot more verbose than it needs to be.  I don't care enough to fix it
	sf::IntRect Clip;
	Clip.left = 0;
	Clip.top = 0;
	Clip.height = 0;
	Clip.width = 0;

	int TileIndex = Game.Players[PlayerIndex].Units[ArmyIndex].getTile();
	int TileType = Game.Board.Tiles[TileIndex].getType();
	int TileOwner = Game.Board.Tiles[TileIndex].getOwner();
	if (Game.Players[PlayerIndex].Units[ArmyIndex].getArmySize() != 0)
	{
		//render overlay under any unit who hasn't moved this turn, as long as no unit is selected
		if (Game.getHumanNationIndex() == Game.Board.Tiles[TileIndex].getOwner())	
			if (Game.Players[PlayerIndex].Units[ArmyIndex].getDidUnitMove() != true)
				if (Game.Board.Tiles[TileIndex].getArmyIndex() != -1)
					if (Game.getSelectedTile() == -1)
					{
						Clip.left = 50;
						Clip.top = 200;
						Clip.height = 40;
						Clip.width = 50;

						MiscTileSprite.setTextureRect(Clip);
						MiscTileSprite.setPosition(Game.Board.Tiles[TileIndex].getPosition());
						App->MainWindow.draw(MiscTileSprite);
					}
		
		//if we're on a capital/city with no unit selected, render the small unit so we don't cover up the capital/city
		if (((TileType == CITY) || (TileType == CAPITAL)))
		{
			//if the unit doesn't belong to us, always render the small version of the unit sprite
			if ((Game.getHumanNationIndex() != TileOwner) && (Game.getSelectedTile() == -1))
			{
				Clip.left = 0;
				Clip.top = 0;
				Clip.height = 10;
				Clip.width = 10;
				
				
				UnitSprites[TileIndex].setTextureRect(Clip);
				App->MainWindow.draw(UnitSprites[TileIndex]);
			}

			//if the unit does belong to us, render the full sprite if we have a unit selected
			else if ((Game.getHumanNationIndex() == TileOwner) || (Game.getSelectedTile() != -1))
			{
				if (Game.getSelectedTile() != -1)
				{
					Clip = handleUnitLocationOnSpriteSheet(Game.Players[PlayerIndex].Units[ArmyIndex].getArmySize());
					//UnitSprites.setPosition(sf::Vector2f((float)aGameObject.BoardObject.getHexSelectionArea(Index).x, (float)aGameObject.BoardObject.getHexSelectionArea(Index).y));
					UnitSprites[TileIndex].setTextureRect(Clip);
					App->MainWindow.draw(UnitSprites[TileIndex]);
				}
			}

			//render the small sprite if it's ours and there's nothing selected
			if (Game.getHumanNationIndex() == TileOwner)
			{
				if (Game.getSelectedTile() == -1)
				{
					Clip.left = 0;
					Clip.top = 0;
					Clip.height = 10;
					Clip.width = 10;
				
					UnitSprites[TileIndex].setTextureRect(Clip);
					App->MainWindow.draw(UnitSprites[TileIndex]);
				}
			}
		}

		//if we're on anything else, render the full sprite
		else
		{
			Clip = handleUnitLocationOnSpriteSheet(Game.Players[PlayerIndex].Units[ArmyIndex].getArmySize());
			UnitSprites[TileIndex].setTextureRect(Clip);
			App->MainWindow.draw(UnitSprites[TileIndex]);
		}
	}
}

void Drawing::drawMovementOverlays(Game_System &Game)
{
	//render overlays over hexes that can be moved to, and render a full unit sprite over capitals and cities
	if (Game.getSelectedTile() != -1)
	{	
		//render overlay on the hex tile for the unit selected	
		sf::IntRect Clip;
		/*Clip.left = 50;
		Clip.top = 200;
		Clip.height = 40;
		Clip.width = 50;	

		//MiscTileSprite.setTextureRect(Clip);
		//App->MainWindow.draw(MiscTileSprite);
		*/
		//render overlay under any tiles that the selected unit can move to		
		Clip.left = 0;
		Clip.top = 200;
		Clip.height = 40;
		Clip.width = 50;			
		MiscTileSprite.setTextureRect(Clip);
		sf::Vector2f OriginalPosition = MiscTileSprite.getPosition();
		for (int i = 0; i < 18; i++)
		{
			if (Game.getTilesInMovementRange(i) != -1)
			{
				MiscTileSprite.setPosition(Game.Board.Tiles[Game.getTilesInMovementRange(i)].getPosition());
				App->MainWindow.draw(MiscTileSprite);	
			}
		}

		MiscTileSprite.setPosition(OriginalPosition);
	}
}

void Drawing::drawExperience(Game_System &Game, int PlayerIndex, int ArmyIndex)
{
	int TileIndex = Game.Players[PlayerIndex].Units[ArmyIndex].getTile();
	if (TileIndex != -1)
	{
		int TileType = Game.Board.Tiles[TileIndex].getType();
		if ((TileType == LAND) || (Game.getSelectedTile() != -1))
		{
			sf::IntRect Clip;
			Clip.left = 90;
			Clip.top = 30;
			Clip.height = 9;
			Clip.width = 9;
			UnitSprites[TileIndex].setTextureRect(Clip);

			sf::Vector2f Offset(0,0);
			sf::Vector2f OriginalPosition = UnitSprites[TileIndex].getPosition();
			int ExperienceSize = Game.Players[PlayerIndex].Units[ArmyIndex].getExperience();
			for (int i = 0; i < ExperienceSize; i++)
			{
				UnitSprites[TileIndex].setPosition(OriginalPosition + Offset);
				App->MainWindow.draw(UnitSprites[TileIndex]);
			
				Offset.x += 9 + 1;

				if (i == 2)
				{
					Offset.x = 0;
					Offset.y = 10;
				}
				if (i == 5)
				{
					Offset.x = 0;
					Offset.y = 20;
				}
			}
		
			UnitSprites[TileIndex].setPosition(OriginalPosition);
		}
	}
}

sf::IntRect Drawing::handleUnitLocationOnSpriteSheet(int ArmySize)
{
	sf::IntRect Clip;

	if (ArmySize == 1)
	{
		Clip.left = 0;
		Clip.top = 0;
		Clip.height = 29;
		Clip.width = 29;
	}

	else if (ArmySize == 2)
	{
		Clip.left = 30;
		Clip.top = 0;
		Clip.height = 29;
		Clip.width = 29;
	}
			
	else if (ArmySize == 3)
	{
		Clip.left = 60;
		Clip.top = 0;
		Clip.height = 29;
		Clip.width = 29;
	}	
	
	else if (ArmySize == 4)
	{
		Clip.left = 90;
		Clip.top = 0;
		Clip.height = 29;
		Clip.width = 29;	
	}	
	
	else if (ArmySize == 5)
	{
		Clip.left = 120;
		Clip.top = 0;
		Clip.height = 29;
		Clip.width = 29;	
	}	
	
	else if (ArmySize == 6)
	{
		Clip.left = 150;
		Clip.top = 0;
		Clip.height = 29;
		Clip.width = 29;	
	}	
	
	else if (ArmySize == 7)
	{
		Clip.left = 0;
		Clip.top = 30;
		Clip.height = 29;
		Clip.width = 29;	
	}

	else if (ArmySize == 8)
	{
		Clip.left = 30;
		Clip.top = 30;
		Clip.height = 29;
		Clip.width = 29;	
	}	
	
	else if (ArmySize == 9)
	{
		Clip.left = 60;
		Clip.top = 30;
		Clip.height = 29;
		Clip.width = 29;	
	}

	return Clip;
}


static inline std::string convertIntToString(int Number)
{
	std::stringstream Convert;
	Convert << Number;
	return Convert.str();
}


void Drawing::debugDrawPaths(Pathfinding &Pathfinder, Board_Component &BoardObject)
{
	int Cost = 0;
	sf::Text TextToDraw;
	TextToDraw.setCharacterSize(14);
	TextToDraw.setColor(sf::Color(0,0,0));
	TextToDraw.setString("0");
	TextToDraw.setFont(font);
	
	for (int i = 0; i < BoardObject.NUMBER_OF_TILES; i++)
	{
		Cost = Pathfinder.getTurnsToReachGoal(App->Input.DEBUG_TILE, i);//TOP_RIGHT_CAPITAL, i);
		TextToDraw.setString(convertIntToString(Cost));
		TextToDraw.setPosition(sf::Vector2f(BoardObject.Tiles[i].getPosition().x + 8, BoardObject.Tiles[i].getPosition().y));

		App->MainWindow.draw(TextToDraw);
	}
}