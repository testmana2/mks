#include "Monkey.h"
//
int main( int argc, char** argv )
{
	Monkey* a = Monkey::self( argc, argv );
	return a->exec();
}
