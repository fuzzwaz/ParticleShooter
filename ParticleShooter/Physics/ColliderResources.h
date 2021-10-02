//
//  ColliderResources.h
//  Particle Shooter
//
//  Created by Ramy Fawaz in 2021
//  Copyright (c) 2021 Ramy Fawaz. All rights reserved.
//

#include "Collider.h"

#pragma once

//A master list of all of the Colliders used in the game
namespace Resources
{
	struct Colliders
	{
		static Collider ITEM()
		{
			Collider item;
			item.SetIsTrigger(true);
			item.SetMinimumCollisionDistance(150);
			item._Baton._Type = ColliderType::PICKUP;
			item.SetColliderType(ColliderType::PICKUP);
			item.SetColliderIgnoreTypes({ ColliderType::PICKUP, ColliderType::PLAYERATTACK, ColliderType::ENEMY, ColliderType::ENVIRONMENT, ColliderType::ENEMYATTACK });

			return item;
		}

		static Collider ITEM_HEALTH()
		{
			Collider healthItem(ITEM());
			healthItem.SetIsCircular(true);
			healthItem.SetRadius(32);
			healthItem._Polygon.AddVertexPoint({ Vector2(-32,0), Vector2(-22.6,-22.6), Vector2(0,-32), Vector2(22.6,-22.6),
			Vector2(32,0), Vector2(22.6,22.6), Vector2(0,32), Vector2(-22.6,22.6) });
			healthItem._Baton._Id = ObjectId::PICK_UP_HEALTH;
			healthItem._Baton._Value = 20;

			return healthItem;
		}

		static Collider ITEM_BOUNCE()
		{
			Collider bounceItem(ITEM());
			bounceItem.SetIsCircular(false);
			bounceItem._Polygon.AddVertexPoint({ Vector2(-48,0), Vector2(0,-48), Vector2(48,0), Vector2(0,48) });
			bounceItem._Baton._Type = ColliderType::PICKUP;
			bounceItem._Baton._Id = ObjectId::PICK_UP_BOUNCE;

			return bounceItem;
		}

		static Collider PLAYER()
		{
			const int minCollisionDistance = 100;
			const float width = 64, height = 64;
			const ColliderType colliderType = ColliderType::PLAYER;

			Collider player;
			player._Polygon.AddVertexPoint({ Vector2(-(width / 2),(height / 2)), Vector2(0,-(height / 2)), Vector2((width / 2),(height / 2)) });
			player.SetColliderType(colliderType);
			player.SetColliderIgnoreTypes({ colliderType, ColliderType::PLAYERATTACK });
			player.SetMinimumCollisionDistance(minCollisionDistance);
			player._Baton._Type = colliderType;

			return player;
		}

		static Collider PLAYER_ATTACK()
		{
			Collider attack;
			attack.SetColliderType(ColliderType::PLAYERATTACK);
			attack.SetColliderIgnoreTypes({ ColliderType::PLAYERATTACK, ColliderType::PLAYER });
			attack._Baton._Type = ColliderType::PLAYERATTACK;

			return attack;
		}

		static Collider PLAYER_ATTACK_LASER()
		{
			Collider laser(PLAYER_ATTACK());
			laser.SetIsTrigger(true);
			laser.SetColliderIgnoreTypes({ ColliderType::PLAYERATTACK, ColliderType::PLAYER, ColliderType::ENVIRONMENT, ColliderType::PICKUP });
			laser._Polygon.AddVertexPoint({ Vector2(-26.5,0),Vector2(-26.5,-1300), Vector2(26.5,-1300), Vector2(26.5,0) });
			laser.SetIsActive(false);
			laser._Baton._Id = ObjectId::PLAYER_SPECIAL_ATTACK;
			laser._Baton._Value = 15;
			laser.SetMinimumCollisionDistance(2000);
			laser.SetIsTrigger(true);

			return laser;
		}

