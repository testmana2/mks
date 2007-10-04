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

define( 'SCRIPT', 'update.php' );

$page_titles = array(
    'welcome' => 'Welcome',
    'config' => 'Configuration',
    'tables' => 'Data Tables',
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
        $body .= "<p>This script will help you upgrade the WebIssues server.</p>\n";
        $body .= "<p>Edit the configuration file before you continue.</p>\n";
        break;

    case 'config':
        if ( $page != $last_page )
            $body .= "<p>The server configuration will be verified.</p>\n";
        break;

    case 'tables':
        if ( $page != $last_page )
            $body .= "<p>The database tables will be updated.</p>\n";
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

        if ( !wi_table_exists( 'server' ) ) {
            $body .= "<p><strong>ERROR:</strong> The data tables were not found in the database.</p>\n";
            $body .= "<p>Make sure the prefix is correct.\n";
            $body .= "Use <a href=\"setup.php\">setup.php</a> if you want to create new tables.</p>\n";
            return;
        }

        $query = "SELECT db_version FROM {server}";
        $row = wi_query_row( $query );

        $db_version = $row[ 'db_version' ];

        if ( $db_version == VERSION ) {
            $body .= "<p><strong>NOTE:</strong> The database is already up to date.</p>\n";
            return;
        }

        if ( version_compare( $db_version, VERSION, '>=' ) ) {
            $body .= "<p><strong>ERROR:</strong> Current database version ($db_version) is too new for this script.</p>\n";
            return;
        }

        $body .= "<p>Server is configured correctly. Current database version is $db_version.</p>\n";
        $page = 'tables';
        break;

    case 'tables':
        if ( !wi_open_database() )
            return;

        $engine = $config[ 'db_engine' ];

        $query = "SELECT db_version FROM {server}";
        $row = wi_query_row( $query );

        $db_version = $row[ 'db_version' ];

        if ( $engine == 'mysql' && $db_version == '0.8.1' ) {
            $query = "ALTER TABLE {files} MODIFY file_data longblob";
            wi_query( $query );
            $query = "ALTER TABLE {files} ADD file_storage tinyint(4) NOT NULL default '0'";
            wi_query( $query );
        }

        if ( $engine == 'pgsql' && $db_version == '0.8.1' ) {
            $query = "ALTER TABLE {files} ALTER COLUMN file_data DROP NOT NULL";
            wi_query( $query );
            $query = "ALTER TABLE {files} ADD COLUMN file_storage smallint NOT NULL default '0'";
            wi_query( $query );
        }

        $query = "UPDATE {server} SET db_version = %s";
        wi_query( $query, VERSION );

        $body .= "<p>Database tables were updated successfully.</p>\n";
        $page = 'finished';
        break;
    }
}

require_once( 'setup/wizard.inc' );
