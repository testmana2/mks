#include "Monkey.h"
//
int main( int argc, char** argv )
{
	Monkey* a = Monkey::self( argc, argv );
	int i = a->exec();
	delete a;
	return i;
}