		static Collider PLAYER_ATTACK_BASIC()
		{
			Collider basic(PLAYER_ATTACK());
			basic.SetMinimumCollisionDistance(50);
			basic.SetIsCircular(true);
			basic.SetRadius(12);
			basic._Polygon.AddVertexPoint({ Vector2(-16,0), Vector2(-11.3,-11.3), Vector2(0,-16), Vector2(11.3,-11.3),
			Vector2(16,0), Vector2(11.3,11.3), Vector2(0,16), Vector2(-11.3,11.3), });
			basic._Baton._Id = ObjectId::PLAYER_PROJECTILE_BASIC;
			basic._Baton._Value = 15;

			return basic;
		}

		static Collider ENEMY_ATTACK()
		{
			Collider enemyAttack;
			enemyAttack.SetColliderType(ColliderType::ENEMYATTACK);
			enemyAttack.SetColliderIgnoreTypes({ ColliderType::ENEMYATTACK, ColliderType::ENEMY });
			enemyAttack._Baton._Type = ColliderType::ENEMYATTACK;
			return enemyAttack;
		}

		static Collider ENEMY_ATTACK_BASIC()
		{
			Collider basic(ENEMY_ATTACK());
			basic.SetMinimumCollisionDistance(70);
			basic._Polygon.AddVertexPoint({ Vector2(-8.3,0), Vector2(0,-24), Vector2(8.3,0), Vector2(0,24) });
			basic._Baton._Value = 5;
			basic._Baton._Id = ObjectId::ENEMY_PROJECTILE_BASIC;
			return basic;
		}

		static Collider ENEMY_ATTACK_HEX()
		{
			Collider hex(ENEMY_ATTACK());
			hex.SetMinimumCollisionDistance(1400);
			hex._Polygon.AddVertexPoint({ Vector2(-100,-14), Vector2(100,-14), Vector2(100,7), Vector2(-100,7) });
			hex._Baton._Value = 10;
			hex._Baton._Id = ObjectId::ENEMY_PROJECTILE_HEXBEAM;

			return hex;
		}

		static Collider ENEMY()
		{
			Collider enemy;
			enemy.SetIsActive(false);
			enemy.SetIsTrigger(false);
			enemy.SetColliderType(ColliderType::ENEMY);
			enemy.SetColliderIgnoreTypes({ ColliderType::ENEMYATTACK });
			enemy._Baton._Type = ColliderType::ENEMY;
			enemy._Baton._Value = 15;
			return enemy;
		}

		static Collider ENEMY_RHOMBUS()
		{
			Collider rhombus(ENEMY());
			rhombus.SetMinimumCollisionDistance(150);
			rhombus._Polygon.AddVertexPoint({ Vector2(-32,0), Vector2(0,-48), Vector2(32,0), Vector2(0,48) });
			rhombus._Baton._Id = ObjectId::ENEMY_RHOMBUS;
			return rhombus;
		}

		static Collider ENEMY_SQUARE()
		{
			Collider square(ENEMY());
			square.SetMinimumCollisionDistance(250);
			square._Polygon.AddVertexPoint({ Vector2(-64,-64), Vector2(64,-64), Vector2(64,64), Vector2(-64,64) });
			square._Baton._Id = ObjectId::ENEMY_SQUARE;
			return square;
		}

		static Collider ENEMY_HEXAGON()
		{
			Collider hex(ENEMY());
			hex.SetIsStatic(true);
			hex.SetMinimumCollisionDistance(350);
			hex._Polygon.AddVertexPoint({ Vector2(-96,0), Vector2(-48,-83.5), Vector2(48,-83.5), Vector2(96,0),
				Vector2(48,83.5), Vector2(-48,83.5) });
			hex._Baton._Id = ObjectId::ENEMY_HEXAGON;
			return hex;
		}

		static Collider ENVIRONMENT()
		{
			Collider environment;
			environment.SetIsStatic(true);
			environment.SetColliderType(ColliderType::ENVIRONMENT);
			environment._Baton._Type = ColliderType::ENVIRONMENT;
			return environment;
		}
	};
}