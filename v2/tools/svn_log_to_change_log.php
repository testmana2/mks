#!/usr/bin/php
<?php
//echo $argv[0]." kool\n";

function usage( $argc, $argv )
{
	echo "SvnLog to ChangeLog.\n";
	echo "\n";
	echo "Usage:	". $argv[ 0 ] ." path_to_check [output_file_path]\n";
	echo "\n";
	echo "	path_to_check is the svn working copy path to check for log.\n";
	echo "	output_file_path is an optionnal file path to write the result to, defaulting to ./ChangeLog\n";
}

function generateChangeLog( $argc, $argv )
{
	$title="Monkey Studio IDE ChangeLog";
	$path = $argv[ 1 ];
	$fileName = "./ChangeLog";
	
	if ( $argc > 2 )
	{
		$fileName = $argv[ 2 ];
	}
	
	system( "svn2cl --html --output=\"$fileName\" --title=\"$title\" \"$path\"" );
}

if ( $argc == 1 )
{
	usage( $argc, $argv );
	exit;
}

generateChangeLog( $argc, $argv );
?>