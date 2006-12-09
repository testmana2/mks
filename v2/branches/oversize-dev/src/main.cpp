#include "Monkey.h"
//
int main( int argc, char** argv )
{
	Monkey* a = Monkey::self( argc, argv );
	a->initialize();
	return a->exec();
}
