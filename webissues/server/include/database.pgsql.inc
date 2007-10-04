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
    global $wi_db_connected;

    $parts = explode( ':', $host );
    if ( isset( $parts[ 1 ] ) ) {
        $host = $parts[ 0 ];
        $port = $parts[ 1 ];
    } else {
        $port = '';
    }

    $string = '';

    if ( $host != '' )
        $string .= "host=$host";
    if ( $port != '' )
        $string .= " port=$port";
    if ( $database != '' )
        $string .= " dbname=$database";
    if ( $user != '' )
        $string .= " user=$user";
    if ( $password != '' )
        $string .= " password=$password";

    if ( !@pg_connect( $string ) )
        return false;

    $wi_db_connected = true;

    return true;
}

function wi_db_preprocess_query( $query )
{
    return str_replace( ' = BINARY ', ' = ', $query );
}

function wi_db_escape_arg( $arg, $type )
{
    switch( $type ) {
        case 'd':
            return (int)$arg;
        case 's':
            return "'" . pg_escape_string( $arg ) . "'";
        case 'b':
            return "'" . pg_escape_bytea( $arg ) . "'";
    }
}

function wi_db_query( $query )
{
    return @pg_query( $query );
}

function wi_db_fetch_assoc( $rs )
{
    return pg_fetch_assoc( $rs );
}

function wi_db_num_rows( $rs )
{
    return pg_num_rows( $rs ) + pg_affected_rows( $rs );
}

function wi_db_unescape_blob( $data )
{
    return pg_unescape_bytea( $data );
}

function wi_db_insert_id( $table, $column )
{
    $query = "SELECT last_value FROM ${table}_${column}_seq";
    $rs = pg_query( $query );
    $row = pg_fetch_row( $rs );
    return $row[ 0 ];
}

function wi_db_table_exists( $table )
{
    $query = "SELECT relname FROM pg_class WHERE relkind = 'r' AND relname = '$table'";
    $rs = pg_query( $query );
    return pg_num_rows( $rs ) > 0;
}

function wi_db_error()
{
    global $wi_db_connected;

    if ( !$wi_db_connected )
        return "Connection to the database failed";

    $error = pg_last_error();

    if ( substr( $error, 0, 6 ) == "ERROR:" )
        $error = substr( $error, 6 );
    return trim( $error );
}
