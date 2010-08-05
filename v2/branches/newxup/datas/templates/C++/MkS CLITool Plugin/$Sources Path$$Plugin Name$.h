#ifndef $Plugin Name.upper$_H
#define $Plugin Name.upper$_H

#include <CLIToolInterface.h>

class $Plugin Name$ : public CLIToolInterface
{
	Q_INTERFACES( CLIToolInterface )

public:
	virtual pCommandList defaultCommands() const;
};

#endif // $Plugin Name.upper$_H
