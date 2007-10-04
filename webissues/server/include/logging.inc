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

function wi_log_open( $name )
{
    global $config;
    static $open_logs;

    if ( isset( $open_logs[ $name ] ) )
        return $open_logs[ $name ];

    if ( isset( $config[ "log_$name" ] ) && $config[ "log_$name" ] ) {
        $log = @fopen( $config[ "log_$name" ], 'a' );
        if ( $log ) {
            wi_log_write_header( $log );
            $open_logs[ $name ] = $log;
            return $log;
        }
    }

    return false;
}

function wi_log_write_header( $log )
{
    $date = date( 'Y-m-d H:i:s' );

    if ( isset( $_SERVER[ 'REMOTE_HOST' ] ) )
        $host = $_SERVER[ 'REMOTE_HOST' ];
    else if ( isset( $_SERVER[ 'REMOTE_ADDR' ] ) )
        $host = $_SERVER[ 'REMOTE_ADDR' ];
    else
        $host = 'unknown';

    fwrite( $log, "\n[$date, $host]\n" );
}
