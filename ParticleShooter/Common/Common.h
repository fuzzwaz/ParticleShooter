//
//  Common.h
//  Particle Shooter
//
//  Created by Ramy Fawaz in 2021
//  Copyright (c) 2015 Ramy Fawaz. All rights reserved.
//

#pragma once

struct Vector2;

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
const int DESIRED_UPDATES_PER_SECOND = 60;
const float FRAME_LENGTH = (1.0 / DESIRED_UPDATES_PER_SECOND);

enum Movement { FORWARD, BACKWARDS, RIGHT_SIDE, LEFT_SIDE };
enum Direction { UP, DOWN, LEFT, RIGHT };
enum ColliderType { ENVIRONMENT, ENEMY, ENEMYATTACK, PLAYER, PLAYERATTACK, PICKUP, ALL };
enum class RenderingLayer { UI = 0, PARTICLES, PLAYER, ENEMIES, ITEMS, GENERAL, LEVEL };

//Identification for game objects. Useful for when collision response is unique to a specific object
enum class ObjectId
{
	ENVIRONMENT,
	PICK_UP_HEALTH,
	PICK_UP_BOUNCE,
	PLAYER_PROJECTILE_BASIC,
	PLAYER_SPECIAL_ATTACK,
	ENEMY_PROJECTILE_BASIC,
	ENEMY_PROJECTILE_HEXBEAM,
	ENEMY_RHOMBUS,
	ENEMY_SQUARE,
	ENEMY_HEXAGON
};

//A grouping of some of the most helpful, reused, and lightweight functions 
struct CommonHelpers
{
	static int DirectionIndex(Direction dir) { return static_cast<int>(dir); }
	static Direction IndexDirection(int id) { return static_cast<Direction>(id); }

	static double DegToRad(double degrees) { return ((degrees)* 3.141592653589793238463) / 180.0; }

	static double RadToDeg(double radians) { return ((radians) * 180.0) / 3.141592653589793238463; }

	static bool AreEqual(double a, double b, const double acceptableDifference = 0.01);

	static bool AreEqual(Vector2 a, Vector2 b, const double acceptableDifference = 0.01);

	static Vector2 RandomOffset(int offsetMagnitude);
};

