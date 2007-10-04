<?php
/**************************************************************************
* This file is part of the WebIssues Server program
* Copyright (C) 2006-2007 Michał Męciński
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
**************************************************************************/

require_once( 'logging.inc' );

function wi_get_command()
{
    if ( !isset( $_GET[ 'command' ] ) )
        wi_error( 400 );
    $command = $_GET[ 'command' ];
    if ( get_magic_quotes_gpc() )
        $command = stripslashes( $command );

    $log = wi_log_open( 'commands' );
    if ( $log )
    {	
        fwrite( $log, "> $command\n" );
	}

    return wi_parse_command( $command );
}

function wi_parse_command( $command )
{
    $reg_kw = '[A-Z]+';
    $reg_str = '\'(?:\\\\[\'\\\\n]|[^\'\\\\])*\'';
    $reg_num = '-?\d+';
    $match = preg_match( "/^($reg_kw( $reg_kw)?)(( ($reg_str|$reg_num))*)$/", $command, $parts );
    if ( !$match )
        wi_error( 400 );
    $keyword = $parts[ 1 ];
    $args = array( $keyword );

    preg_match_all( "/($reg_str|$reg_num)/", $parts[ 3 ], $matches );
    foreach ( $matches[ 0 ] as $word ) {
        $letter = $word{ 0 };
        if ( $letter == '\'' ) {
            $string = wi_unquote( $word );
            if ( !wi_check_utf8( $string ) )
                wi_error( 400 );
            $args[] = $string;
        } else {
            if ( !wi_check_integer( $word ) )
                wi_error( 400 );
            $args[] = (int)$word;
        }
    }

    wi_verify_command( $args );

    return $args;
}

function wi_verify_command( $args )
{
    $command_args[ 'HELLO' ] = '';
    $command_args[ 'LOGIN' ] = 'ss';

    $command_args[ 'LIST USERS' ] = '';
    $command_args[ 'ADD USER' ] = 'sss';
    $command_args[ 'SET PASSWORD' ] = 'is';
    $command_args[ 'GRANT USER' ] = 'ii';
    $command_args[ 'RENAME USER' ] = 'is';

    $command_args[ 'LIST TYPES' ] = '';
    $command_args[ 'ADD TYPE' ] = 's';
    $command_args[ 'RENAME TYPE' ] = 'is';
    $command_args[ 'DELETE TYPE' ] = 'i';
    $command_args[ 'ADD ATTRIBUTE' ] = 'iss';
    $command_args[ 'RENAME ATTRIBUTE' ] = 'is';
    $command_args[ 'MODIFY ATTRIBUTE' ] = 'is';
    $command_args[ 'DELETE ATTRIBUTE' ] = 'i';

    $command_args[ 'LIST PROJECTS' ] = '';
    $command_args[ 'ADD PROJECT' ] = 's';
    $command_args[ 'RENAME PROJECT' ] = 'is';
    $command_args[ 'DELETE PROJECT' ] = 'i';
    $command_args[ 'ADD FOLDER' ] = 'iis';
    $command_args[ 'RENAME FOLDER' ] = 'is';
    $command_args[ 'DELETE FOLDER' ] = 'i';
    $command_args[ 'GRANT MEMBER' ] = 'iii';

    $command_args[ 'LIST ISSUES' ] = 'ii';
    $command_args[ 'GET DETAILS' ] = 'ii';
    $command_args[ 'ADD ISSUE' ] = 'is';
    $command_args[ 'RENAME ISSUE' ] = 'is';
    $command_args[ 'SET VALUE' ] = 'iis';
    $command_args[ 'ADD COMMENT' ] = 'is';
    $command_args[ 'ADD ATTACHMENT' ] = 'iss';
    $command_args[ 'GET ATTACHMENT' ] = 'i';
    $command_args[ 'FIND ITEM' ] = 'i';

    $keyword = array_shift( $args );
    if ( !isset( $command_args[ $keyword ] ) )
        wi_error( 401 );

    $types = $command_args[ $keyword ];

    $count = count( $args );
    if ( $count != strlen( $types ) )
        wi_error( 402 );

    for ( $i = 0; $i < $count; $i++ ) {
        $type = $types{ $i };
        $arg = $args[ $i ];
        if ( $type == 's' ) {
            if ( !is_string( $arg ) )
                wi_error( 402 );
        } else {
            if ( !is_int( $arg ) )
                wi_error( 402 );
        }
    }
}

function wi_check_utf8( $string )
{
    $reg_utf8 = "/^([\x09\x0a\x20-\x7e]|"
        . "[\xc2-\xdf][\x80-\xbf]|"
        . "\xe0[\xa0-\xbf][\x80-\xbf]|"
        . "[\xe1-\xec][\x80-\xbf]{2}|"
        . "\xed[\x80-\x9f][\x80-\xbf]|"
        . "\xee[\x80-\xbf]{2}|"
        . "\xef[\x80-\xbf][\x80-\xbd]|"
        . "\xf0[\x90-\xbf][\x80-\xbf]{2}|"
        . "[\xf1-\xf3][\x80-\xbf]{3}|"
        . "\xf4[\x80-\x8f][\x80-\xbf]{2})*$/";

    if ( !preg_match( $reg_utf8, $string ) )
        return false;

    return true;
}

function wi_length( $string )
{
    if ( function_exists( 'mb_strlen' ) )
        return mb_strlen( $string, 'UTF-8' );

    return preg_match_all( "/[\x01-\x7e\xc0-\xfd]/", $string, $matches );
}

