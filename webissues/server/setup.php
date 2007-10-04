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

require_once( 'config/config.inc' );

require_once( 'include/database.inc' );

define( 'VERSION', '0.8.2' );

define( 'SCRIPT', 'setup.php' );

$page_titles = array(
    'welcome' => 'Welcome',
    'config' => 'Configuration',
    'tables' => 'Data Tables',
    'name' => 'Server Name',
    'admin' => 'Administrator',
    'types' => 'Issue Types',
    'finished' => 'Finished'
);

$last_page = $_POST[ 'page' ];
$page = $last_page;
$body = '';

function setup_db_error_handler( $error )
{
    global $body;

    $body .= "<p><strong>ERROR:</strong> $error.</p>\n";
}

wi_set_db_error_handler( 'setup_db_error_handler' );

setup_execute();
setup_display();

function setup_display()
{
    global $last_page, $page, $body;

    switch ( $page ) {
    case 'welcome':
        $body .= "<p>This script will help you configure the WebIssues server.</p>\n";
        $body .= "<p>Edit the configuration file before you continue.</p>\n";
        break;

    case 'config':
        if ( $page != $last_page )
            $body .= "<p>The server configuration will be verified.</p>\n";
        break;

    case 'tables':
        if ( $page != $last_page )
            $body .= "<p>The database tables will be created.</p>\n";
        break;

    case 'name':
        if ( $page != $last_page )
            $body .= "<p>The server name will be configured.</p>\n";
        $body .= "<p>Enter server name: <input name=\"name\" /></p>\n";
        break;

    case 'admin':
        if ( $page != $last_page )
            $body .= "<p>The Administrator user will be created.</p>\n";
        $body .= "<p>Login: admin</p>\n";
        $body .= "<p>Enter password: <input type=\"password\" name=\"pwd\" /></p>\n";
        $body .= "<p>Confirm password: <input type=\"password\" name=\"pwd2\" /></p>\n";
        break;

    case 'types':
        if ( $page != $last_page )
            $body .= "<p>A set of example issue types will be imported.</p>\n";
        break;

    case 'finished':
        $body .= "<p>The WebIssues setup is now complete.</p>\n";
        break;
    }
}

function setup_execute()
{
    global $last_page, $page, $body;
    global $config;

    switch ( $last_page ) {
    case null:
        $page = 'welcome';
        break;

    case 'welcome':
        $page = 'config';
        break;

    case 'config':
        $engine = $config[ 'db_engine' ];
        if ( $engine != 'mysql' && $engine != 'pgsql' ) {
            $body .= "<p><strong>ERROR:</strong> Database connection is not configured correctly.</p>\n";
            return;
        }

        if ( !wi_open_database() )
            return;

        if ( !empty( $config[ 'storage_path' ] ) ) {
            if ( !is_writable( $config[ 'storage_path' ] ) ) {
                $body .= "<p><strong>ERROR:</strong> The storage folder does not exist or is not writable.</p>\n";
                return;
            }
        }

        if ( wi_table_exists( 'server' ) ) {
            $body .= "<p><strong>NOTE:</strong> The data tables already exist in the database.</p>\n";
            $body .= "<p>Use a different prefix or drop the tables before running this script.\n";
            $body .= "Use <a href=\"setup.php\">update.php</a> if you want to update the existing tables.</p>\n";
            return;
        }

        $body .= "<p>Server is configured correctly.</p>\n";
        $page = 'tables';
        break;

    case 'tables':
        if ( !wi_open_database() )
            return;

        $engine = $config[ 'db_engine' ];

        if ( $engine == 'mysql' ) {
            if ( version_compare( mysql_get_server_info(), '4.1', '>=' ) )
                $engine = 'mysql41';
            else
                $engine = 'mysql40';
        }

        $lines = file( "setup/database.$engine.sql" );
        $filtered = array();

        foreach ( $lines as $line ) {
            $line = trim( $line );
            if ( $line != '' && substr( $line, 0, 2 ) != '--' )
                $filtered[] = $line;
        }

        $queries = explode( ';', implode( ' ', $filtered ) );
        array_pop( $queries );

        foreach ( $queries as $query ) {
            $query = trim( $query );
            if ( !wi_query( $query ) )
                return;
        }

        $body .= "<p>All tables were created successfully.</p>\n";
        $page = 'name';
        break;

    case 'name':
        if ( !wi_open_database() )
            break;

        if ( !strlen( $_GET[ 'name' ] ) ) {
            $body .= "<p><strong>ERROR:</strong> You have to enter the name of the server.</p>\n";
            break;
        }

        $uuid = sprintf( '%04x%04x-%04x-%04x-%04x-%04x%04x%04x',
            mt_rand( 0, 0xffff ), mt_rand( 0, 0xffff ), mt_rand( 0, 0xffff ),
            mt_rand( 0, 0x0fff ) | 0x4000, mt_rand( 0, 0x3fff ) | 0x8000,
            mt_rand( 0, 0xffff ), mt_rand( 0, 0xffff ), mt_rand( 0, 0xffff ) );

        $query = 'INSERT INTO {server} ( server_name, server_uuid, db_version ) VALUES ( %s, %s, %s )';

        if ( !wi_query( $query, $_GET[ 'name' ], $uuid, VERSION ) )
            return;

        $body .= "<p>The server name was configured successfully.</p>\n";
        $page = 'admin';
        break;

    case 'admin':
        if ( !wi_open_database() )
            return;

        if ( !strlen( $_GET[ 'pwd' ] ) ) {
            $body .= "<p><strong>ERROR:</strong> You have to enter the password for the Administrator user.</p>\n";
            return;
        }

        if ( $_GET[ 'pwd' ] != $_GET[ 'pwd2' ] ) {
            $body .= "<p><strong>ERROR:</strong> The password was misspelled, please type it again.</p>\n";
            return;
        }

        $query = 'INSERT INTO {users} ( user_login, user_name, user_passwd, user_access )';
        $query .= ' VALUES ( \'admin\', \'Administrator\', MD5(%s), 2 )';

        if ( !wi_query( $query, $_GET[ 'pwd' ] ) )
            return;

        $body .= "<p>The Administrator user was created successfully.</p>\n";
        $page = 'types';
        break;

    case 'types':
        if ( !wi_open_database() )
            return;

        $lines = file( 'setup/types.ini' );
        $types = array();
        $type = null;

        foreach ( $lines as $line ) {
            $line = trim( $line );
            if ( $line == '' || $line{ 0 } == ';' )
                continue;
            if ( $line{ 0 } == '[' ) {
                $type = substr( $line, 1, -1 );
                $types[ $type ] = array();
            } else {
                list( $name, $def ) = explode( '=', $line, 2 );
                $types[ $type ][ $name ] = $def;
            }
        }

        foreach ( $types as $type => $attrs ) {
            $query = 'INSERT INTO {issue_types} ( type_name ) VALUES ( %s )';

            if ( !wi_query( $query, $type ) )
                return;

            $type_id = wi_insert_id( 'issue_types', 'type_id' );

            foreach ( $attrs as $attr => $def ) {
                $query = 'INSERT INTO {attr_types} ( type_id, attr_name, attr_def )'
                    . ' VALUES ( %d, %s, %s )';

                if ( !wi_query( $query, $type_id, $attr, $def ) )
                    return;
            }
        }

        $body .= "<p>The issue types were imported successfully.</p>\n";
        $page = 'finished';
        break;
    }
}

require_once( 'setup/wizard.inc' );
