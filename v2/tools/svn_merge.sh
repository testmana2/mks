#!/bin/sh
#------------

os=(`uname`)
svn_path=
svn_bin=svn

if [  $os = "Darwin" ]; then
	svn_path=/usr/local/bin
fi

svn="$svn_path/$svn_bin"

rev1=$1
rev2=$2

usage()
{
	echo "MkS Svn Merging Tool."
	echo ""
	echo "Usage:	$0 revision1 [revision2]"
	echo ""
	echo "	revision1 is the revision to compare from."
	echo "	revision2 is the revision to compare to, defaulting to HEAD."
}

if [ -z "$rev1" ]; then
	usage
	exit
fi

if [ -z "$rev2" ]; then
	rev2="HEAD";
fi

echo "Merging using first revision at $rev1 and second at $rev2 from branches/dev to trunk..."

$svn merge -r $rev1:$rev2 ../branches/dev ../trunk