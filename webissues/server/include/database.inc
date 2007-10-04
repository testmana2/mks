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

switch ( $config[ 'db_engine' ] ) {
    case 'mysql':
        require_once( 'database.mysql.inc' );
        break;
    case 'pgsql':
        require_once( 'database.pgsql.inc' );
        break;
}

function wi_open_database()
{
    global $config;

    set_magic_quotes_runtime( 0 );

    if ( !wi_db_connect( $config[ 'db_host' ], $config[ 'db_database' ], $config[ 'db_user' ], $config[ 'db_password' ] ) ) {
        $error = wi_db_error();
        wi_call_db_error_handler( $error );
        return false;
    }

    ignore_user_abort( true );

    return true;
}

function wi_query( $query )
{
    $args = func_get_args();
    array_shift( $args );

    return wi_query_args( $query, $args );
}

function wi_query_row( $query )
{
    if ( func_num_args() > 1 ) {
        $args = func_get_args();
        array_shift( $args );
    } else {
        $args = null;
    }

    $rs = wi_query_args( $query, $args );
    if ( !$rs )
        return false;

    return wi_db_fetch_assoc( $rs );
}

function wi_query_args( $query, $args )
{
    global $config;

    $query = str_replace( '{', $config[ 'db_prefix' ], $query );
    $query = str_replace( '}', '', $query );

    $query = wi_db_preprocess_query( $query );

    $query = wi_build_query( $query, $args );

    $log = wi_log_open( 'sql' );
    if ( $log )
        fwrite( $log, "> $query\n" );

    $rs = wi_db_query( $query );

    if ( !$rs ) {
        $error = wi_db_error();
        wi_call_db_error_handler( $error );
        return false;
    }

    if ( $log ) {
        $rows = wi_db_num_rows( $rs );
        fwrite( $log, "($rows rows)\n" );
    }

    return $rs;
}

function wi_build_query( $query, $args )
{
    $result = '';
    $index = 0;

    $parts = preg_split( '/(%(?:\d+\\$)?[a-z])/', $query, -1, PREG_SPLIT_DELIM_CAPTURE | PREG_SPLIT_NO_EMPTY );
    foreach ( $parts as $part ) {
        if ( $part{0} == '%' ) {
            $type = substr( $part, -1, 1 );
            if ( strlen( $part ) > 2 )
                $index = (int)substr( $part, 1, -2 );
            else
                $index = $index + 1;
            $arg = $args[ $index - 1 ];
            $result .= wi_db_escape_arg( $arg, $type );
        } else {
            $result .= $part;
        }
    }

    return $result;
}

function wi_fetch( $rs )
{
    return wi_db_fetch_assoc( $rs );
}

function wi_unescape_blob( $data )
{
    return wi_db_unescape_blob( $data );
}

function wi_insert_id( $table, $column )
{
    global $config;

    return wi_db_insert_id( $config[ 'db_prefix' ] . $table, $column );
}

function wi_table_exists( $table )
{
    global $config;

    return wi_db_table_exists( $config[ 'db_prefix' ] . $table );
}

function wi_set_db_error_handler( $handler )
{
    global $db_error_handler;

    $db_error_handler = $handler;
}

function wi_call_db_error_handler( $error )
{
    global $db_error_handler;

    if ( isset( $db_error_handler ) && function_exists( $db_error_handler ) )
        call_user_func( $db_error_handler, $error );
}
