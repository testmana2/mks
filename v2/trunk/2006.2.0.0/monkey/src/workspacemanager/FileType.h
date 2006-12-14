#ifndef FILETYPE_H
#define FILETYPE_H
//
namespace FileType
{
	enum FileType { ftUnknow = 0,
					ftHeader,
					ftSource,
					ftForm };
	//
	void initialize();
	//
};
//
#endif // FILETYPE_H
