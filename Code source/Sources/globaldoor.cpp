/**
 * @file globaldoor.cpp
 * @brief Implementation of the GlobalDoor base class.
 *
 * The constructor simply forwards position and dimensions to Entity.
 * All door-specific logic (open/close state, teleportation, key checking)
 * lives in the concrete subclasses so that this file stays minimal and
 * the inheritance chain remains easy to extend.
 */

#include "Headers/globaldoor.h"

GlobalDoor::GlobalDoor(Vector2f pos, float w, float h) : Entity(pos, w, h) {}
