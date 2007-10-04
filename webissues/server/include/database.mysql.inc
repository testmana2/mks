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

function wi_db_connect( $host, $database, $user, $password )
{
    if ( !@mysql_connect( $host, $user, $password ) )
        return false;

    if ( !@mysql_select_db( $database ) )
        return false;

    $info = mysql_get_server_info();
    $version = explode( '.', $info );

    if ( version_compare( mysql_get_server_info(), '4.1', '>=' ) )
        mysql_query( "SET NAMES 'utf8'" );

    return true;
}

function wi_db_preprocess_query( $query )
{
    return $query;
}

function wi_db_escape_arg( $arg, $type )
{
    switch( $type ) {
        case 'd':
            return (int)$arg;
        case 's':
        case 'b':
            return "'" . mysql_escape_string( $arg ) . "'";
    }
}

function wi_db_query( $query )
{
    return @mysql_query( $query );
}

function wi_db_fetch_assoc( $rs )
{
    return mysql_fetch_assoc( $rs );
}

function wi_db_num_rows( $rs )
{
    if ( $rs === true )
        return mysql_affected_rows();
    return mysql_num_rows( $rs );
}

function wi_db_unescape_blob( $data )
{
    return $data;
}

function wi_db_insert_id( $table, $column )
{
    return mysql_insert_id();
}

function wi_db_table_exists( $table )
{
    $query = "SHOW TABLES LIKE '$table'";
    $rs = mysql_query( $query );
    return mysql_num_rows( $rs ) > 0;
}

function wi_db_error()
{
    return mysql_error();
}
