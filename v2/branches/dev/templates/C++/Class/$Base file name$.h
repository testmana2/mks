#ifndef $Base file name.upper$_H
#define $Base file name.upper$_H

class $Class name$
{
public:
	$Class name$();
	virtual ~$Class name$();

	/*** For dynamic memory management (can be removed if it isn't necessary) ***/
	$Class name$(const $Class name$ & obj);
	$Class name$ & operator =(const $Class name$ & obj);
	bool operator ==(const $Class name$ & obj) const;
	bool operator !=(const $Class name$ & obj) const;
	/*** End of dynamic memory management ***/
};

#endif // $Base file name.upper$_H
