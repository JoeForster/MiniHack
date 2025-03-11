#pragma once

#include "DrawDebugHelpers.h"

#define DRAW_SPHERE(location) if (GetWorld()) DrawDebugSphere(GetWorld(), location, 20.f, 10, FColor::Emerald, false);
#define DRAW_SPHERE_ONEFRAME(location) if (GetWorld()) DrawDebugSphere(GetWorld(), location, 20.f, 10, FColor::Emerald, false);
#define DRAW_SPHERE_COLOUR(location, colour) if (GetWorld()) DrawDebugSphere(GetWorld(), location, 20.f, 10, colour, false);
#define DRAW_LINE(startLocation, endLocation) if (GetWorld()) DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Emerald, false, -1.f, 0, 1.f);
#define DRAW_POINT(location) if (GetWorld()) DrawDebugPoint(GetWorld(), location, 10.f, FColor::Emerald, false);
#define DRAW_CROSSHAIRS(location, rotation) if (GetWorld()) if (GetWorld()) DrawDebugCrosshairs(GetWorld(), location, rotation, 20.f, FColor::Emerald, false;
#define DRAW_CROSSHAIRS_COLOUR(location, rotation, colour) if (GetWorld()) if (GetWorld()) DrawDebugCrosshairs(GetWorld(), location, rotation, 20.f, colour, false);
#define DRAW_VECTOR(startLocation, endLocation) if (GetWorld()) \
	{ \
		DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Emerald, true, -1.f, 0, 1.f); \
		DrawDebugPoint(GetWorld(), endLocation, 10.f, FColor::Emerald, true); \
	}