function wi_check_integer( $string )
{
    $number = (float)$string;
    if ( $number < -2147483648.0 || $number > 2147483647.0 )
        return false;
    return true;
}

function wi_quote( $string )
{
    $string = str_replace( "\\", "\\\\", $string );
    $string = str_replace( "\n", "\\n", $string );
    $string = str_replace( "'", "\\'", $string );
    return "'$string'";
}

function wi_unquote( $string )
{
    return stripcslashes( substr( $string, 1, -1 ) );
}

function wi_string( $string, $max_length, $required = true )
{
    $string = rtrim( $string, ' ' );

    if ( $string == '' ) {
        if ( $required )
            wi_error( 340 );
        return $string;
    }

    if ( wi_length( $string ) > $max_length )
        wi_error( 340 );

    if ( preg_match( "/[\x01-\x1f]/", $string ) )
        wi_error( 340 );

    return $string;
}

function wi_attachment_path( $id, $create_dir )
{
    global $config;

    $dir1 = sprintf( "%s/%03d", $config[ 'storage_path' ], $id / 1000000 );
    $dir2 = sprintf( "%s/%03d", $dir1, ( $id / 1000 ) % 1000 );
    $file = sprintf( "%s/%03d", $dir2, $id % 1000 );

    if ( $create_dir ) {
        if ( !@is_dir( $dir1 ) )
            mkdir( $dir1, 0755 );
        if ( !@is_dir( $dir2 ) )
            mkdir( $dir2, 0755 );
    }

    return $file;
}

function wi_send_headers()
{
    if ( $_SERVER[ 'REQUEST_METHOD' ] != 'GET' ) {
        header( 'HTTP/1.1 405 Method Not Allowed' );
        exit;
    }

    ini_set( 'session.use_cookies', 1 );
    ini_set( 'session.use_only_cookies', 1 );
    ini_set( 'session.use_trans_sid', 0 );
    ini_set( 'url_rewriter.tags', '' );

    session_name( 'WebIssuesSID' );

    session_start();

    header( 'X-WebIssues-Version: 0.8' );

	header ('Content-Type: text/xml');
}

function wi_start_reply()
{
	echo "<response>\n";
}

function wi_reply( $reply, $roottag = '' )
{
    if ( empty( $reply ) )
        $reply = 'NULL';

    if ( !is_array( $reply ) )
        $reply = array( $reply );
	if ($roottag)
		echo "<$roottag>\n";
	foreach ($reply as $k => $v) {
		echo "<$k>$v</$k>\n";
	}
	if ($roottag)
		echo "</$roottag>\n";
}

function wi_finish_reply()
{
	echo "</response>";
	exit;
}

function wi_error( $code )
{
    $error_msg[ 300 ] = 'Login Required';
    $error_msg[ 301 ] = 'Access Denied';
    $error_msg[ 302 ] = 'Incorrect Login';
    $error_msg[ 310 ] = 'Unknown Project';
    $error_msg[ 311 ] = 'Unknown Folder';
    $error_msg[ 312 ] = 'Unknown Issue';
    $error_msg[ 313 ] = 'Unknown File';
    $error_msg[ 314 ] = 'Unknown User';
    $error_msg[ 315 ] = 'Unknown Type';
    $error_msg[ 316 ] = 'Unknown Attribute';
    $error_msg[ 320 ] = 'Project Already Exists';
    $error_msg[ 321 ] = 'Folder Already Exists';
    $error_msg[ 324 ] = 'User Already Exists';
    $error_msg[ 325 ] = 'Type Already Exists';
    $error_msg[ 326 ] = 'Attribute Already Exists';
    $error_msg[ 330 ] = 'Cannot Delete Project';
    $error_msg[ 331 ] = 'Cannot Delete Folder';
    $error_msg[ 335 ] = 'Cannot Delete Type';
    $error_msg[ 340 ] = 'Invalid String';
    $error_msg[ 341 ] = 'Invalid Access Level';
    $error_msg[ 342 ] = 'Invalid Value';
    $error_msg[ 343 ] = 'Invalid Definition';
    $error_msg[ 400 ] = 'Syntax Error';
    $error_msg[ 401 ] = 'Unknown Command';
    $error_msg[ 402 ] = 'Invalid Arguments';
    $error_msg[ 403 ] = 'Upload Error';
    $error_msg[ 500 ] = 'Server Error';
    $error_msg[ 501 ] = 'Database Error';

    $msg = $error_msg[ $code ];

    wi_reply( array('code'=>$code, 'text'=>$msg) ,"ERROR");
    exit;
}

function wi_error_handler( $errno, $msg, $file, $line )
{
    if ( $errno & error_reporting() ) {
        $log = wi_log_open( 'errors' );
        if ( $log ) {
            if ( $errno == E_NOTICE )
                $level = 'E_NOTICE';
            else
                $level = 'E_WARNING';

            fwrite( $log, "$level in $file($line):\n$msg\n" );
        }

        if ( $errno != E_NOTICE )
            wi_error( 500 );
    }
}

function wi_db_error_handler( $error )
{
    $log = wi_log_open( 'errors' );
    if ( $log )
        fwrite( $log, "DATABASE ERROR:\n$error\n" );

    wi_error( 501 );
}

error_reporting( E_ALL );
set_error_handler( 'wi_error_handler' );

wi_set_db_error_handler( 'wi_db_error_handler' );
