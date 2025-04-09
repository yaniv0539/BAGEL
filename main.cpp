#include <iostream>

struct Position {
	int x;
	int y;
};
struct Direction {
	int dx;
	int dy;
};


#include "bagel.h"
using namespace std;
using namespace bagel;

void run_tests();


int main()
{
	Mask m2 = MaskBuilder()
		.add<Position>()
		.add<Direction>()
		.build();

	ent_type e = World::createEntity();
	World::addComponents(e,
		Position{1,2},
		Direction{4,4}
	);
	return 0;
}
