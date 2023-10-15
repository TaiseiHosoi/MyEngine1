/**
 * @file CollisionAttribute.h
 * @brief 当たり判定の属性
 */
#pragma once

//当たり判定属性
const unsigned short COLLISION_ATTR_LANDSPHERE = 0b1 << 0;
const unsigned short COLLISION_ATTR_ALLIES = 0b1 << 1;
const unsigned short COLLISION_ATTR_ENEMIES = 0b1 << 2;
const unsigned short COLLISION_ATTR_ENEMIEBULLETS = 0b1 << 3;
const unsigned short COLLISION_ATTR_NONE = 0b1 << 8;